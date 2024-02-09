/*
 * scheduler.c
 *
 *  Created on: Feb 5, 2024
 *      Author: malolasimman.s.k
 */

#include "scheduler.h"

static uint32_t events =0;

void schedulerSetEventUF(void)
{
  CORE_DECLARE_IRQ_STATE;

  CORE_ENTER_CRITICAL(); // enter critical, turn off interrupts in NVIC

  events |= UFbitmask;

  CORE_EXIT_CRITICAL(); // exit critical, re-enable interrupts in NVIC
} // schedulerSetEventUF



// scheduler routine to return event to main()code and clear that event
uint32_t getNextEvent(void)
{

  uint32_t theEvent = 0; // occurred events

  CORE_DECLARE_IRQ_STATE;

  CORE_ENTER_CRITICAL(); // enter critical, turn off interrupts in NVIC

  theEvent = events;     // pass the occurred events to main process
  events = 0;            // clear the source of events

  CORE_EXIT_CRITICAL();  // exit critical, re-enable interrupts in NVIC

  return theEvent;       // return event to main process

} // getNextEvent()

