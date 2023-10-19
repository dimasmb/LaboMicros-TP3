/***************************************************************************//**
  @file     FSK.c
  @brief    Implementacion de Modulador y Demodulador FSK.
  @author   Enrique Irigoyen
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "FSK.h"
#include "DAC.h"
#include "SysTick.h"
#include "BufferCirc.h"
#include "gpio.h"
#include "ADC.h"
#include "FIR.h"
#include "BufferCirc.h"



/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define MAXPTR 8

#define FSKBITS 11

#define FREQFACTOR (((float)2200)/((float)1200))

#define FIRORDER 7

#define DELAY 3

#define OVERSAMPLES 8

#define MODBUFFERSIZE 10000
#define DEMODBUFFERSIZE 10000

#define CMPTHRESHOLD_H 1750

#define CMPTHRESHOLD_L 1450

#define MARK 1		//estan dados vueltas por la multplicacion
#define SPACE 0		//en realidad seria MARK 1 y SPACE 0
/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
enum Status {
    NODATA,
    FULL
};
/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

void fsk_callback (void);
void filterInput(void);
uint8_t oversamplingResult(uint8_t bitCtr);
uint8_t checkData(void);


/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 *******************************************************************************/

//CONSTANT ARRAYS.

/*const  uint16_t SineBuffer[32] =
{
  2048, 2447, 2831, 3185, 3495, 3750, 3939, 4056,
  4095, 4056, 3939, 3750, 3495, 3185, 2831, 2447,
  2048, 1648, 1264,  910,  600,  345,  156,   39,
     0,   39,  156,  345,  600,  910, 1264, 1648
};*/

const  uint16_t SineBuffer[MAXPTR] = {
    2048,
    3495,
    4095,
    3495,
    2048,
    0600,
    0001,
    0600};



const float filterCoefs[FIRORDER+1]={
    0.031310260236815996848225296389500726946,
    0.114285372475780827583591303664434235543,
    0.229419858052978820062151044112397357821,
    0.316990407868467660712497036001877859235,
    0.316990407868467660712497036001877859235,
    0.229419858052978820062151044112397357821,
    0.114285372475780827583591303664434235543,
    0.031310260236815996848225296389500726946


};

//MODULACION
float SinePointer = 0;
int PointCount= 0;
int dataIn =0;
char isHighFreq=0;
char currBit=1;

circBuffer_t modBuffer;
int16_t modB[MODBUFFERSIZE];




//DEMODULACION
/* DELAY USED 446us -> 434us/Tsample y Tsample=833us/MAXPTR ->434*MAXPTR/833 retrasos de señal. probar piso y techo del valor obtenido  */

circBuffer_t demodBuffer;
int16_t demodB[DEMODBUFFERSIZE];

float delayedInput[DELAY]; // se usa para el delay de la señal de entrada.

float FIRInput[FIRORDER+1]; //se usa como el buffer que guarda la señal que entra al fir.

char isRxReceiving=0;

uint8_t fullRxBuffer=0;

uint8_t overSamplingArray[MAXPTR*FSKBITS]; //se usa para samplear la señal que sale del fir ya comparada. //reviar poque no deberia ser la misma cantidad que muestras.

uint8_t bitCounter=0;

int oversampleCounter=0;

uint8_t RxBuffer[FSKBITS];

adc_t ADCConfs={.modo=ADC_b12s,.clk=bus_clk,.k=clk_1,.e_interrup=0}; 

uint8_t tempArray[10000];
int tempArrayCounter=0;

int prevState=1;
int reachState=1;
int shiftCounter =0;


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void Init_FSK(void){

    modBuffer.Buffer=modB;
    modBuffer.buffSize=MODBUFFERSIZE;
    modBuffer.d=MODBUFFERSIZE-1;
    modBuffer.inPtr=0;
    modBuffer.outPtr=0;
    clearBuff(&modBuffer);

    demodBuffer.Buffer=demodB;
    demodBuffer.buffSize=DEMODBUFFERSIZE;
    demodBuffer.d=DEMODBUFFERSIZE-1;
    demodBuffer.inPtr=0;
    demodBuffer.outPtr=0;
    clearBuff(&demodBuffer);
    int i=0;
    for(i=0;i<DELAY;i++)
        delayedInput[i]=0;
    prevState=1;
    DAC_Init();
    SysTick_Init(fsk_callback);
    gpioMode(PORTNUM2PIN(PC,10),OUTPUT);
    ADC_Init(ADCConfs);



}

int Tx_Fsk(uint16_t data){
    
    int i=0;
    int out=0;
    if(isBuffEmpty(&modBuffer)){
		for(i=1;i<=FSKBITS;i++){
			insertData(&modBuffer,(data>>(FSKBITS-i))&0x01);
		}
		out=1;
		dataIn+=12;
    }
    return out;


}

uint8_t isRxDataFull(void){
    filterInput();
    return fullRxBuffer;
}

