/***************************************************************************//**
  @file     FTM.c
  @brief    Driver de FTM
  @author   Dimas Bosch (con extractos proporcionados por el Prof. Daniel Jacoby)
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "FTM.h"
// #include "PORT.h"
#include "gpio.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

enum{
	FTM0_port = PC,
	FTM0_CH0_pin = 1,
	FTM0_CH1_pin = 2,
	FTM0_CH2_pin = 3,
	FTM0_CH3_pin = 4,

	//FTM1_CH0 = PORTNUM2PIN(),
	//FTM1_CH1 = PORTNUM2PIN(),

	FTM2_port = PB,
	FTM2_CH0_pin = 18,
	FTM2_CH1_pin = 19,

	FTM3_port = PD,
	FTM3_CH0_pin = 0,
	FTM3_CH1_pin = 1,
	FTM3_CH2_pin = 2
};

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

void CH_Init(FTMModule_t ftm, int channel);
void IC_Init(FTM_t ftm, int channel);
void OC_Init(FTM_t ftm, int channel);
void PWM_Init(FTM_t ftm, int channel);
void PWM_ISR(void);

/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static uint16_t PWM_modulus = 10000-1;
static uint16_t PWM_duty    = 1000;

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/*******************************
 * INICIALIZADOR GLOBAL
*****************************/
void FTM_Init (FTMModule_t module, FTMMode_t mode, int channel)
{
	static FTM_t mod;
	switch (module)
	{
		case FTM_0:
			SIM->SCGC6 |= SIM_SCGC6_FTM0_MASK;
			NVIC_EnableIRQ(FTM0_IRQn);
			FTM0->PWMLOAD = FTM_PWMLOAD_LDOK_MASK | 0x0F;
			mod = FTM0;
			break;

		case FTM_1:
			SIM->SCGC6 |= SIM_SCGC6_FTM1_MASK;
			NVIC_EnableIRQ(FTM1_IRQn);
			FTM1->PWMLOAD = FTM_PWMLOAD_LDOK_MASK | 0x0F;
			mod = FTM1;
			break;

		case FTM_2:
			SIM->SCGC6 |= SIM_SCGC6_FTM2_MASK;
			SIM->SCGC3 |= SIM_SCGC3_FTM2_MASK;
			NVIC_EnableIRQ(FTM2_IRQn);
			FTM2->PWMLOAD = FTM_PWMLOAD_LDOK_MASK | 0x0F;
			mod = FTM2;
			break;

		case FTM_3:
			SIM->SCGC3 |= SIM_SCGC3_FTM3_MASK;
			NVIC_EnableIRQ(FTM3_IRQn);
			FTM3->PWMLOAD = FTM_PWMLOAD_LDOK_MASK | 0x0F;
			mod = FTM3;
			break;
			
		default:
			break;
	}

	CH_Init(module, channel);

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

/************************************************************************************
 * Setters
*********************/
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
	if(ftm->SC &FTM_SC_TOIE_MASK)
	{
		return ftm->SC & FTM_SC_TOF_MASK;
	}
	else
	{
		return false;
	}
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
	if(ftm->CONTROLS[channel].CnSC & FTM_CnSC_CHIE_MASK)
	{
		return ftm->CONTROLS[channel].CnSC & FTM_CnSC_CHF_MASK;
	}
	else
	{
		return false;
	}
}

