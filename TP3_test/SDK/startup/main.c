/***************************************************************************//**
  @file     main.c
  @brief    FW main
  @author   Nicolás Magliola
 ******************************************************************************/
#define ARM_MATH_CM4 0
#include "hardware.h"
#include "uart_fsk.h"
<<<<<<< HEAD
//#include "FSK.h"
#include "gpio.h"
//#include "arm_math.h"
=======
#include "FTM.h"
>>>>>>> master

void App_Init (void);
void App_Run (void);


void main (void)
{
<<<<<<< HEAD
     hw_Init();
     hw_DisableInterrupts();
     //App_Init(); /* Program-specific setup */
     hw_EnableInterrupts();
     Init_FSK();

     //probando multiplicaciones de floats
     //gpioMode(PORTNUM2PIN(PC,10),OUTPUT);
     float result=0;
     float a=1.7;
     float b=39.6;
     int i=0;
     __FOREVER__{


     }



=======
    // hw_Init();
    // //hw_DisableInterrupts();
    // App_Init(); /* Program-specific setup */
    // //hw_EnableInterrupts();

    // __FOREVER__
    //     App_Run(); /* Program-specific loop  */
    hw_Init ();
    FTM_Init(FTM_0, FTM_mPulseWidthModulation, 0);

  /*uint8_t word = 0x11;
  uint16_t modu = uart_2_fsk(word);

  for(int i = 0; i<11 ; i++)
  {
	  if((modu) & (1<<(10-i)))
	  {
		  printf("1");
	  }
	  else
	  {
		  printf("0");
	  }
  }*/
  while(1)
  {
	  //printf("\n");
  }
>>>>>>> master
}

