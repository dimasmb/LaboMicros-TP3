
#include "FTM.h"
// #include "PORT.h"
#include "gpio.h"



void IC_Init(FTM_t ftm, int channel);
void OC_Init(FTM_t ftm, int channel);
void PWM_Init(FTM_t ftm, int channel);
void PWM_ISR(void);

uint16_t PWM_modulus = 10000-1;
uint16_t PWM_duty    = 1000;//5000-1;

void FTM_Init (FTMModule_t module, FTMMode_t mode, int channel)
{
	static FTM_t mod;
	switch (module)
	{
		case FTM_0:
			SIM->SCGC6 |= SIM_SCGC6_FTM0_MASK;
			NVIC_EnableIRQ(FTM0_IRQn);
			FTM0->PWMLOAD = FTM_PWMLOAD_LDOK_MASK | 0x0F;
			FTM0->MODE=(FTM0->MODE & ~FTM_MODE_FTMEN_MASK) | FTM_MODE_FTMEN(1);
			mod = FTM0;
			break;

		case FTM_1:
			SIM->SCGC6 |= SIM_SCGC6_FTM1_MASK;
			NVIC_EnableIRQ(FTM1_IRQn);
			FTM1->PWMLOAD = FTM_PWMLOAD_LDOK_MASK | 0x0F;
			FTM1->MODE=(FTM1->MODE & ~FTM_MODE_FTMEN_MASK) | FTM_MODE_FTMEN(1);
			mod = FTM1;
			break;

		case FTM_2:
			SIM->SCGC6 |= SIM_SCGC6_FTM2_MASK;
			SIM->SCGC3 |= SIM_SCGC3_FTM2_MASK;
			NVIC_EnableIRQ(FTM2_IRQn);
			FTM2->PWMLOAD = FTM_PWMLOAD_LDOK_MASK | 0x0F;
			FTM2->MODE=(FTM2->MODE & ~FTM_MODE_FTMEN_MASK) | FTM_MODE_FTMEN(1);
			mod = FTM2;
			break;

		case FTM_3:
			SIM->SCGC3 |= SIM_SCGC3_FTM3_MASK;
			NVIC_EnableIRQ(FTM3_IRQn);
			FTM3->PWMLOAD = FTM_PWMLOAD_LDOK_MASK | 0x0F;
			FTM3->MODE=(FTM3->MODE & ~FTM_MODE_FTMEN_MASK) | FTM_MODE_FTMEN(1);
			mod = FTM3;
			break;
			
		default:
			break;
	}

	switch(mode)
	{
		case FTM_mInputCapture:
			IC_Init(mod, channel);
			break;
		case FTM_mOutputCompare:
			OC_Init(mod, channel);
			break;
		case FTM_mPulseWidthModulation:
			PWM_Init(mod, channel);
			break;

	}
}

/* FTM0 fault, overflow and channels interrupt handler*/
__ISR__ FTM0_IRQHandler(void)
{
	//chequear en que modo estoy
	//chequear que flag se prendio (OVF o channel)
	//bajar el flag
	//operar
	PWM_ISR();
}

void PWM_ISR (void)
{
	FTM_ClearOverflowFlag (FTM0);

	FTM_SetCounter(FTM0, 0, PWM_duty++);  //change DC
	//GPIO_Toggle(PTC, 1 << 8);			  //GPIO pin PTC8
	PWM_duty %= PWM_modulus;
}

/**********************************
* INICIALIZADORES
******************/
void PWM_Init (FTM_t ftm, int channel)
{
	/* SETEO LOS PINES
	// PTC 1 as PWM
		PCRstr UserPCR;

		UserPCR.PCR=false;			// Default All false, Set only those needed

		UserPCR.FIELD.DSE=true;
		UserPCR.FIELD.MUX=PORT_mAlt4;
		UserPCR.FIELD.IRQC=PORT_eDisabled;

		PORT_Configure2 (PORTC,1,UserPCR);

	// PTC 8 as GPIO
		UserPCR.PCR=false;			// Default All false, Set only those needed

		UserPCR.FIELD.DSE=true;
		UserPCR.FIELD.MUX=PORT_mGPIO;
		UserPCR.FIELD.IRQC=PORT_eDisabled;

		PORT_Configure2 (PORTC,8,UserPCR);

		GPIO_SetDirection(PTC, 8, GPIO__OUT);
		*/



	//Prescaler: Bus clock (50MHz)/32
	FTM_SetPrescaler(ftm, FTM_PSC_x32);

	//Initial value = 0 ; counter = 0 (inicializo) ; max_cont = PWM_modulus
	FTM_SetModulus(ftm, PWM_modulus);

	//Habilito interrupciones de overflow
	FTM_SetOverflowMode(ftm, true);

	//Pongo cierto canal como PWM
	FTM_SetWorkingMode(ftm, channel, FTM_mPulseWidthModulation);			// MSA  / B

	//High-true pulses (clear Output on match)
	FTM_SetPulseWidthModulationLogic(ftm, channel, FTM_lAssertedHigh);   // ELSA / B

	//"comparator" value = PWM_duty (donde hace el clear)
	FTM_SetCounter(ftm, channel, PWM_duty);

	//Inicio el clock
	FTM_StartClock(ftm);
}

