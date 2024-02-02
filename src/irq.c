/*
 * irq.c
 *
 *  Created on: Jan 31, 2024
 *      Author: malolasimman.s.k
 */
#include "irq.h"
#include "em_letimer.h"
#include "gpio.h"
#include "em_core.h"


 void LETIMER0_IRQHandler (void){

  CORE_DECLARE_IRQ_STATE;

  // NVIC IRQs are disabled
  CORE_ENTER_CRITICAL();

  // First: determine source of IRQ
  uint32_t flags = LETIMER_IntGetEnabled(LETIMER0);

  // Second: clear source of IRQ set in step 3
  LETIMER_IntClear(LETIMER0,0xFFFFFFFF);

  // Third: perform whatever processing is required
  if(flags & (LETIMER_IF_COMP1)){
      gpioLed0SetOff();
      gpioLed1SetOff();
  }

  if(flags & (LETIMER_IF_UF)){
      gpioLed0SetOn();
      gpioLed1SetOn();
  }

  // NVIC IRQs are re-enabled
  CORE_EXIT_CRITICAL();

 }//LETIMER0_IRQHandler
