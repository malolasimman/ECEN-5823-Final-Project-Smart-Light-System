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
#include "app.h"

//#define UFbitmask ((uint32_t)1<<0) // underflow bit mask
//#define COMP1bitmask ((uint32_t)1<<1) // comp1 bit mask
//#define I2C_COMPLETED ((uint32_t)1<<2) // I2C bit mask


#define UFbitmask (0x00000001) // underflow bit mask
#define COMP1bitmask (0x00000002) // comp1 bit mask
#define I2C_COMPLETED (0x00000004) // I2C bit mask

// scheduler function declarations
void schedulerSetEventUF(void);
void schedulerSetEventCOMP1(void);
uint32_t getNextEvent(void);
void schedulerSetI2CEvent();
void temperature_state_machine(uint32_t event);

#endif /* SRC_SCHEDULER_H_ */
