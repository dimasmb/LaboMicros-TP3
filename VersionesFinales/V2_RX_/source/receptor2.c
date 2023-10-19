#include "receptor2.h"
#include "uart_drv.h"
#include "DMA.h"
void CMP_INIT();
void IC_ISR(FTM_Type* ftm, int channel);

//static char newbit = 0;

#define BUFFSIZE 1000


int insertChar(char);
char getChar();
void clearBuff(void);
void newbit_process();
void newmed_process();

static int dist=BUFFSIZE-1;
static int outPointer=0;
static int inPointer=0;
static char cirBuff[BUFFSIZE];
static uint16_t new_IC_measurement = 0;


void receptor2_init(){
	UART_Init(1200, 2);
	CMP_INIT();
	FTM_Init(FTM_1, FTM_mInputCapture, FTM_CH_0);
	SIM->SOPT4 |= SIM_SOPT4_FTM1CH0SRC(1);
	DMAConfig_t my_dma_config;
	my_dma_config.sourceChannel = sFTM1_CH0;
	my_dma_config.sourceAddr = &(FTM1->CONTROLS[0].CnV);
	my_dma_config.cant = 1;
	my_dma_config.destAddr = &new_IC_measurement;
	my_dma_config.sourceOff = 0;
	my_dma_config.destOff = 0;
	FTM_DmaMode(FTM1, FTM_CH_0, true);
	FTM_SetInterruptMode(FTM1, FTM_CH_0, true);
	DMA_init(0, my_dma_config);

}

void receptor_2_poll(){


	int var = (DMA0->TCD[0].CSR) & DMA_CSR_DONE_MASK;

	newmed_process();
	if(inPointer!=outPointer){
		newbit_process();
	}
}


typedef enum
{
	state_reading,
	state_idle,
	state_start
} readmachine_state;

//static int debugarr[150];
//static int i = 0;

void newbit_process() {
	static readmachine_state state = state_start;
	static char char2send = 0;
	static char counter = 0;
	static char start_counter = 0;

	//newbit = 0;
	char bit = getChar();
	if (state == state_idle && bit == 0) {
		state = state_reading;
	} else if (state == state_reading) {
		counter++;
		if (counter < 9) {
			char2send *= 2;
			char2send += bit;
		} else if (counter == 10) {
			char aux = 0;
			for(int e=0; e<8;e++){
				aux*=2;
				aux += 1&(char2send>>e);

			}
			UART_Send_Data(&aux, 1);
			char2send = 0;
			counter = 0;
			state = state_idle;
		}
	} else if (state == state_start) {
		if (bit == 1) {
			start_counter++;
			if (start_counter == 2) {
				state = state_idle;
			}
		} else {
			start_counter = 0;
		}
	}
}



void newmed_process(){

	static uint16_t med1,med2,med;
	static uint8_t  state = 0;
	static char zero_counter = 0;
	static char one_counter = 0;


	if(state==1)
	{
		med1 = med2;
		med2 = new_IC_measurement;

		med=med2-med1;

		/*if (med>50){
			debugarr[i] = med;
			i++;
		}*/

		if (med>475 && ((++one_counter)==2)){
			one_counter=0;
			//newbit=1;

			insertChar(1);
		}
		else if (med>50 && 475>med && ((++zero_counter)==4)){
			zero_counter=0;
			//newbit=1;
			insertChar(0);
		}
	}
	else
	{
		med2=new_IC_measurement;
		state=1;
	}
}


void CMP_INIT() {


	/*
	 *
	 SIM_SCGC4->cmp

	CMPx->CR1
		{
			PMODE: 1    High-Speed (HS) Comparison mode selected. In this mode, CMP has faster output propagation delay
						and higher current consumption.
			OPE: 1      CMPO is available on the associated CMPO output pin.
						The comparator output (CMPO) is driven out on the associated CMPO output pin if the comparator
						owns the pin. If the comparator does not own the field, this bit has no effect.
			COS: 1      Set the unfiltered comparator output (CMPO) to equal COUTA
			EN: 1       Analog Comparator is enabled.
		}

	CMPx->DACCR
		{
			DACEN: 1        DAC is enabled
			VRSEL: 0        Vin1 is selected as resistor ladder network supply reference.
			VOSEL: 011111   DACO = (V in /64) * (VOSEL[5:0] + 1) , so the DACO range is from V in /64 to V in .

		}

	CMP->MUXCR
		{
			PSEL: 111 (IN7, debería ser el DAC)
			MSEL: 000 (IN0)
		}

	 */
	SIM->SCGC4 |= SIM_SCGC4_CMP_MASK;

	CMP_Type* my_cmp = CMP0;



	my_cmp->CR1 |= CMP_CR1_EN_MASK;
//my_cmp->CR1 |= CMP_CR1_PMODE_MASK;
	my_cmp->CR1 |= CMP_CR1_OPE_MASK;

	my_cmp->CR1 |= CMP_CR1_SE(0);
	my_cmp->CR0 |= CMP_CR0_FILTER_CNT(0);
	my_cmp->CR0 |= CMP_CR0_FILTER_CNT(0b111);

	my_cmp->CR0 |= CMP_CR0_HYSTCTR(3);


	my_cmp->DACCR |= CMP_DACCR_DACEN_MASK;
	my_cmp->DACCR |= CMP_DACCR_VOSEL(31); // Vo = (31+1)*Vi/64
	my_cmp->DACCR |= CMP_DACCR_VRSEL_MASK;

	my_cmp->MUXCR |= CMP_MUXCR_MSEL(1); // entrada ptc 7
	my_cmp->MUXCR |= CMP_MUXCR_PSEL(7); // salida del dac



	//MUXEO pines
	// PTB 20, ALT 6, CMP0_OUT
	// PTC 7, ALT 0, CMP0_IN1

	PORT_Type* ptb = PORTB;
	ptb->PCR[20] |= PORT_PCR_MUX(6);

	PORT_Type* ptc = PORTC;
	ptc->PCR[7] |= PORT_PCR_MUX(0);
	ptc->PCR[8] |= PORT_PCR_MUX(0);




}




void clearBuff(void){

    int i=0;
    for (i=0;i<BUFFSIZE;i++){
        cirBuff[i]=0;
    }
}

int insertChar(char data){
    if(dist>0){
        cirBuff[inPointer]=data;
        dist--;
        if(inPointer>=(BUFFSIZE-1)){
            inPointer=0;
        }else{
            inPointer++;
        }
    return 1;
    }
    return 0;

}

char getChar(){
    char result=0;
    if(outPointer-inPointer){
        dist++;
        result=cirBuff[outPointer];
        if(outPointer>=(BUFFSIZE-1)){
            outPointer=0;
        }else{
            outPointer++;
        }
    }
    return result;

}





