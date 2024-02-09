/***************************************************************************//**
 *
 * @file scheduler.h
 * @brief This code involves scheduler declarations
 * Created on: Feb 5, 2024
 * Author: Malola Simman Srinivasan Kannan, Malola.Srinivasankannan@Colorado.edu
 * @student: Malola Simman Srinivasan Kannan, Malola.Srinivasankannan@Colorado.edu
 * Reference: Class lecture note 6
 *
 ******************************************************************************/

#ifndef SRC_SCHEDULER_H_
#define SRC_SCHEDULER_H_
#include "em_core.h"

#define UFbitmask (0x00000001) // underflow bit mask

// scheduler function declarations
void schedulerSetEventUF(void);
uint32_t getNextEvent(void);


#endif /* SRC_SCHEDULER_H_ */
