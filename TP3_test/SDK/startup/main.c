/***************************************************************************//**
  @file     main.c
  @brief    FW main
  @author   Nicolás Magliola
 ******************************************************************************/
#define ARM_MATH_CM4 0
#include "hardware.h"
#include "uart_fsk.h"
//#include "FSK.h"
#include "gpio.h"
//#include "arm_math.h"



void App_Init (void);
void App_Run (void);


void main (void)
{

     hw_Init();
     hw_DisableInterrupts();
     //App_Init(); /* Program-specific setup */
     hw_EnableInterrupts();
     Init_FSK();

     //probando multiplicaciones de floats
     //gpioMode(PORTNUM2PIN(PC,10),OUTPUT);

     __FOREVER__{


     }

}

