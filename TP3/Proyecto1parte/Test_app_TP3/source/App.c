/***************************************************************************//**
  @file     App.c
  @brief    Applicacion sobre la modulacion FSK, con manejo de los perifericos ADC y DAC
  @author   Salta
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/


#include "board.h"
#include "gpio.h"
#include <stdio.h>  /*borrar despues*/
#include "DAC.h"
#include "ADC.h"
#include "uart2fsk.h"
#include "uart_drv.h"
#include "FSK.h"
#include "SysTick.h"



/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
static void delayLoop(uint32_t veces);

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
adc_t adc;
/* Función que se llama 1 vez, al comienzo del programa */
void App_Init (void)
{
	adc.modo=1;
	adc.e_interrup=false;
	adc.k=0;
	adc.clk=0;
	Init_FSK();	//ahora solo
	//DAC_Init();
	ADC_Init(adc);
	UART_Init (1200, 2);


}
uint8_t array[1000];

/* Función que se llama constantemente en un ciclo infinito */
void App_Run (void)
{
	static char newch[1000];
	static int i = 0;
		if (!inputEmpty()){
			newch[i] = retreiveInput();
			//UART_Send_Data(&newch, 1);
			uint16_t prueba=uart_2_fsk(newch[i]);
			Tx_Fsk(prueba);
			i++;
		}
	int juan=0;
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static void delayLoop(uint32_t veces)
{
    while (veces--);
}

/*******************************************************************************
 ******************************************************************************/


