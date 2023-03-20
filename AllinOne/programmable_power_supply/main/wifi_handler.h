/*
Written by Abhishek Tiwari
*/
#ifdef __cplusplus
extern "C" {
#endif

#ifndef WIFI_HANDLER_H
#define WIFI_HANDLER_H
//----------includes headers---------https://esp32tutorials.com/esp32-web-server-esp-idf/
//add send of stream of data to structure on server
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //Requires by memset
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include <esp_http_server.h>

#include "esp_wifi.h"
#include "esp_event.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "driver/gpio.h"
#include <lwip/sockets.h>
#include <lwip/sys.h>
#include <lwip/api.h>
#include <lwip/netdb.h>
//////////////////////////////////////////////
//-----------functions to call------------------
void initWifi();
void sendtowifi(float *vcc, uint16_t *raw,float *i);
//////////////////////////////////////////////////
#endif
#ifdef __cplusplus
}
#endif// WIFI_HANDLER_H 