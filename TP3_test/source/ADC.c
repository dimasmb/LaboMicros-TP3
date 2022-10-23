#include "ADC.h"
#define TWO_POW_NUM_OF_CAL (1 << 4)

/*FUNCIONES PRIVADAS*/
void Clock_Gatting_Enable(void);
void Config_CFG1(adc_t adc);//selects the mode of operation, clock source, clockdivide, and configuration for low power or long sample time
void Config_CFG2(adc_t adc);
void Config_SC1(adc_t adc);
void Config_SC2(adc_t adc);
bool ADC_Calibrates(void);
/***********************************/

/*FUNCIONES  PUBLICAS*/
void ADC_Init(adc_t adc){
    Clock_Gatting_Enable();
	NVIC_EnableIRQ(ADC1_IRQn);
	ADC_Calibrates();

    Config_CFG1(adc);

    Config_CFG2(adc);

    Config_SC2(adc);
    Config_SC1(adc);
}

uint16_t ADC_Data(void){
	//ADC_Calibrates();
	return ADC1->R[0];
}

void Start_conv(void){
	ADC1->SC1[0]&=~ADC_SC1_DIFF(1);
}

bool Conver_Complete(void){
	return ADC1->SC1[0]&ADC_SC1_COCO_MASK;	//1  si esta libre
}


/***************************/
void Clock_Gatting_Enable(void){
	SIM->SCGC3 |= SIM_SCGC3_ADC1_MASK;
}

void Config_CFG1(adc_t adc){
    uint32_t CFG1=0; 
    ADC1->CFG1 = CFG1; //Se resetea el registro.

    CFG1|=ADC_CFG1_ADICLK(adc.clk);
    CFG1|=ADC_CFG1_MODE(adc.modo);
    CFG1|=ADC_CFG1_ADLSMP(1);   //long sample time
    CFG1|=ADC_CFG1_ADIV(adc.k);
    CFG1&=~ADC_CFG1_ADLPC(0);
    ADC1->CFG1 = CFG1;
    return;
}
void Config_CFG2(adc_t adc){
	uint32_t CFG2=0;
	ADC1->CFG2=0;	//se retea

	CFG2&=~ADC_CFG2_ADLSTS(1);  //Sampleo por default.
	CFG2&=~ADC_CFG2_ADHSC(1);	//modo normal.
	CFG2&=~ADC_CFG2_ADACKEN(1); //Disable el clock asincronico
	CFG2&=~ADC_CFG2_MUXSEL(1); //elijo el mux a
	ADC1->CFG2=CFG2;
	return;
}
void Config_SC1(adc_t adc){
	uint32_t SC1=0b00000000000000000000000000010010;
	ADC1->SC1[0]=SC1;

	//SC1&=~ADC_SC1_ADCH(10010); //elijo channel DP0
	//SC1=0;
	SC1&=~ADC_SC1_DIFF(1);
	if(adc.e_interrup==true){
		SC1|=ADC_SC1_AIEN(adc.e_interrup);
	}
	ADC1->SC1[0]=SC1;
	return;
}
void Config_SC2(adc_t adc){	//deshabilito  todo comparre, DMA
	uint32_t SC2=0;
	ADC1->SC2=SC2;
	return;
}


