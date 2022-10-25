/***************************************************************************//**
  @file     main.c
  @brief    FW main
  @author   Nicolás Magliola
 ******************************************************************************/
#include <stddef.h>
#include <stdlib.h>
#include "hardware.h"
#include "../../source/FTM.h"

const uint16_t FullSine_5Bit[32] =
{
  2048, 2447, 2831, 3185, 3495, 3750, 3939, 4056,
  4095, 4056, 3939, 3750, 3495, 3185, 2831, 2447,
  2048, 1648, 1264,  910,  600,  345,  156,   39,
     0,   39,  156,  345,  600,  910, 1264, 1648
};

void App_Init (void);
void App_Run (void);


void main (void)
{
  hw_Init();

  static FTMConfig_t config;
  config.modulus = 651;
  config.init_counter = 0;
  config.mode = FTM_mPulseWidthModulation;
  config.prescale = FTM_PSC_x2;
  config.logic = FTM_lAssertedHigh;
  FTM_Init(FTM_0, 0, config);

  PWMGuide_t guide;
  guide.cant = 32;
  guide.indice = 0;
  guide.step = 1;

  static uint16_t arr[32];
  for(int i =0 ; i<32 ; i++)
  {
    arr[i] = ((FullSine_5Bit[i] * config.modulus)/4096);
  }

  guide.duty_arr = &arr[0];

  FTM_SetPWMGuide(FTM_0, 0, guide);

  while(1)
  {

  }
}
