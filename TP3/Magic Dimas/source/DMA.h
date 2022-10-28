/***************************************************************************//**
  @file     +Nombre del archivo (ej: template.h)+
  @brief    +Descripcion del archivo+
  @author   +Nombre del autor (ej: Salvador Allende)+
 ******************************************************************************/

#ifndef _DMA_H_
#define _DMA_H_

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
typedef enum
{
  sFTM0_CH0 = 20,
  sFTM0_CH1,
  sFTM0_CH2,
  sFTM0_CH3,
  sFTM0_CH4,
  sFTM0_CH5,
  sFTM0_CH6,
  sFTM0_CH7,

  sFTM1_CH0,
  sFTM1_CH1,  
  
  sFTM2_CH0,
  sFTM2_CH1,

  sFTM3_CH0,
  sFTM3_CH1,
  sFTM3_CH2,
  sFTM3_CH3,
  sFTM3_CH4,
  sFTM3_CH5,
  sFTM3_CH6,
  sFTM3_CH7 
} DMASource_t;

typedef struct
{
  DMASource_t sourceChannel;
  uint32_t sourceAddr;
  uint16_t cant;  //tamaño del arreglo de source
  uint32_t destAddr;
  uint16_t sourceOff;
  uint16_t destOff;
} DMAConfig_t;

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

// +ej: extern unsigned int anio_actual;+


/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief TODO: DMA con tamaño de 16 bits
 * @param channel DMA channel
 * @param config parametros de configuración
 * @return Descripcion valor que devuelve
*/
void DMA_Init(uint8_t channel, DMAConfig_t config);

void SetSourceAddr(uint8_t channel, uint32_t address);
/*******************************************************************************
 ******************************************************************************/

#endif // _TEMPLATE_H_
