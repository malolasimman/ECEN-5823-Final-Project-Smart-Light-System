/*
 * oscillators.c
 *
 *  Created on: Jan 30, 2024
 *      Author: malolasimman.s.k
 */

#include "oscillators.h"
// Include logging specifically for this .c file
#define INCLUDE_LOG_DEBUG 1
#include "src/log.h"

#include "em_cmu.h"
#include "app.h"

void cmu_init()
{
  if(LOWEST_ENERGY_MODE == EM0 || LOWEST_ENERGY_MODE == EM1 || LOWEST_ENERGY_MODE == EM2){
      CMU_LFXOInit_TypeDef lfxoInit = CMU_LFXOINIT_DEFAULT;
      CMU_LFXOInit(&lfxoInit);

      CMU_OscillatorEnable(cmuOsc_LFXO, true, true);
      CMU_ClockSelectSet(cmuClock_LFA,cmuSelect_LFXO);
      CMU_ClockDivSet(cmuClock_LETIMER0, cmuClkDiv_4);
      CMU_ClockEnable(cmuClock_LETIMER0,true);

        CMU_Select_TypeDef clksel = CMU_ClockSelectGet(cmuClock_LFA);
        CMU_ClkDiv_TypeDef clkdiv = CMU_ClockDivGet(cmuClock_LETIMER0);
        uint32_t lfaclkfreq = CMU_ClockFreqGet(cmuClock_LFA);
        uint32_t timclkfreq = CMU_ClockFreqGet(cmuClock_LETIMER0);
  }
  if(LOWEST_ENERGY_MODE == EM3){
      CMU_LFXOInit_TypeDef lfxoInit = CMU_LFXOINIT_DEFAULT;
      CMU_LFXOInit(&lfxoInit);

      CMU_OscillatorEnable(cmuOsc_ULFRCO, true, true);
      CMU_ClockSelectSet(cmuClock_LFA,cmuSelect_ULFRCO);
      CMU_ClockDivSet(cmuClock_LETIMER0, cmuClkDiv_1);
      CMU_ClockEnable(cmuClock_LETIMER0,true);

      CMU_Select_TypeDef clksel = CMU_ClockSelectGet(cmuClock_LFA);
      CMU_ClkDiv_TypeDef clkdiv = CMU_ClockDivGet(cmuClock_LETIMER0);
      uint32_t lfaclkfreq = CMU_ClockFreqGet(cmuClock_LFA);
      uint32_t timclkfreq = CMU_ClockFreqGet(cmuClock_LETIMER0);
//      LOG_INFO("cmuClock_LETIMER0 %d",timclkfreq);

  }

}