void OC_Init (FTM_t ftm, int channel)
{

	//Set up pin 1 of PORTC FTM0_CH0
	/*SETEO PINES
	PCRstr UserPCR;

	UserPCR.PCR=false;			// Default All false, Set only those needed
	UserPCR.FIELD.DSE=true;
	UserPCR.FIELD.MUX=PORT_mAlt4;
	UserPCR.FIELD.IRQC=PORT_eDisabled;
	PORT_Configure2 (PORTC,1,UserPCR);

	//Set up pin 8 of PORTC as output for debug OC interrupt

	UserPCR.PCR=false;			// Default All false, Set only those needed
	UserPCR.FIELD.DSE=true;
	UserPCR.FIELD.MUX=PORT_mGPIO;
	UserPCR.FIELD.IRQC=PORT_eDisabled;
	PORT_Configure2 (PORTC,8,UserPCR);
	GPIO_SetDirection(PTC, 8, GPIO__OUT);
	*/
	//  Set up timer for OC interrupt

	//  Enable Timer advanced modes (FTMEN=1)

	// FTM0->MODE=(FTM0->MODE & ~FTM_MODE_FTMEN_MASK) | FTM_MODE_FTMEN(1);

	/// BusClock=sysclk/2= 50MHz
	/// Set prescaler = divx32 => timer clock = 32 x (1/BusClock)= 32x1/50MHz= 0.64 useg
	/// =>Timer Clock x OC_Delta= 0.64 x 100=64 useg
	FTM_SetPrescaler(ftm, FTM_PSC_x32);
	/// Free running
	FTM0->CNTIN=0x0000;
	FTM0->MOD=0xFFFF;

	/// Set OC Mode
	FTM_SetWorkingMode(ftm, channel,FTM_mOutputCompare);
	// Set OC output in  toggle mode
	FTM_SetOutputCompareEffect (ftm, channel,FTM_eToggle);
	// Enable interrupts
	FTM_SetInterruptMode (ftm, channel, true);
	FTM_StartClock(ftm); //Select BusClk
}

void IC_Init (FTM_t ftm, int channel)
{
	/*SETEO PINES
	// PTC8 as GPIO
	UserPCR.PCR=false;			// Default All false, Set only those needed

	UserPCR.FIELD.DSE=true;
	UserPCR.FIELD.MUX=PORT_mGPIO;
	UserPCR.FIELD.IRQC=PORT_eDisabled;

	PORT_Configure2 (PORTC,8,UserPCR);

	GPIO_SetDirection(PTC, 8, GPIO__OUT);


	// PTC9 as IC (FTM3-CH5)
	UserPCR.PCR=false;			// Default All false, Set only those needed

	UserPCR.FIELD.DSE=true;
	UserPCR.FIELD.MUX=PORT_mAlt3;
	UserPCR.FIELD.IRQC=PORT_eDisabled;

	PORT_Configure2 (PORTC,9,UserPCR);
	*/

	//--- medidor

	FTM_SetPrescaler(ftm, FTM_PSC_x32);	 				// Set Prescaler = divx32
	FTM3->CNTIN=0x0000;				  		  				// Free running
	FTM3->MOD=0xFFFF;
	FTM_SetWorkingMode(ftm, channel, FTM_mInputCapture);   // Select IC Function
	FTM_SetInputCaptureEdge (ftm, channel, FTM_eEither);  // Capture on both edges
	FTM_SetInterruptMode (ftm, channel, true);            // Enable interrupts
	FTM_StartClock(ftm);                                  // Select BusClk
}

//************************************************************************************
// Setters

void FTM_SetPrescaler (FTM_t ftm, FTM_Prescal_t data)
{
	ftm->SC = (ftm->SC & ~FTM_SC_PS_MASK) | FTM_SC_PS(data);
}

void FTM_SetModulus (FTM_t ftm, FTMData_t data)
{
	ftm->CNTIN = 0X00;
	ftm->CNT = 0X00;
	ftm->MOD = FTM_MOD_MOD(data);
}

FTMData_t FTM_GetModulus (FTM_t ftm)
{
	return ftm->MOD & FTM_MOD_MOD_MASK;
}

void FTM_StartClock (FTM_t ftm)
{
	ftm->SC |= FTM_SC_CLKS(0x01);
}

void FTM_StopClock (FTM_t ftm)
{
	ftm->SC &= ~FTM_SC_CLKS(0x01);
}

