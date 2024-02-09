/*
 * timers.h
 *
 *  Created on: Jan 30, 2024
 *      Author: malolasimman.s.k
 */

#ifndef SRC_TIMERS_H_
#define SRC_TIMERS_H_

#include "em_letimer.h"

#define LETIMER_PERIOD_MS  (3000)

#define SEC_US (1000000)
#define SEC_MS (1000)

void timerWaitUs(uint32_t us_wait);

void initLETIMER0();

#endif /* SRC_TIMERS_H_ */
