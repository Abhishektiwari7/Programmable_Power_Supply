/*
Written by Abhishek Tiwari
*/
#ifdef __cplusplus
extern "C" {
#endif

#ifndef I2C_H
#define I2C_H

//-------libraries-----------------------------------------------
#include <stdio.h> //functions
#include <esp_log.h> //esplogi
#include <driver/i2c.h> //i2c
//#include <freertos/FreeRTOS.h>
//#include <freertos/task.h>
/////////////////////////////////////////////////////////////////
//-------------macros for i2c function---------------------------

#define SDA_IO (21)                      /*!< gpio number for I2C master data  */
#define SCL_IO (22)                      /*!< gpio number for I2C master clock */

#define FREQ_HZ (400000)                 /*!< I2C master clock frequency */
#define TX_BUF_DISABLE (0)               /*!< I2C master doesn't need buffer */
#define RX_BUF_DISABLE (0)               /*!< I2C master doesn't need buffer */

#define I2C_NUM I2C_NUM_0                /*!< I2C number */
#define I2C_MODE I2C_MODE_MASTER         /*!< I2C mode to act as */
#define I2C_RX_BUF_STATE RX_BUF_DISABLE  /*!< I2C set rx buffer status */
#define I2C_TX_BUF_STATE TX_BUF_DISABLE  /*!< I2C set tx buffer status */
#define I2C_INTR_ALOC_FLAG (0)           /*!< I2C set interrupt allocation flag */

static const char *TAG_I2C = "I2C";
/////////////////////////////////////////////////////////////////

//-------functions-----------------------------------------------
void init_I2C ();
//////////////////////////////////////////////////////////////////






#endif
#ifdef __cplusplus
}
#endif// I2C_H