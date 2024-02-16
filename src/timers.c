/***************************************************************************//**
 *
 * @file timers.c
 * @brief This code involves initializing the Low energy timer0 for EFR32BG13
 * Created on: Jan 30, 2024
 * Author: Malola Simman Srinivasan Kannan, Malola.Srinivasankannan@Colorado.edu
 * @student: Malola Simman Srinivasan Kannan, Malola.Srinivasankannan@Colorado.edu
 * Reference: Class lecture note 5
 *
 ******************************************************************************/


/**************************************************************************//**
 * Headers
 *****************************************************************************/
#include "timers.h"
#include "oscillators.h"
#include "em_cmu.h"

/**************************************************************************//**
 * Low Energy Timer Initialization function
 *****************************************************************************/
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
  uint32_t topValue = (LETIMER_PERIOD_MS * CMU_ClockFreqGet(cmuClock_LETIMER0)) / SEC_MS; // 3sec
  LETIMER_CompareSet(LETIMER0, 0, topValue);

  // Clear all IRQ flags in the LETIMER0 IF status register
  LETIMER_IntClear (LETIMER0, 0xFFFFFFFF);

  // Set UF in LETIMER0_IEN, so that the timer will generate IRQs to the NVIC.
  tmp = LETIMER_IEN_UF;
  LETIMER_IntEnable (LETIMER0, tmp); // Make sure you have defined the ISR routine LETIMER0_IRQHandler()

  // Enable the timer to starting counting down, set LETIMER0_CMD[START] bit, see LETIMER0_STATUS[RUNNING] bit
  LETIMER_Enable (LETIMER0, true);


}

/**************************************************************************//**
 * timerWaitUs polled function
 *****************************************************************************/
void timerWaitUs(uint32_t us_wait)
{
  uint32_t delta =0;
  uint32_t topValue = LETIMER_TopGet(LETIMER0); // topValue is Configured for 3sec
  uint32_t ticks = (CMU_ClockFreqGet(cmuClock_LETIMER0) * us_wait)/SEC_US; // converts us_wait to ticks

  if ((ticks > UINT16_MAX) || (ticks > topValue)) // range check
  {
      return;
  }

  uint32_t curr_ticks = LETIMER_CounterGet(LETIMER0);// get current tick

  // CASE1 : If the required ticks is less than topValue and less than current tick
  if (curr_ticks >= ticks)
  {
    delta = curr_ticks - ticks;
    while (LETIMER_CounterGet(LETIMER0) > delta);
  }
  else // CASE2 : If the required ticks is less than topValue and greater than current tick
  {
    uint32_t rem_ticks = ticks - curr_ticks;
    while (LETIMER_CounterGet(LETIMER0) <= curr_ticks); // run loop for current ticks to required ticks

    delta = topValue - rem_ticks;
    while (LETIMER_CounterGet(LETIMER0) > delta); // run loop for remaining ticks

  }

}

/**************************************************************************//**
 * timerWaitUs interrupt function
 *****************************************************************************/
void timerWaitUs_irq(uint32_t us_wait)
{

  LETIMER_IntClear (LETIMER0, LETIMER_IEN_COMP1); // clear interrupts

  uint32_t topValue = LETIMER_TopGet(LETIMER0); // topValue is Configured for 3sec

  uint32_t ON_timeVal = (us_wait * CMU_ClockFreqGet(cmuClock_LETIMER0)) / SEC_US;// converts us_wait to ticks


  if ((ON_timeVal > UINT16_MAX) || (ON_timeVal > topValue)) // range check
  {
      return;
  }

  uint32_t Curr_count = LETIMER_CounterGet(LETIMER0);// get current tick

  // CASE1 : If the required ticks is less than topValue and less than current tick
  if (Curr_count >= ON_timeVal) // 3 2
  {
      ON_timeVal = Curr_count - ON_timeVal; //3-2 =1
      LETIMER_CompareSet(LETIMER0, 1, ON_timeVal);
  }
  else{// CASE2 : If the required ticks is less than topValue and greater than current tick
      uint32_t rem_ticks = ON_timeVal - Curr_count;
      ON_timeVal = topValue - rem_ticks + 1;
      LETIMER_CompareSet(LETIMER0, 1, ON_timeVal);

  }

  // Enable the timer to starting counting down, set LETIMER0_CMD[START] bit, see LETIMER0_STATUS[RUNNING] bit
  LETIMER_IntEnable (LETIMER0, LETIMER_IEN_COMP1);
}