void FTM_SetOverflowMode (FTM_t ftm, bool mode)
{
	ftm->SC = (ftm->SC & ~FTM_SC_TOIE_MASK) | FTM_SC_TOIE(mode);
}

bool FTM_IsOverflowPending (FTM_t ftm)
{
	return ftm->SC & FTM_SC_TOF_MASK;
}

void FTM_ClearOverflowFlag (FTM_t ftm)
{
	ftm->SC &= ~FTM_SC_TOF_MASK;
}

void FTM_SetWorkingMode (FTM_t ftm, FTMChannel_t channel, FTMMode_t mode)
{
	ftm->CONTROLS[channel].CnSC = (ftm->CONTROLS[channel].CnSC & ~(FTM_CnSC_MSB_MASK | FTM_CnSC_MSA_MASK)) |
			                      (FTM_CnSC_MSB((mode >> 1) & 0X01) | FTM_CnSC_MSA((mode >> 0) & 0X01));
}

FTMMode_t FTM_GetWorkingMode (FTM_t ftm, FTMChannel_t channel)
{
	return (ftm->CONTROLS[channel].CnSC & (FTM_CnSC_MSB_MASK | FTM_CnSC_MSA_MASK)) >> FTM_CnSC_MSA_SHIFT;
}

void FTM_SetInputCaptureEdge (FTM_t ftm, FTMChannel_t channel, FTMEdge_t edge)
{
	ftm->CONTROLS[channel].CnSC = (ftm->CONTROLS[channel].CnSC & ~(FTM_CnSC_ELSB_MASK | FTM_CnSC_ELSA_MASK)) |
				                  (FTM_CnSC_ELSB((edge >> 1) & 0X01) | FTM_CnSC_ELSA((edge >> 0) & 0X01));
}

FTMEdge_t FTM_GetInputCaptureEdge (FTM_t ftm, FTMChannel_t channel)
{
	return (ftm->CONTROLS[channel].CnSC & (FTM_CnSC_ELSB_MASK | FTM_CnSC_ELSA_MASK)) >> FTM_CnSC_ELSA_SHIFT;
}

void FTM_SetOutputCompareEffect (FTM_t ftm, FTMChannel_t channel, FTMEffect_t effect)
{
	ftm->CONTROLS[channel].CnSC = (ftm->CONTROLS[channel].CnSC & ~(FTM_CnSC_ELSB_MASK | FTM_CnSC_ELSA_MASK)) |
				                  (FTM_CnSC_ELSB((effect >> 1) & 0X01) | FTM_CnSC_ELSA((effect >> 0) & 0X01));
}

FTMEffect_t FTM_GetOutputCompareEffect (FTM_t ftm, FTMChannel_t channel)
{
	return (ftm->CONTROLS[channel].CnSC & (FTM_CnSC_ELSB_MASK | FTM_CnSC_ELSA_MASK)) >> FTM_CnSC_ELSA_SHIFT;
}

void FTM_SetPulseWidthModulationLogic (FTM_t ftm, FTMChannel_t channel, FTMLogic_t logic)
{
	ftm->CONTROLS[channel].CnSC = (ftm->CONTROLS[channel].CnSC & ~(FTM_CnSC_ELSB_MASK | FTM_CnSC_ELSA_MASK)) |
				                  (FTM_CnSC_ELSB((logic >> 1) & 0X01) | FTM_CnSC_ELSA((logic >> 0) & 0X01));
}

FTMLogic_t FTM_GetPulseWidthModulationLogic (FTM_t ftm, FTMChannel_t channel)
{
	return (ftm->CONTROLS[channel].CnSC & (FTM_CnSC_ELSB_MASK | FTM_CnSC_ELSA_MASK)) >> FTM_CnSC_ELSA_SHIFT;
}

void FTM_SetCounter (FTM_t ftm, FTMChannel_t channel, FTMData_t data)
{
	ftm->CONTROLS[channel].CnV = FTM_CnV_VAL(data);
}

FTMData_t FTM_GetCounter (FTM_t ftm, FTMChannel_t channel)
{
	return ftm->CONTROLS[channel].CnV & FTM_CnV_VAL_MASK;
}

void FTM_SetInterruptMode (FTM_t ftm, FTMChannel_t channel, bool mode)
{
	ftm->CONTROLS[channel].CnSC = (ftm->CONTROLS[channel].CnSC & ~FTM_CnSC_CHIE_MASK) | FTM_CnSC_CHIE(mode);
}

bool FTM_IsInterruptPending (FTM_t ftm, FTMChannel_t channel)
{
	return ftm->CONTROLS[channel].CnSC & FTM_CnSC_CHF_MASK;
}

void FTM_ClearInterruptFlag (FTM_t ftm, FTMChannel_t channel)
{
	ftm->CONTROLS[channel].CnSC &= ~FTM_CnSC_CHF_MASK;
}

