/***************************************************************************//**
  @file     +Nombre del archivo (ej: template.c)+
  @brief    +Descripcion del archivo+
  @author   +Nombre del autor (ej: Salvador Allende)+
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "PWM.h"
#include "FTM.h"
#include "DMA.h"
#include "MK64F12.h"
// +Incluir el header propio (ej: #include "template.h")+


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define FTM_PWM_CHANNEL 0
#define DMA_CHANNEL 1
#define CANT_MUESTRAS 120

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/



/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

// +ej: unsigned int anio_actual;+


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

// +ej: static void falta_envido (int);+


/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
const uint16_t SpaceRef[CANT_MUESTRAS] = {2048, 2262, 2473, 2680, 2880, 3072, 3251, 3418, 3569, 3704, 
                                3821, 3918, 3995, 4051, 4084, 4096, 4084, 4051, 3995, 3918, 
                                3821, 3704, 3569, 3418, 3251, 3072, 2880, 2680, 2473, 2262, 
                                2048, 1833, 1622, 1415, 1215, 1023, 844, 677, 526, 391, 274, 
                                177, 100, 44, 11, 0, 11, 44, 100, 177, 274, 391, 526, 677, 844, 
                                1024, 1215, 1415, 1622, 1833, 2047, 2262, 2473, 2680, 2880, 3072, 
                                3251, 3418, 3569, 3704, 3821, 3918, 3995, 4051, 4084, 4096, 4084, 
                                4051, 3995, 3918, 3821, 3704, 3569, 3418, 3251, 3071, 2880, 2680, 
                                2473, 2262, 2048, 1833, 1622, 1415, 1215, 1024, 844, 677, 526, 
                                391, 274, 177, 100, 44, 11, 0, 11, 44, 100, 177, 274, 391, 526, 
                                677, 844, 1024, 1215, 1415, 1622, 1833};

const uint16_t MarkRef[CANT_MUESTRAS]={2048, 2155, 2262, 2368, 2473, 2578, 2680, 2781, 2880, 2977, 3072,
                             3163, 3251, 3336, 3418, 3496, 3569, 3639, 3704, 3765, 3821, 3872, 
                             3918, 3959, 3995, 4026, 4051, 4070, 4084, 4093, 4096, 4093, 4084, 
                             4070, 4051, 4026, 3995, 3959, 3918, 3872, 3821, 3765, 3704, 3639, 
                             3569, 3496, 3418, 3336, 3251, 3163, 3072, 2977, 2880, 2781, 2680, 
                             2578, 2473, 2368, 2262, 2155, 2048, 1940, 1833, 1727, 1622, 1517, 
                             1415, 1314, 1215, 1118, 1023, 932, 844, 759, 677, 599, 526, 456, 
                             391, 330, 274, 223, 177, 136, 100, 69, 44, 25, 11, 2, 0, 2, 11, 
                             25, 44, 69, 100, 136, 177, 223, 274, 330, 391, 456, 526, 599, 677, 
                             759, 844, 932, 1024, 1118, 1215, 1314, 1415, 1517, 1622, 1727, 1833, 1940};

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
static uint16_t mark[CANT_MUESTRAS];
static uint16_t space[CANT_MUESTRAS];

static uint16_t* curr_state;
/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
void PWM_1200_Init(void)
{
  static FTMConfig_t config;
  static DMAConfig_t DMAconfig;
  config.modulus = 347;
  config.init_counter = 0;
  config.mode = FTM_mPulseWidthModulation;
  config.prescale = FTM_PSC_x1;
  config.logic = FTM_lAssertedHigh;
  FTM_Init(FTM_0, FTM_PWM_CHANNEL, config);

  for(int i =0 ; i<CANT_MUESTRAS ; i++)
  {
    mark[i] = ((MarkRef[i] * config.modulus)/4096);
    if(mark[i] == 0) mark[i] = 1;
    space[i] = ((SpaceRef[i] * config.modulus)/4096);
    if(space[i] == 0) space[i] = 1;
  }

  //Set DMA
  DMAconfig.sourceAddr = (uint32_t)(&mark);
  curr_state = mark;
  DMAconfig.destAddr = (uint32_t)(&(FTM0->CONTROLS[FTM_PWM_CHANNEL].CnV));
  DMAconfig.cant = CANT_MUESTRAS;
  DMAconfig.sourceChannel = sFTM0_CH0;
  DMAconfig.sourceOff = 0x02;
  DMAconfig.destOff = 0x00;
  FTM_DmaMode(FTM0, FTM_CH_0, true);
  FTM_SetInterruptMode(FTM0, FTM_CH_0, true);
  DMA_Init(DMA_CHANNEL, DMAconfig);
}

void SetMarkSpace(bool ms)
{
  if(ms)
  {
    SetSourceAddr(DMA_CHANNEL, (uint32_t)(&mark));
  }
  else SetSourceAddr(DMA_CHANNEL, (uint32_t)(&space));
}

bool IsPeriodDone(void)
{
	if(DMA0->TCD[DMA_CHANNEL].CSR & DMA_CSR_DONE_MASK)
	{
		DMA0->TCD[DMA_CHANNEL].CSR &= ~DMA_CSR_DONE_MASK;
		return true;
	}
	else return false;
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/



 