bool ADC_Calibrates (void)
{
	int32_t  Offset		= 0;
	uint32_t Minus	[7] = {0,0,0,0,0,0,0};
	uint32_t Plus	[7] = {0,0,0,0,0,0,0};
	uint8_t  i;
	uint32_t scr3;

	/// SETUP
	ADC1->SC1[0] = 0x1F;
	scr3 = ADC1->SC3;
	ADC1->SC3 &= (ADC_SC3_AVGS(0x03) | ADC_SC3_AVGE_MASK);

	/// INITIAL CALIBRATION
	ADC1->SC3 &= ~ADC_SC3_CAL_MASK;
	ADC1->SC3 |=  ADC_SC3_CAL_MASK;
	while (!(ADC1->SC1[0] & ADC_SC1_COCO_MASK));
	if (ADC1->SC3 & ADC_SC3_CALF_MASK)
	{
		ADC1->SC3 |= ADC_SC3_CALF_MASK;
		return false;
	}
	ADC1->PG  |= (0x8000 | ((ADC1->CLP0+ADC1->CLP1+ADC1->CLP2+ADC1->CLP3+ADC1->CLP4+ADC1->CLPS) >> (1 + TWO_POW_NUM_OF_CAL)));
	ADC1->MG  |= (0x8000 | ((ADC1->CLM0+ADC1->CLM1+ADC1->CLM2+ADC1->CLM3+ADC1->CLM4+ADC1->CLMS) >> (1 + TWO_POW_NUM_OF_CAL)));

	// FURTHER CALIBRATIONS
	for (i = 0; i < TWO_POW_NUM_OF_CAL; i++)
	{
		ADC1->SC3 &= ~ADC_SC3_CAL_MASK;
		ADC1->SC3 |=  ADC_SC3_CAL_MASK;
		while (!(ADC1->SC1[0] & ADC_SC1_COCO_MASK));
		if (ADC1->SC3 & ADC_SC3_CALF_MASK)
		{
			ADC1->SC3 |= ADC_SC3_CALF_MASK;
			return 1;
		}
		Offset += (short)ADC1->OFS;
		Plus[0] += (unsigned long)ADC1->CLP0;
		Plus[1] += (unsigned long)ADC1->CLP1;
		Plus[2] += (unsigned long)ADC1->CLP2;
		Plus[3] += (unsigned long)ADC1->CLP3;
		Plus[4] += (unsigned long)ADC1->CLP4;
		Plus[5] += (unsigned long)ADC1->CLPS;
		Plus[6] += (unsigned long)ADC1->CLPD;
		Minus[0] += (unsigned long)ADC1->CLM0;
		Minus[1] += (unsigned long)ADC1->CLM1;
		Minus[2] += (unsigned long)ADC1->CLM2;
		Minus[3] += (unsigned long)ADC1->CLM3;
		Minus[4] += (unsigned long)ADC1->CLM4;
		Minus[5] += (unsigned long)ADC1->CLMS;
		Minus[6] += (unsigned long)ADC1->CLMD;
	}
	ADC1->OFS |= (Offset >> TWO_POW_NUM_OF_CAL);
	ADC1->PG  |= (0x8000 | ((Plus[0] +Plus[1] +Plus[2] +Plus[3] +Plus[4] +Plus[5] ) >> (1 + TWO_POW_NUM_OF_CAL)));
	ADC1->MG  |= (0x8000 | ((Minus[0]+Minus[1]+Minus[2]+Minus[3]+Minus[4]+Minus[5]) >> (1 + TWO_POW_NUM_OF_CAL)));
	ADC1->CLP0 |= (Plus[0] >> TWO_POW_NUM_OF_CAL);
	ADC1->CLP1 |= (Plus[1] >> TWO_POW_NUM_OF_CAL);
	ADC1->CLP2 |= (Plus[2] >> TWO_POW_NUM_OF_CAL);
	ADC1->CLP3 |= (Plus[3] >> TWO_POW_NUM_OF_CAL);
	ADC1->CLP4 |= (Plus[4] >> TWO_POW_NUM_OF_CAL);
	ADC1->CLPS |= (Plus[5] >> TWO_POW_NUM_OF_CAL);
	ADC1->CLPD |= (Plus[6] >> TWO_POW_NUM_OF_CAL);
	ADC1->CLM0 |= (Minus[0] >> TWO_POW_NUM_OF_CAL);
	ADC1->CLM1 |= (Minus[1] >> TWO_POW_NUM_OF_CAL);
	ADC1->CLM2 |= (Minus[2] >> TWO_POW_NUM_OF_CAL);
	ADC1->CLM3 |= (Minus[3] >> TWO_POW_NUM_OF_CAL);
	ADC1->CLM4 |= (Minus[4] >> TWO_POW_NUM_OF_CAL);
	ADC1->CLMS |= (Minus[5] >> TWO_POW_NUM_OF_CAL);
	ADC1->CLMD |= (Minus[6] >> TWO_POW_NUM_OF_CAL);

	/// UN-SETUP
	ADC1->SC3 = scr3;

	return true;
}

