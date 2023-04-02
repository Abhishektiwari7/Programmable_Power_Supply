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

//-------init of timer_0 interrupt for milliseconds---------
void init_Interrupt_Timer1_MSec ();
///////////////////////////////////////////////////////

//----------attach interrupt gpio to isr handler--------
void init_Interrupt_Gpio ();
////////////////////////////////////////////////////////

// //------------switch 0 Interrupt Handdler-------------
// static void IRAM_ATTR gpio_isr_handler_sw0 (void *arg);
// ///////////////////////////////////////////////////////

// //------------switch 1 Interrupt Handdler-------------
// static void IRAM_ATTR gpio_isr_handler_sw1 (void *arg);
// ////////////////////////////////////////////////////////

// //------------switch 2 Interrupt Handdler-------------
// static void IRAM_ATTR gpio_isr_handler_sw2 (void *arg);
// ///////////////////////////////////////////////////////

// //------------switch 3 Interrupt Handdler-------------
// static void IRAM_ATTR gpio_isr_handler_sw3 (void *arg);
// ////////////////////////////////////////////////////////

//-------timer interrupt callback handler---second------------
static bool timer_0_sec(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx);
/////////////////////////////////////////////////////////////

//-------timer interrupt callback handler------millisecond---------
static bool timer_1_Msec(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx);
/////////////////////////////////////////////////////////////
//--------switch handler debounce----------------------------------
void switchHandlerDebounce ();
/////////////////////////////////////////////////////////////////////
//-----------5 second alarm tick-----------------------------------
volatile uint8_t alarm_5_second_tick();
///////////////////////////////////////////////////////////////////

#endif
#ifdef __cplusplus
}
#endif// INTERRUPTHANDLER_H