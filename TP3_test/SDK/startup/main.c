/***************************************************************************//**
  @file     main.c
  @brief    FW main
  @author   Nicolás Magliola
 ******************************************************************************/

#include "hardware.h"
#include "..\..\source\FSK.h"

void App_Init (void);
void App_Run (void);


void main (void)
{
    // hw_Init();
    // //hw_DisableInterrupts();
    // App_Init(); /* Program-specific setup */
    // //hw_EnableInterrupts();

    // __FOREVER__
    //     App_Run(); /* Program-specific loop  */

  char word[8] = "10110011";
  FSK_t modu;
  modu  = FSK_modulation(word);
  printf("su");
  
}
