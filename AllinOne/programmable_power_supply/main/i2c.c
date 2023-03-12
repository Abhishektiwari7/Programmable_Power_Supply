/*
Written by Abhishek Tiwari
*/
#include "i2c.h" //i2c config

void init_I2C () {
/* i2c setup ----------------------------------------- */
// Config profile for espressif I2C lib
i2c_config_t i2c_cfg = {            
.mode = I2C_MODE_MASTER, 
.sda_io_num = SDA_IO,
.scl_io_num = SCL_IO,
.sda_pullup_en = GPIO_PULLUP_ENABLE,
.scl_pullup_en = GPIO_PULLUP_ENABLE,
.master.clk_speed = FREQ_HZ,
};
i2c_param_config(I2C_NUM, &i2c_cfg);
i2c_driver_install(I2C_NUM, I2C_MODE, I2C_RX_BUF_STATE, I2C_TX_BUF_STATE, I2C_INTR_ALOC_FLAG);
}