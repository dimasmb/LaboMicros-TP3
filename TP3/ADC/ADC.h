/***************************************************************************//**
  @file     Template.h
  @brief    Drive para el manejo del ADC.h
  @author   Salta
 ******************************************************************************/

#ifndef _ADC_H_
#define _ADC_H_


/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "hardware.h"
#include "MK64F12.h"
#include <stdbool.h>
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/




/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef enum
{
	ADC_b8s,
	ADC_b12s,
	ADC_b10s,
	ADC_b16s,
} modo_t;
typedef enum
{
	bus_clk,
	bus_clkd2,
	alt_clk,
	asyn_clk,
} clk_Source_t; 
typedef enum
{
	clk_1,
	clk_2,
	clk_4,
	clk_8,
} clk_divide_t; 

typedef struct
{
    modo_t modo;    //modo de uso, es necesario activar el diff tmb
    clk_Source_t clk;
    clk_divide_t k ;
    bool e_interrup;
}adc_t ;

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Se inicializa las configuracines basicas del ADC1.
 * @adc se le pasan a traves de adc, los paramentros mas importantes ,como el modo ,source clock, etc.
 */
void ADC_Init(adc_t adc);

/**
 * @brief Permite obtener lo datos que cuantificados por el adc.
 */
uint16_t ADC_Data(void);

/**
 * @brief Se usa para la conversion continua, en la que captura un nuevo valor cada vez q actualizamos  SC1.
 */
void Start_conv(void);

/**
 * @brief Verifica el flag coco de SC1, con el fin de saber el estado de la conversion.
 * @return Devuelve 1 si la conversion esta completa, y esta libre para una nueva.
 */
bool Conver_Complete(void);

/**
 * @brief es similar a ADC_Data, pero cuando llamamos a ésta, nos ahorramos llamar Start_Conv
 * @return Devuelve la conversion
 */
uint16_t ADC_Read(void);

void Clear_interrup_flag(void);
/*******************************************************************************
 ******************************************************************************/

#endif // _ADC_H_
