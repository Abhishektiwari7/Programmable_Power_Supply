
/******************************************************************************
*  Device:   MCP4922                                                          *
*  Platform: Arduino                                                          *
*  File:     MCP4922.h                                                        *
*                                                                             *
*  MCP4922.cpp - Library for driving MCP4922 2 channel DAC using hardware SPI.*
*  Created by Helge Nodland, January 1, 2015.                                 *
*  Released into the public domain.                                           *
******************************************************************************/



#ifndef MCP4922_h
#define MCP4922_h
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"

#include "driver/spi_master.h"

#include "MCP4922.h"
//------------dac init fuction-------------------------
void mcp4922_init(uint8_t SDI, uint8_t SCK, uint8_t CS);
//--------Set Channel A Voltage----------------------
void setDacVoltageMcp4922_CHA(uint16_t bit,uint8_t gain,uint8_t buffered,uint8_t shutDown );
//--------Set Channel B Voltage----------------------
void setDacVoltageMcp4922_CHB(uint16_t bit,uint8_t gain,uint8_t buffered,uint8_t shutDown );
//----------spi write function-----------------------------
void mcp4922_WriteSPI(uint16_t _data);

#endif