uint16_t getRxData(void){
    fullRxBuffer=0;
    uint16_t result=0;
    int i=0;
    for(i=0;i<FSKBITS;i++){
        result |= RxBuffer[i]<<i;
    }
    return result;
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void fsk_callback (void){
	gpioToggle(PORTNUM2PIN(PC,10));
    //Check status and write DAC.
    if(dataIn && !currBit){
           	DAC_SetData(SineBuffer[(int)SinePointer]);
            SinePointer+=(float)FREQFACTOR;
            isHighFreq+=1;
    }
    else{
    	DAC_SetData(SineBuffer[(int)SinePointer]);
    	SinePointer+=1;

    }

    if(SinePointer>=MAXPTR){
        SinePointer=0;
        if(!isHighFreq && dataIn){
            dataIn--;
            if(dataIn){
                currBit=getData(&modBuffer);
            }else{
            	currBit=1;
            }
        }
        else if(isHighFreq>=7 && dataIn){
            dataIn--;
            isHighFreq=0;
            if(dataIn){
                currBit=getData(&modBuffer);
            }else{
            	currBit=1;
            }
        }
    }
    //Read ADC and save.
    if(Conver_Complete()){
        insertData(&demodBuffer,ADC_Read());
    }
    gpioToggle(PORTNUM2PIN(PC,10));
    
}

void filterInput(void){
    int i=0;
    float ydelayed=0;
    float firOut=0;

    if((demodBuffer.inPtr) - (demodBuffer.outPtr)){
		int16_t inputValue=getData(&demodBuffer);

		delayedInput[0]=(inputValue-2048.0)/2048.0;
		ydelayed=delayedInput[DELAY-1];
		for(i=DELAY-1;i>=1;i--){
			delayedInput[i]=delayedInput[i-1];
		}
		firOut=firFilter(delayedInput[1]*ydelayed,FIRORDER,filterCoefs,FIRInput);
		//float signFirOut=firOut/firOut;
		int16_t result=(int16_t)(firOut*2048.0+2048.0);
		//DAC_SetData(result);
		if(((result > CMPTHRESHOLD_H) && (prevState==1)) || ((result >CMPTHRESHOLD_L) && (prevState==0)) ){
			if(!((result>= CMPTHRESHOLD_L) && (result<= CMPTHRESHOLD_H))){
				prevState=1;
			}
			if(isRxReceiving){
				tempArray[tempArrayCounter]=MARK;
				tempArrayCounter++;
				overSamplingArray[oversampleCounter+(bitCounter*MAXPTR)]=MARK;
				if(oversampleCounter>=(MAXPTR-1)){
						oversampleCounter=-1;
						//RxBuffer[bitCounter]=oversamplingResult(bitCounter);  //esto hay q hacer el algoritmo de sampleo.
						bitCounter++;

				}else{
					oversampleCounter++;
				}
			}
			if(bitCounter==11){
				bitCounter=0;
				oversampleCounter=0;
				isRxReceiving=0;
				fullRxBuffer=checkData();
				if(!fullRxBuffer){
					isRxReceiving=1;
				}
				//bitCounter=0;
				//isRxReceiving=0;
			}
		}
		else if(((result < CMPTHRESHOLD_L) && (prevState==0)) || ((result <CMPTHRESHOLD_H) && (prevState==1))){
			if(!((result>= CMPTHRESHOLD_L) && (result<= CMPTHRESHOLD_H))){
				prevState=0;
			}
			isRxReceiving=1;
			tempArray[tempArrayCounter]=SPACE;
			tempArrayCounter++;
			overSamplingArray[oversampleCounter+(bitCounter*MAXPTR)]=SPACE;
			if(oversampleCounter>=(MAXPTR-1)){
				oversampleCounter=-1;
				//RxBuffer[bitCounter]=oversamplingResult(bitCounter);
				bitCounter++;
				int j=0;

			}else{
				oversampleCounter++;
			}

			if(bitCounter==11){
				bitCounter=0;
				oversampleCounter=0;
				isRxReceiving=0;
				fullRxBuffer=checkData();
				if(!fullRxBuffer){
					isRxReceiving=1;
				}
				//if(RxBuffer[0]==1 || RxBuffer[10]==0)
				//	fullRxBuffer=0;
				//bitCounter=0;
				//isRxReceiving=0;
			}


		}
    }


}

uint8_t checkData(void){
	int i=0;
	int res=0;
	for(i=0;i<FSKBITS;i++){
		RxBuffer[i]=oversamplingResult(i);
	}
	if(!RxBuffer[0]){
		if(RxBuffer[FSKBITS-1]){
			for(i=0;i<FSKBITS;i++){
				res+=RxBuffer[i];
			}
			if(!(res%2))
				return 1;
		}
	}
	int j=0;
	for(j=0;j<((FSKBITS*MAXPTR)-1);j++){
		overSamplingArray[j]=overSamplingArray[j+1];
	}
	//shiftCounter++;
	bitCounter=10;
	oversampleCounter=MAXPTR-1;
	return 0;


}

uint8_t oversamplingResult(uint8_t bitCtr){

	uint8_t result=0;
	result = overSamplingArray[2+(bitCtr*MAXPTR)];
	/*char bitPrev=0;
	if(bitCtr)
		 bitPrev=RxBuffer[bitCtr-1];
	char tail=overSamplingArray[7+(bitCtr*MAXPTR)];
	char sum = overSamplingArray[3+(bitCtr*MAXPTR)] + overSamplingArray[4+(bitCtr*MAXPTR)] + overSamplingArray[5+(bitCtr*MAXPTR)];
	if(sum>=2){
		result=1;
	}else if(sum==0){
		result=0;
	}else{
		if(bitPrev && tail==0){
			result=0;
		}else if(bitPrev && tail==1){
			result=1;
		}else if(tail==1){
			result=1;
		}else{
			result=0;
		}
	}*/
	return result;
}

