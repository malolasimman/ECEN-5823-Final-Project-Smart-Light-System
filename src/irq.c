/***************************************************************************//**
 *
 * @file irq.c
 * @brief This code involves irq handler definitions
 * Created on: Jan 30, 2024
 * Author: Malola Simman Srinivasan Kannan, Malola.Srinivasankannan@Colorado.edu
 * @student: Malola Simman Srinivasan Kannan, Malola.Srinivasankannan@Colorado.edu
 * Reference: Class lecture note 5
 *
 ******************************************************************************/

/**************************************************************************//**
 * Headers
 *****************************************************************************/
#include "irq.h"
#include "em_letimer.h"
#include "gpio.h"
#include "em_core.h"
// Include logging specifically for this .c file
#define INCLUDE_LOG_DEBUG 1
#include "log.h"
#include "i2c.h"

uint32_t timeTicks =0;
/**************************************************************************//**
 * Low Energy Timer0 Interrupt handler
 *****************************************************************************/
 void LETIMER0_IRQHandler (void){

  CORE_DECLARE_IRQ_STATE;

  // NVIC IRQs are disabled
  CORE_ENTER_CRITICAL();

  // First: determine source of IRQ
  uint32_t flags = LETIMER_IntGetEnabled(LETIMER0);

  // Second: clear source of IRQ set in step 3
  LETIMER_IntClear(LETIMER0,0xFFFFFFFF);

  // Third: perform whatever processing is required
  if(flags & (LETIMER_IF_UF)){
      timeTicks++;
      schedulerSetEventUF();
  }

  if(flags & (LETIMER_IF_COMP1)){
      schedulerSetEventCOMP1();
  }

  // NVIC IRQs are re-enabled
  CORE_EXIT_CRITICAL();

 }//LETIMER0_IRQHandler

 uint32_t letimerMilliseconds(){

   uint32_t currtime = 0;

   CORE_DECLARE_IRQ_STATE;

   // NVIC IRQs are disabled
   CORE_ENTER_CRITICAL();

   currtime = timeTicks;

   // NVIC IRQs are re-enabled
   CORE_EXIT_CRITICAL();

   return currtime;
 }

 void I2C0_IRQHandler(void)
 {
   // this can be locally defined
   I2C_TransferReturn_TypeDef transferStatus;

   // This shepherds the IC2 transfer along,
   // it’s a state machine! see em_i2c.c
   // It accesses global variables :
   // transferSequence
   // cmd_data
   // read_data
   // that we put into the data structure passed
   // to I2C_TransferInit()

   transferStatus = I2C_Transfer(I2C0);

   if (transferStatus == i2cTransferDone)
   {
       schedulerSetI2CEvent();
   }
//   else if (transferStatus < 0)
//   {
//       LOG_ERROR("%d \r\n", transferStatus);
//   }
 } // I2C0_IRQHandler()

