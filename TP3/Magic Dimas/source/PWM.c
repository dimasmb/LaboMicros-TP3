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

const uint16_t Mark_5Bit[32] =
{
  2048, 2447, 2831, 3185, 3495, 3750, 3939, 4056,
  4095, 4056, 3939, 3750, 3495, 3185, 2831, 2447,
  2048, 1648, 1264,  910,  600,  345,  156,   39,
  0,   39,  156,  345,  600,  910, 1264, 1648
};

const uint16_t Space_5Bit[32] =
{
  2048, 2831, 3495, 3939,
  4095, 3939, 3495, 2831,
  2048, 1264,  600,  156,
  30,  156,  600, 1264,
  2048, 2831, 3495, 3939,
  4095, 3939, 3495, 2831,
  2048, 1264,  600,  156,
  30,  156,  600, 1264
};

const uint16_t SpaceRef[6][10] = {{2048, 2473, 2880, 3251, 3569, 3821, 3995, 4084, 4084, 3995},
{3821, 3569, 3251, 2880, 2473, 2048, 1622, 1215, 844, 526},
{274, 100, 11, 11, 100, 274, 526, 844, 1215, 1622},
{2047, 2473, 2880, 3251, 3569, 3821, 3995, 4084, 4084, 3995},
{3821, 3569, 3251, 2880, 2473, 2048, 1622, 1215, 844, 526},
{274, 100, 11, 11, 100, 274, 526, 844, 1215, 1622}};

const uint16_t MarkRef[6][10]={{2048, 2262, 2473, 2680, 2880, 3072, 3251, 3418, 3569, 3704},
{3821, 3918, 3995, 4051, 4084, 4096, 4084, 4051, 3995, 3918},
{3821, 3704, 3569, 3418, 3251, 3072, 2880, 2680, 2473, 2262},
{2048, 1833, 1622, 1415, 1215, 1023, 844, 677, 526, 391},
{274, 177, 100, 44, 11, 11, 11, 44, 100, 177},
{274, 391, 526, 677, 844, 1024, 1215, 1415, 1622, 1833}};

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
static uint16_t mark1[10];
static uint16_t space1[10];
static uint16_t mark2[10];
static uint16_t space2[10];
static uint16_t mark3[10];
static uint16_t space3[10];
static uint16_t mark4[10];
static uint16_t space4[10];
static uint16_t mark5[10];
static uint16_t space5[10];
static uint16_t mark6[10];
static uint16_t space6[10];

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
  config.modulus = 695;
  config.init_counter = 0;
  config.mode = FTM_mPulseWidthModulation;
  config.prescale = FTM_PSC_x1;
  config.logic = FTM_lAssertedHigh;

  FTM_Init(FTM_0, FTM_PWM_CHANNEL, config);

  // PWMGuide_t guide;
  // guide.cant = 32;
  // guide.indice = 0;
  // guide.step = 1;

  //static uint16_t arr[32];
  for(int i =0 ; i<10 ; i++)
  {
    mark1[i] = ((MarkRef[0][i] * config.modulus)/4096);
    space1[i] = ((SpaceRef[0][i] * config.modulus)/4096);
    mark2[i] = ((MarkRef[1][i] * config.modulus)/4096);
    space2[i] = ((SpaceRef[1][i] * config.modulus)/4096);
    mark3[i] = ((MarkRef[2][i] * config.modulus)/4096);
    space3[i] = ((SpaceRef[2][i] * config.modulus)/4096);
    mark4[i] = ((MarkRef[3][i] * config.modulus)/4096);
    space4[i] = ((SpaceRef[3][i] * config.modulus)/4096);
    mark5[i] = ((MarkRef[4][i] * config.modulus)/4096);
    space5[i] = ((SpaceRef[4][i] * config.modulus)/4096);
    mark6[i] = ((MarkRef[5][i] * config.modulus)/4096);
    space6[i] = ((SpaceRef[5][i] * config.modulus)/4096);
  }
  //guide.duty_arr = &arr[0];

  //FTM_SetPWMGuide(FTM_0, 0, guide);

  //Set DMA
  DMAconfig.sourceAddr = (uint32_t)(&mark1);
  curr_state = mark1;
  DMAconfig.destAddr = (uint32_t)(&(FTM0->CONTROLS[FTM_PWM_CHANNEL].CnV));
  DMAconfig.cant = 10;
  DMAconfig.sourceChannel = sFTM0_CH0;
  DMAconfig.sourceOff = 0x02;
  DMAconfig.destOff = 0x00;
  FTM_DmaMode(FTM0, FTM_CH_0, true);
  FTM_SetInterruptMode(FTM0, FTM_CH_0, true);
  DMA_Init(DMA_CHANNEL, DMAconfig);
}

void SetFrecMult(FrecMult_t step)
{
  FTM_SetPWMGuideStep(FTM_0, FTM_CH_0, step);
}




void SetMarkSpace(bool mark)
{

  if(mark){
    if(curr_state == 0){
        curr_state = mark1;
    }
    else if (curr_state == mark1) {
        curr_state = mark2;
    }
    else if (curr_state == mark2) {
        curr_state = mark3;
    }
        else if (curr_state == mark3) {
        curr_state = mark4;
    }
        else if (curr_state == mark4) {
        curr_state = mark5;
    }
        else if (curr_state == mark5) {
        curr_state = mark6;
    }
    else{
        curr_state =0;
    }
  }
  else {
    if(curr_state == 0){
        curr_state = space1;
    }
    else if (curr_state == space1) {
        curr_state = space2;
    }
    else if (curr_state == space2) {
        curr_state = space3;
    }
        else if (curr_state == space3) {
        curr_state = space4;
    }
        else if (curr_state == space4) {
        curr_state = space5;
    }
        else if (curr_state == space5) {
        curr_state = space6;
    }
    else{
        curr_state =0;
    }
  }
  if (curr_state != 0){
    SetSourceAddr(DMA_CHANNEL, (uint32_t)(curr_state));
  }
  


}

bool IsPeriodDone(void)
{
  return (DMA0->TCD[DMA_CHANNEL].CSR & DMA_CSR_DONE_MASK);
}
/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/



 
