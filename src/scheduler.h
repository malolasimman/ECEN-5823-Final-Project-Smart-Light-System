/*
 * scheduler.h
 *
 *  Created on: Feb 5, 2024
 *      Author: malolasimman.s.k
 */

#ifndef SRC_SCHEDULER_H_
#define SRC_SCHEDULER_H_
#include "em_core.h"

#define UFbitmask (0x00000001)

void schedulerSetEventUF(void);
uint32_t getNextEvent(void);


#endif /* SRC_SCHEDULER_H_ */
