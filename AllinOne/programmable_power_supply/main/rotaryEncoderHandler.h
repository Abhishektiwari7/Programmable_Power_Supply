/*
Written by Abhishek Tiwari
*/
#ifdef __cplusplus
extern "C" {
#endif

#ifndef ROTARYENCODERHANDLER_H
#define ROTARYENCODERHANDLER_H
//-----------------library-------------------
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "driver/pulse_cnt.h"
#include "driver/gpio.h"
// #include "esp_sleep.h"
/////////////////////////////////////////////
//------functions---------------------------
void init_Rotary_Encoder ();
void read_Rotary_Encoder ();

#endif
#ifdef __cplusplus
}
#endif// ROTARYENCODERHANDLER_H