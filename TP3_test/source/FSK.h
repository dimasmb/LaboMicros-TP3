/***************************************************************************//**
  @file     FSK.h
  @brief    Funciones para modular FSK
  @author   Dimas Bosch
 ******************************************************************************/

#ifndef _FSK_H_
#define _FSK_H_

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
typedef struct
{
  uint16_t stop     :1;
  uint16_t parity   :1;
  uint16_t data     :8;
  uint16_t start    :1;
  uint16_t none     :5;
}FSK_t;



/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

// +ej: extern unsigned int anio_actual;+


/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief TODO: arma bitfield con start, datos (8 bits), parity bit (odd) y stop acorde con la modulacion FSK
 * @param uart_str string de 8 chars recibido por uart
 * @return Estructura FSK_t correspondiente
*/
FSK_t FSK_modulation(char* uart_str);


/*******************************************************************************
 ******************************************************************************/

#endif // _TEMPLATE_H_