void FTM_ClearInterruptFlag (FTM_t ftm, FTMChannel_t channel)
{
	ftm->CONTROLS[channel].CnSC &= ~FTM_CnSC_CHF_MASK;
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
/****************************
 * HANDLERS DE INTERRUPCIONES
************************/
/**********************************
* INICIALIZADORES
******************/
void CH_Init(FTMModule_t ftm, int channel)
{
	int port;
	int pin;
	switch (ftm)
	{
		case FTM_0:
		{
			port = FTM0_port;
			SIM->SCGC5 |= (1<<(9 + port));
			//SET_BIT(SIM->SCGC5, 9+port);
			switch(channel)
			{
				case 0:
				{
					pin = FTM0_CH0_pin;
					PORTC->PCR[pin] = 0;
					PORTC->PCR[pin] |= PORT_PCR_MUX(4);
					break;
				}
				case 1:
				{
					pin = FTM0_CH1_pin;
					PORTC->PCR[pin] = 0;
					PORTC->PCR[pin] |= PORT_PCR_MUX(4);
					break;
				}
				case 2:
				{
					pin = FTM0_CH2_pin;
					PORTC->PCR[pin] = 0;
					PORTC->PCR[pin] |= PORT_PCR_MUX(4);
					break;
				}
				default: break;
			}
			break;
		}

		case FTM_1:
			break;

		case FTM_2:
			port = FTM2_port;
			SIM->SCGC5 |= (1<<(9+port));
			switch(channel)
			{
				case 0:
				{
					pin = FTM2_CH0_pin;
					PORTB->PCR[pin] = 0;
					PORTB->PCR[pin] |= PORT_PCR_MUX(3);
					break;
				}
				case 1:
				{
					pin = FTM2_CH1_pin;
					PORTB->PCR[pin] = 0;
					PORTB->PCR[pin] |= PORT_PCR_MUX(3);
					break;
				}
				default: break;
			}
			break;

		case FTM_3:
			port = FTM3_port;
			SIM->SCGC5 |= (1<<(9+port));
			switch(channel)
			{
				case 0:
				{
					pin = FTM3_CH0_pin;
					PORTD->PCR[pin] = 0;
					PORTD->PCR[pin] |= PORT_PCR_MUX(4);
					break;
				}
				case 1:
				{
					pin = FTM3_CH1_pin;
					PORTD->PCR[pin] = 0;
					PORTD->PCR[pin] |= PORT_PCR_MUX(4);
					break;
				}
				case 2:
				{
					pin = FTM3_CH2_pin;
					PORTD->PCR[pin] = 0;
					PORTD->PCR[pin] |= PORT_PCR_MUX(4);
					break;
				}
				default: break;
			}
			break;
			
		default:
			break;
	}
}

void PWM_Init (FTM_t ftm, int channel)
{
	//Prescaler: Bus clock (50MHz)/32
	FTM_SetPrescaler(ftm, FTM_PSC_x16);

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

	//  Set up timer for OC interrupt

	//  Enable Timer advanced modes (FTMEN=1)

	ftm->MODE=(ftm->MODE & ~FTM_MODE_FTMEN_MASK) | FTM_MODE_FTMEN(1);

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
	//FTM_SetInterruptMode (ftm, channel, true);
	FTM_StartClock(ftm); //Select BusClk
}

void IC_Init (FTM_t ftm, int channel)
{
	
	//  Enable Timer advanced modes (FTMEN=1)

	ftm->MODE=(ftm->MODE & ~FTM_MODE_FTMEN_MASK) | FTM_MODE_FTMEN(1);

	//--- medidor

	FTM_SetPrescaler(ftm, FTM_PSC_x32);	 				// Set Prescaler = divx32
	FTM3->CNTIN=0x0000;				  		  				// Free running
	FTM3->MOD=0xFFFF;
	FTM_SetWorkingMode(ftm, channel, FTM_mInputCapture);   // Select IC Function
	FTM_SetInputCaptureEdge (ftm, channel, FTM_eEither);  // Capture on both edges
	FTM_SetInterruptMode (ftm, channel, true);            // Enable interrupts
	FTM_StartClock(ftm);                                  // Select BusClk
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
	if(FTM_IsOverflowPending (FTM0))
	{
		FTM_ClearOverflowFlag(FTM0);

		FTM_SetCounter(FTM0, 0, PWM_duty);  //change DC
		//gpioToggle(PORTNUM2PIN(PC, 8));			  //GPIO pin PTC8
		//int a = FTM_GetCounter(FTM0, 0);
		PWM_duty++;
		PWM_duty %= PWM_modulus;

	}
}
