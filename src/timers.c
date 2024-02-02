/*
 * timers.c
 *
 *  Created on: Jan 30, 2024
 *      Author: malolasimman.s.k
 */

#include "timers.h"
#include "oscillators.h"
#include "em_letimer.h"
#include "em_cmu.h"

#define INCLUDE_LOG_DEBUG 1
#include "src/log.h"
void initLETIMER0()
{

  uint32_t tmp=0;

  const LETIMER_Init_TypeDef letimerInitData =
  {
      false,              // enable; don't enable when init completes, we'll enable last
      true,               // debugRun; useful to have the timer running when single-stepping in the debugger
      true,               // comp0Top; load COMP0 into CNT on underflow
      false,              // bufTop; don't load COMP1 into COMP0 when REP0==0
      0,                  // out0Pol; 0 default output pin value
      0,                  // out1Pol; 0 default output pin value
      letimerUFOANone,    // ufoa0; no underflow output action
      letimerUFOANone,    // ufoa1; no underflow output action
      letimerRepeatFree,  // repMode; free running mode i.e. load & go forever
      0                   // COMP0(top) Value, I calculate this below
  };

  // timer init
  LETIMER_Init (LETIMER0, &letimerInitData);

  // calculate and load COMP0 (top)
  uint32_t topValue = (LETIMER_PERIOD_MS * CMU_ClockFreqGet(cmuClock_LETIMER0)) / 1000;
  LETIMER_CompareSet(LETIMER0, 0, topValue);
//  LOG_INFO("topValue %d",topValue);
//  // calculate and load COMP1
  uint32_t ON_timeVal = (LETIMER_ON_TIME_MS * CMU_ClockFreqGet(cmuClock_LETIMER0)) / 1000;
  LOG_INFO("ON_timeVal %d",ON_timeVal);
//  LETIMER_CompareSet(LETIMER0, 1, ON_timeVal);

  // Clear all IRQ flags in the LETIMER0 IF status register
  LETIMER_IntClear (LETIMER0, 0xFFFFFFFF);

  // Set UF and COMP1 in LETIMER0_IEN, so that the timer will generate IRQs to the NVIC.
  tmp = LETIMER_IEN_UF | LETIMER_IEN_COMP1;
  LETIMER_IntEnable (LETIMER0, tmp); // Make sure you have defined the ISR routine LETIMER0_IRQHandler()

  // Enable the timer to starting counting down, set LETIMER0_CMD[START] bit, see LETIMER0_STATUS[RUNNING] bit
  LETIMER_Enable (LETIMER0, true);


}
