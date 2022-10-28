/***************************************************************************//**
  @file     App.c
  @brief    Applicacion sobre la modulacion FSK, con manejo de los perifericos ADC y DAC
  @author   Salta
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/



#include "uart2fsk.h"
#include "uart_drv.h"
#include "buffer.h"
#include "PWM.h"
#include <stdint.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

void process_uart();

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/* Función que se llama 1 vez, al comienzo del programa */
void App_Init (void)
{
	UART_Init (1200, 2);
	PWM_1200_Init();
}

/* Función que se llama constantemente en un ciclo infinito */


void App_Run (void)
{
	process_uart();
	static int first_pass=1;
	if (!first_pass){
		if(IsPeriodDone()){
			if (bufferEmpty()){
				SetMarkSpace(true); // si no tengo nada en el buffer de UART mando idle
			}
			else{

				SetMarkSpace(getChar());
			}

		}
	}
	else{
		SetMarkSpace(true);
		first_pass = 0;

	}


}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


/*******************************************************************************
 ******************************************************************************/

void process_uart(){
	if (!inputEmpty()){
			char newch = retreiveInput();
			uint16_t message=uart_2_fsk(newch);

			message = message<<5;
			for(int e=0; e<11; e++){
				char newbit;
				newbit = (((message<<e) & 0x8000)>>15);
				insertChar(newbit);
			}
	}
}

