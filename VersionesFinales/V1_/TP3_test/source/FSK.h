/***************************************************************************//**
  @file     FSK.h
  @brief    Implementacion de Modulador y Demodulador FSK.
  @author   Enrique Irigoyen
 ******************************************************************************/

#ifndef _FSK_H_
#define _FSK_H_

#include "stdint.h"

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/



/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/



/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief TODO: completar descripcion
 * @param param1 Descripcion parametro 1
 * @param param2 Descripcion parametro 2
 * @return Descripcion valor que devuelve
*/

void Init_FSK(void);
int Tx_Fsk(uint16_t data);
uint8_t isRxDataFull(void);
uint16_t getRxData(void);

/*******************************************************************************
 ******************************************************************************/

#endif // _FSK_H_
