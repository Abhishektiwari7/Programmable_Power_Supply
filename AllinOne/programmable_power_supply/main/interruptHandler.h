/*
Written by Abhishek Tiwari
*/
#ifdef __cplusplus
extern "C" {
#endif

#ifndef INTERRUPTHANDLER_H
#define INTERRUPTHANDLER_H

//--------header files of architecture-----------
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "driver/gptimer.h" //for timers
//////////////////////////////////////////////////
//---------macros for interrupt------------------
#define GPIO_LED (gpio_num_t)(2) //led
static const char *TAG_INT = "Interrupt";
/////////////////////////////////////////////////

//-------------interrupt handler and init functions-----------------

//-------init of timer_0 interrupt for seconds---------
void init_Interrupt_Timer0_Sec ();
///////////////////////////////////////////////////////

//----------attach interrupt gpio to isr handler--------
void init_Interrupt_Gpio ();
////////////////////////////////////////////////////////

//------------switch 3 Interrupt Handdler-------------
static void IRAM_ATTR gpio_isr_handler_sw2 (void *arg);
///////////////////////////////////////////////////////

//------------switch 4 Interrupt Handdler-------------
static void IRAM_ATTR gpio_isr_handler_sw3 (void *arg);
////////////////////////////////////////////////////////

//-------timer interrupt callback handler---------------
static bool timer_0_sec(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx);
/////////////////////////////////////////////////////////////

#endif
#ifdef __cplusplus
}
#endif// INTERRUPTHANDLER_H