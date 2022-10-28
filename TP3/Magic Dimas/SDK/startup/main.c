/***************************************************************************//**
  @file     main.c
  @brief    FW main
  @author   Nicolás Magliola
 ******************************************************************************/
#include <stddef.h>
#include <stdlib.h>
#include "hardware.h"
#include "../../source/FTM.h"
#include "PWM.h"
// const uint16_t FullSine_5Bit[32] =
// {
//   2048, 2447, 2831, 3185, 3495, 3750, 3939, 4056,
//   4095, 4056, 3939, 3750, 3495, 3185, 2831, 2447,
//   2048, 1648, 1264,  910,  600,  345,  156,   39,
//      0,   39,  156,  345,  600,  910, 1264, 1648
// };

void App_Init (void);
void App_Run (void);


void main (void)
{
  hw_Init();
  PWM_1200_Init();
  //SetFrecMult(Mult_x2);
  bool flag = true;
  while(1)
  {
    if(IsPeriodDone())
    {
      SetMarkSpace(flag);
      flag = !flag;
    }
  }
}
