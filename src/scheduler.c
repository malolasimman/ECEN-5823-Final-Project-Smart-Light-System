/***************************************************************************//**
 *
 * @file scheduler.c
 * @brief This code involves scheduler function definitions
 * Created on: Feb 5, 2024
 * Author: Malola Simman Srinivasan Kannan, Malola.Srinivasankannan@Colorado.edu
 * @student: Malola Simman Srinivasan Kannan, Malola.Srinivasankannan@Colorado.edu
 * Reference: Class lecture note 6
 *
 ******************************************************************************/

#include "scheduler.h"

#include "timers.h"
#include "irq.h"
#include "em_core.h"


#include "i2c.h"
#include "em_common.h"
#include "app_assert.h"
#include "app.h"
static uint32_t events =0;


/**************************************************************************//**
 * schedulerSetEventUF function
 *****************************************************************************/
void schedulerSetEventUF(void)
{
  CORE_DECLARE_IRQ_STATE;

  CORE_ENTER_CRITICAL(); // enter critical, turn off interrupts in NVIC
  events |= UFbitmask;

  CORE_EXIT_CRITICAL(); // exit critical, re-enable interrupts in NVIC
} // schedulerSetEventUF

void schedulerSetEventCOMP1(void)
{
  CORE_DECLARE_IRQ_STATE;

  CORE_ENTER_CRITICAL(); // enter critical, turn off interrupts in NVIC
  events |= COMP1bitmask;

  CORE_EXIT_CRITICAL(); // exit critical, re-enable interrupts in NVIC
}//schedulerSetEventCOMP1

/**************************************************************************//**
 * scheduler routine to return event to main()code and clear that event
 *****************************************************************************/
uint32_t getNextEvent(void)
{

  uint32_t theEvent = 0; // occurred events

  CORE_DECLARE_IRQ_STATE;

  CORE_ENTER_CRITICAL(); // enter critical, turn off interrupts in NVIC

  if(events & UFbitmask)
  {
    theEvent = UFbitmask;     // pass the occurred events to main process
    events &= ~UFbitmask;            // clear the source of events
  }
  else if(events & COMP1bitmask)
  {
    theEvent = COMP1bitmask;     // pass the occurred events to main process
    events &= ~COMP1bitmask;            // clear the source of events
  }
  else if(events & I2C_COMPLETED)
  {
    theEvent = I2C_COMPLETED;     // pass the occurred events to main process
    events &= ~I2C_COMPLETED;            // clear the source of events
  }


  CORE_EXIT_CRITICAL();  // exit critical, re-enable interrupts in NVIC

  return theEvent;       // return event to main process

} // getNextEvent()


void schedulerSetI2CEvent()
{
  CORE_DECLARE_IRQ_STATE;

  CORE_ENTER_CRITICAL(); // enter critical, turn off interrupts in NVIC
  events |= I2C_COMPLETED;

  CORE_EXIT_CRITICAL(); // exit critical, re-enable interrupts in NVIC
}//schedulerSetI2CEvent



void temperature_state_machine(uint32_t event)
{
  static my_states curr_state = IDLE;
  static my_states next_state = IDLE;

  switch (curr_state)
  {
    case IDLE:
      if(event == UF_EVENT)
      {
          GPIO_PinOutSet(gpioPortD, I2C_ENABLE);
          timerWaitUs_irq(_Poweruptime); //80ms
          next_state = STATE1;
      }
      break;

     case STATE1:
       if(event == COMP1_EVENT)
       {
           init_temp();
           sl_power_manager_em_t em = EM1;
           sl_power_manager_add_em_requirement(em);
           start_temp_I2CWrite();
           next_state = STATE2;
       }

       break;

    case STATE2:
      if(event == I2C_COMPLETE)
      {
          NVIC_DisableIRQ(I2C0_IRQn);
          sl_power_manager_remove_em_requirement(EM1);
          timerWaitUs_irq(_14bitCONVTIME); //10.8ms
          next_state = STATE3;
      }
      break;

    case STATE3:
      if(event == COMP1_EVENT)
      {
         sl_power_manager_em_t em = EM1;
         sl_power_manager_add_em_requirement(em);
         Read_TemperatureSensor();
         next_state = STATE4;
      }
      break;

    case STATE4:
      if(event == I2C_COMPLETE)
      {
          NVIC_DisableIRQ(I2C0_IRQn);
          sl_power_manager_remove_em_requirement(EM1);
          Convert_TemperatureSensor();
          GPIO_PinOutClear(gpioPortD, I2C_ENABLE);
          next_state = IDLE;
      }
      break;

    default:
      break;
  }
  curr_state = next_state;
}

