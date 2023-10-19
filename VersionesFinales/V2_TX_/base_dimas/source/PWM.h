/***************************************************************************//**
  @file     +Nombre del archivo (ej: template.h)+
  @brief    +Descripcion del archivo+
  @author   +Nombre del autor (ej: Salvador Allende)+
 ******************************************************************************/

#ifndef _PWM_H_
#define _PWM_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdbool.h>


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/



/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef enum
{
  Mult_x1 = 1,
  Mult_x2 = 2,
  Mult_x4 = 4
} FrecMult_t;


/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

// +ej: extern unsigned int anio_actual;+


/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief TODO: genera una PWM para generar una senoidal de 1200Hz por FTM0_CH0
*/
void PWM_1200_Init(void);


void SetMarkSpace(bool ms);

/**
 * @brief TODO: genera una PWM para generar una senoidal de 1200Hz por FTM0_CH0
 * @return true si está en la última muestra del ciclo
*/
bool IsPeriodDone(void);
/*******************************************************************************
 ******************************************************************************/

#endif // _TEMPLATE_H_
