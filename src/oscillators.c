/*
 *  File Name: oscillators.c
 *  Description: This code involves initializing the clock for low energy timer0 for EFR32BG13
 *  Created on: Jan 30, 2024
 *  Author: Malola Simman Srinivasan Kannan, Malola.Srinivasankannan@Colorado.edu
 *  @student: Malola Simman Srinivasan Kannan, Malola.Srinivasankannan@Colorado.edu
 *  Reference: Class lecture note 4
 */

#include "oscillators.h"
#include "em_cmu.h"
#include "app.h"

void cmu_init()
{
  if(LOWEST_ENERGY_MODE == EM0 || LOWEST_ENERGY_MODE == EM1 || LOWEST_ENERGY_MODE == EM2){
      CMU_OscillatorEnable(cmuOsc_LFXO, true, true);
      CMU_ClockSelectSet(cmuClock_LFA,cmuSelect_LFXO);
      CMU_ClockDivSet(cmuClock_LETIMER0, cmuClkDiv_4);
      CMU_ClockEnable(cmuClock_LETIMER0,true);
  }
  if(LOWEST_ENERGY_MODE == EM3){
      CMU_OscillatorEnable(cmuOsc_ULFRCO, true, true);
      CMU_ClockSelectSet(cmuClock_LFA,cmuSelect_ULFRCO);
      CMU_ClockDivSet(cmuClock_LETIMER0, cmuClkDiv_1);
      CMU_ClockEnable(cmuClock_LETIMER0,true);

  }

}
