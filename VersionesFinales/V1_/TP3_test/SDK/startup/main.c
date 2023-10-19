/***************************************************************************//**
  @file     main.c
  @brief    FW main
  @author   Nicolás Magliola
 ******************************************************************************/
#define ARM_MATH_CM4 0
#include "hardware.h"

#include "FSK.h"
#include "gpio.h"
#include "stdio.h"
//#include "board.h"
#include "DAC.h"
#include "ADC.h"
#include "uart2fsk.h"
#include "uart_drv.h"
#include "SysTick.h"
#include "bufferCirc.h"

#define BUFFER_APP 10000


void App_Init (void);
void App_Run (void);

circBuffer_t bufferApp;
int16_t Bapp[BUFFER_APP];
char resp =0;
void main (void)
{

     hw_Init();
     hw_DisableInterrupts();
     //App_Init(); /* Program-specific setup */
     hw_EnableInterrupts();
     App_Init();

     bufferApp.d=BUFFER_APP-1;
     bufferApp.inPtr=0;
     bufferApp.outPtr=0;
     bufferApp.buffSize=BUFFER_APP;
     bufferApp.Buffer=Bapp;
     clearBuff(&bufferApp);

     __FOREVER__{
    	 App_Run();
     }

}



void App_Init (void)
{
	  Init_FSK();
	  UART_Init (1200, 2);

}


void App_Run (void)
{
		static int16_t transmitting=-1;
		if (!inputEmpty() ){
			insertData(&bufferApp,retreiveInput());

		}
		if(!isBuffEmpty(&bufferApp)){
			if(transmitting==-1)
				transmitting = getData(&bufferApp);

			if(Tx_Fsk(uart_2_fsk((int8_t)transmitting)))
				transmitting=-1;

		}

		if(isRxDataFull()){
			int16_t resul = getRxData();
			resp=(char)(resul>>1);
			UART_Send_Data(&resp,1);
		}

}

