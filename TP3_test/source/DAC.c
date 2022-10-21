
#include "DAC.h"
#include "Lookup_sine.h"
#define DAC_DATL_DATA0_WIDTH 8


DAC_Type DAC;

void DAC_Init (void)
{
	SIM->SCGC2 |= SIM_SCGC2_DAC0_MASK;	//se habilita el clck gatting
	//RESET CRxs
	uint8_t CR0=0; //Control register cero. => MAs control sobre el DAC por si solo
	uint8_t CR1=0;//Control sobre DMA
	uint8_t CR2=0b00001111;//
	CR0 |=DAC_C0_DACEN(1);	//habilito el dac.
	CR0 |=DAC_C0_DACRFS(1);
	//CR0 &=~DAC_C0_DACRFS(1); //habilito la referencias de tension, 0 para Vref1 VDDA , cero pa Vref0 => Creo que es de 1.2V la referencias intera , sino VDDA la de afuera.
	CR0 |=DAC_C0_DACTRGSEL(1); //habilito trigger por software. 
	//dejo el resto de las configuraciones en cero. 
	DAC0->C0=CR0;
	DAC0->C1=CR1;
	DAC0->C2=CR2;
	//DAC0->C0 = DAC_C0_DACEN_MASK | DAC_C0_DACRFS_MASK | DAC_C0_DACTRGSEL_MASK;
	return;
}

void DAC_SetData (DACData_t data)
{
	DAC0->DAT[0].DATL=DAC_DATL_DATA0(data);
	DAC0->DAT[0].DATH=DAC_DATH_DATA1(data >> DAC_DATL_DATA0_WIDTH);
	return;
}

void Build_trinangle(uint32_t pico,int paso){ //full scale 4095 => 2^12
	uint32_t counter;
	if (paso<=0)
	{
		paso=1;
	}	
	for (counter = 0; counter < pico; )
	{
		DAC_SetData(counter);
		counter=counter+paso;
	}
	for (counter = pico; counter > 0; )
	{
		DAC_SetData(counter);
		counter=counter-paso;
	}
	
	return;
}

void Build_sine(int resolucion,int kfrecuencia){
switch (resolucion)
{
	case 9:
		for (int i = -kfrecuencia; i < 512; i++)		//512 , numero de muestrras.
		{
			DAC_SetData( DACLookup_FullSine_9Bit[i+kfrecuencia]);
		}break;
	case 8:
		for (int i = 0; i < 256; i++)		//512 , numero de muestrras.
		{
		DAC_SetData( DACLookup_FullSine_8Bit[i]);
		}break;
	case 7:
		for (int i = 0; i < 128; i++)		
		{
		DAC_SetData( DACLookup_FullSine_7Bit[i]);
		}break;
	case 6:
		for (int i = 0; i < 64; i++)		
		{
		DAC_SetData( DACLookup_FullSine_6Bit[i]);
		}break;
	case 5:
		for (int i = 0; i < 32; i++)		
		{
		DAC_SetData( DACLookup_FullSine_5Bit[i]);
		}break;
	default:
		for (int i = 0; i < 32; i++)		
		{
		DAC_SetData( DACLookup_FullSine_5Bit[i]);
		}break;
}


return;
} 
