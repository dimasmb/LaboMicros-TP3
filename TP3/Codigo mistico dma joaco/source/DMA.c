/***************************************************************************//**
  @file     +Nombre del archivo (ej: template.c)+
  @brief    +Descripcion del archivo+
  @author   +Nombre del autor (ej: Salvador Allende)+
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "DMA.h"
#include "MK64F12.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/



/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/



/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

// +ej: unsigned int anio_actual;+


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

// +ej: static void falta_envido (int);+


/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

// +ej: static const int temperaturas_medias[4] = {23, 26, 24, 29};+


/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

// +ej: static int temperaturas_actuales[4];+


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
void DMA_init(uint8_t channel, DMAConfig_t config)
{
  /* Enable the clock for the eDMA and the DMAMUX. */
	SIM->SCGC7 |= SIM_SCGC7_DMA_MASK;
	SIM->SCGC6 |= SIM_SCGC6_DMAMUX_MASK;

	/* Enable the eDMA channel 0 and set the FTM CH0 as the DMA request source. */
	DMAMUX->CHCFG[channel] |= DMAMUX_CHCFG_ENBL_MASK | DMAMUX_CHCFG_SOURCE(config.sourceChannel);   // FTM0 CH0

	/* Enable the interrupts for the channel 0. */

	/* Clear all the pending events. */
	//NVIC_ClearPendingIRQ(DMA0_IRQn);
	/* Enable the DMA interrupts. */
	 //NVIC_EnableIRQ(DMA0_IRQn);

	/// ============= INIT TCD0 ===================//
	/* Set memory address for source and destination. */
	DMA0->TCD[channel].SADDR = (uint32_t)(config.sourceAddr);	//List of Duties

    //DMA_TCD0_DADDR = (uint32_t)(destinationBuffer);
	DMA0->TCD[channel].DADDR = (uint32_t)(config.destAddr);  // To change FTM Duty


		/* Set an offset for source and destination address. */
	DMA0->TCD[channel].SOFF = 0x02;//config.sourceOff; // Source address offset of 2 bytes per transaction.
	DMA0->TCD[channel].DOFF = config.destOff; // Destination address offset is 0. (Siempre al mismo lugar)

	/* Set source and destination data transfer size to 16 bits (CnV is 2 bytes wide). */
	DMA0->TCD[channel].ATTR = DMA_ATTR_SSIZE(1) | DMA_ATTR_DSIZE(1);

	/*Number of bytes to be transfered in each service request of the channel.*/
	DMA0->TCD[channel].NBYTES_MLNO = 0x02;

	/* Autosize for Current major iteration count */
	DMA0->TCD[channel].CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(config.cant);
	DMA0->TCD[channel].BITER_ELINKNO = DMA_BITER_ELINKNO_BITER(config.cant);

	/* Autosize SLAST for Wrap Around. This value is added to SADD at the end of Major Loop */
	DMA0->TCD[channel].SLAST = -(config.cant * (sizeof(uint16_t)));


	/* DLASTSGA DLAST Scatter and Gatter */
	DMA0->TCD[channel].DLAST_SGA = 0x00;

	/* Setup control and status register. */

	//DMA0->TCD[0].CSR = DMA_CSR_INTMAJOR_MASK;	//Enable Major Interrupt.
	//DMA0->TCD[channel].CSR |= DMA_CSR_INTMAJOR_MASK;

	/* Enable request signal for channel 0. */
	switch(channel)
		{
			case 0:
				DMA0->ERQ = DMA_ERQ_ERQ0_MASK;
				break;
			case 1:
				DMA0->ERQ = DMA_ERQ_ERQ1_MASK;
				break;
		}

	//hw_EnableInterrupts();
	//DMA0->TCD[channel].CSR |= DMA_CSR_START_MASK;
}

void Enable_MajorInt(uint8_t channel)
{
  /* Setup control and status register. */
	DMA0->TCD[channel].CSR |= DMA_CSR_INTMAJOR_MASK;	//Enable Major Interrupt.
}
/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void SetSourceAddr(uint8_t channel, uint32_t address)
{
  DMA0->TCD[channel].SADDR = (uint32_t)(address);
}

/*
void DMA_Error_IRQHandler(void){
    DMA0->CERR = 0;
}

void DMA0_IRQHandler(void){

}*/
 
