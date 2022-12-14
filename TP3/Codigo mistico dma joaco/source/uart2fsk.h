/***************************************************************************//**
  @file     uart_fsk.h
  @brief    Funciones para modular FSK
  @author   Dimas Bosch
 ******************************************************************************/

#ifndef _UART_FSK_H_
#define _UART_FSK_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdint.h>


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/



/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/


/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

// +ej: extern unsigned int anio_actual;+


/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief TODO: Devuelve un 16 bits donde los 11LSB (0000 0SDD DDDD DDPT) 8-Odd-1 con lo datos en LSB first
 * @param datos char recibido por uart
 * @return Estructura FSK_t correspondiente
*/
uint16_t uart_2_fsk(uint8_t datos);


/*******************************************************************************
 ******************************************************************************/

#endif // _TEMPLATE_H_
