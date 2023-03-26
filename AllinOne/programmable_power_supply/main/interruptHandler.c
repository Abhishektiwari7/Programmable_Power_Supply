#include "interruptHandler.h" //timer and gpio interrupt

//----------------timer macros------------------
#define TIMER0_SECOND         (1000000)  //  Hardware timer clock divider //1sec
#define TIMER1_MILLISECOND    (1000)     //  Hardware timer clock divider //1 msec
////////////////////////////////////////////////

//----------pin gpio number set-----------------
//Pins with internal pull up INPUT_PULLUP , NOT ON gpio12
#define GPIO_INTERRUPT_SW0 (gpio_num_t)(14)
#define GPIO_INTERRUPT_SW1 (gpio_num_t)(16)
#define GPIO_INTERRUPT_SW2 (gpio_num_t)(17)
#define GPIO_INTERRUPT_SW3 (gpio_num_t)(12)
///////////////////////////////////////////////////////////

//----------debounce on switch defines and variables--------------------
#define DEBOUNCING_INTERVAL_MS    200
#define RELEASE_PRESS_INTERVAL_MS 300
//---------switch 0---------------------
volatile bool cur_Switch_State_0 = 0;
volatile bool SW0_Pressed          = false;
//---------switch 1---------------------
volatile bool cur_Switch_State_1 = 0;
volatile bool SW1_Pressed          = false;
//---------switch 2---------------------
volatile bool cur_Switch_State_2 = 0;
volatile bool SW2_Pressed          = false;
//---------switch 3---------------------
volatile bool cur_Switch_State_3 = 0;
volatile bool SW3_Pressed          = false;
////////////////////////////////////////////////

//------------switch 1 Interrupt Handdler-------------
static void IRAM_ATTR gpio_isr_handler_sw0 (void *arg) {
//gpio_set_level(GPIO_LED,0);
cur_Switch_State_0 = 1;
//ESP_DRAM_LOGI(TAG_INT, "Interrupt Handler Call: Switch 0");
}
//////////////////////////////////////////////////////

//------------switch 2 Interrupt Handdler-------------
static void IRAM_ATTR gpio_isr_handler_sw1 (void *arg) {
//	gpio_set_level(GPIO_LED,gpio_get_level(GPIO_LED));
// state = gpio_get_level(GPIO_LED);
// gpio_set_level(GPIO_LED,0);
cur_Switch_State_1 = 1;
//ESP_DRAM_LOGI(TAG_INT, "Interrupt Handler Call: Switch 1");
}
///////////////////////////////////////////////////////

//------------switch 3 Interrupt Handdler-------------
static void IRAM_ATTR gpio_isr_handler_sw2 (void *arg) {
//	gpio_set_level(GPIO_LED,gpio_get_level(GPIO_LED));
// state = gpio_get_level(GPIO_LED);
//gpio_set_level(GPIO_LED,1);
cur_Switch_State_2 = 1;
//ESP_DRAM_LOGI(TAG_INT, "Interrupt Handler Call: Switch 2");
}
///////////////////////////////////////////////////////

//------------switch 4 Interrupt Handdler-------------
static void IRAM_ATTR gpio_isr_handler_sw3 (void *arg) {
//	gpio_set_level(GPIO_LED,gpio_get_level(GPIO_LED));
//gpio_set_level(GPIO_LED,0);
cur_Switch_State_3 = 1;
//ESP_DRAM_LOGI(TAG_INT, "Interrupt Handler Call: Switch 3");
}
////////////////////////////////////////////////////////

//-------timer interrupt callback handler----1 SECOND-----------
static bool timer_0_sec(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx) {
BaseType_t high_task_awoken = pdFALSE;
// Optional: send the event data to other task by OS queue
//-add here specific variables for toogle
static bool ON;
ON = !ON;  
gpio_set_level(GPIO_LED, ON);
// return whether we need to yield at the end of ISR
return high_task_awoken == pdTRUE;
}
/////////////////////////////////////////////////////////////

//-------timer interrupt callback handler----1 MILLISECOND-----------
static bool timer_1_Msec(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx) {
BaseType_t high_task_awoken = pdFALSE;
// static bool ON;
// ON = !ON;  
// gpio_set_level(GPIO_LED, ON);
// return whether we need to yield at the end of ISR
switchHandlerDebounce ();
return high_task_awoken == pdTRUE;
}
/////////////////////////////////////////////////////////////

//----------attach interrupt gpio to isr handler--------
void init_Interrupt_Gpio () {
//interrupt setting
gpio_config_t io_conf_0,io_conf_1,io_conf_2,io_conf_3;
//interrupt of falling edge
io_conf_0.intr_type = GPIO_INTR_NEGEDGE; // GPIO interrupt type : falling edge
io_conf_1.intr_type = GPIO_INTR_NEGEDGE; // GPIO interrupt type : falling edge
io_conf_2.intr_type = GPIO_INTR_NEGEDGE; // GPIO interrupt type : falling edge
io_conf_3.intr_type = GPIO_INTR_NEGEDGE; // GPIO interrupt type : falling edge
//bit mask of the pins
io_conf_0.pin_bit_mask = 1ULL<<GPIO_INTERRUPT_SW0;
io_conf_1.pin_bit_mask = 1ULL<<GPIO_INTERRUPT_SW1;
io_conf_2.pin_bit_mask = 1ULL<<GPIO_INTERRUPT_SW2;
io_conf_3.pin_bit_mask = 1ULL<<GPIO_INTERRUPT_SW3;
//set as input mode
io_conf_0.mode = GPIO_MODE_INPUT;
io_conf_1.mode = GPIO_MODE_INPUT;
io_conf_2.mode = GPIO_MODE_INPUT;
io_conf_3.mode = GPIO_MODE_INPUT;
//enable pull-up mode
io_conf_0.pull_up_en = GPIO_PULLUP_ENABLE;
io_conf_1.pull_up_en = GPIO_PULLUP_ENABLE;
io_conf_2.pull_up_en = GPIO_PULLUP_ENABLE;
io_conf_3.pull_up_en = GPIO_PULLUP_ENABLE;

io_conf_0.pull_down_en = GPIO_PULLDOWN_DISABLE;
io_conf_1.pull_down_en = GPIO_PULLDOWN_DISABLE;
io_conf_2.pull_down_en = GPIO_PULLDOWN_DISABLE;
io_conf_3.pull_down_en = GPIO_PULLDOWN_DISABLE;

gpio_config(&io_conf_0);
gpio_config(&io_conf_1);
gpio_config(&io_conf_2);
gpio_config(&io_conf_3);

//install gpio isr service
gpio_install_isr_service(0);
//hook isr handler for specific gpio pin
gpio_isr_handler_add(GPIO_INTERRUPT_SW0, gpio_isr_handler_sw0, (void*) GPIO_INTERRUPT_SW0);
gpio_isr_handler_add(GPIO_INTERRUPT_SW1, gpio_isr_handler_sw1, (void*) GPIO_INTERRUPT_SW1);
gpio_isr_handler_add(GPIO_INTERRUPT_SW2, gpio_isr_handler_sw2, (void*) GPIO_INTERRUPT_SW2);
gpio_isr_handler_add(GPIO_INTERRUPT_SW3, gpio_isr_handler_sw3, (void*) GPIO_INTERRUPT_SW3);

}
///////////////////////////////////////////////////////////////////////////////////////////

//-------init of timer_0 interrupt for seconds---------------
void init_Interrupt_Timer0_Sec () {
gptimer_handle_t gptimer = NULL;
gptimer_config_t timer_config;
timer_config.clk_src = GPTIMER_CLK_SRC_DEFAULT;
timer_config.direction = GPTIMER_COUNT_UP;
timer_config.resolution_hz = 1000000; // 1MHz, 1 tick=1us

ESP_ERROR_CHECK(gptimer_new_timer(&timer_config, &gptimer)); //create Timer
//---attach call back function--------
gptimer_event_callbacks_t cbs; //call back function struct
cbs.on_alarm = timer_0_sec;
ESP_ERROR_CHECK(gptimer_register_event_callbacks(gptimer, &cbs, NULL));

ESP_LOGI(TAG_INT, "Enable timer");
ESP_ERROR_CHECK(gptimer_enable(gptimer)); //enable timer


gptimer_alarm_config_t alarm_config0;
alarm_config0.reload_count = 0; //starting count of timer
alarm_config0.flags.auto_reload_on_alarm = true;  //auto reload enable
alarm_config0.alarm_count = TIMER0_SECOND; // period = 1s //set by user
ESP_ERROR_CHECK(gptimer_set_alarm_action(gptimer, &alarm_config0)); //set in action
ESP_ERROR_CHECK(gptimer_start(gptimer));   //start the timer to count
}
////////////////////////////////////////////////////////////////////////////////////

//-------init of timer_1 interrupt for milli seconds---------------
void init_Interrupt_Timer1_MSec () {
gptimer_handle_t gptimer1 = NULL;
gptimer_config_t timer_config1;
timer_config1.clk_src = GPTIMER_CLK_SRC_DEFAULT;
timer_config1.direction = GPTIMER_COUNT_UP;
timer_config1.resolution_hz = 1000000; // 1MHz, 1 tick=1us

ESP_ERROR_CHECK(gptimer_new_timer(&timer_config1, &gptimer1)); //create Timer
//---attach call back function--------
gptimer_event_callbacks_t cbs; //call back function struct
cbs.on_alarm = timer_1_Msec;
ESP_ERROR_CHECK(gptimer_register_event_callbacks(gptimer1, &cbs, NULL));

ESP_LOGI(TAG_INT, "Enable timer");
ESP_ERROR_CHECK(gptimer_enable(gptimer1)); //enable timer


gptimer_alarm_config_t alarm_config1;
alarm_config1.reload_count = 0; //starting count of timer
alarm_config1.flags.auto_reload_on_alarm = true;  //auto reload enable
alarm_config1.alarm_count = TIMER1_MILLISECOND; // period = 1s //set by user
ESP_ERROR_CHECK(gptimer_set_alarm_action(gptimer1, &alarm_config1)); //set in action
ESP_ERROR_CHECK(gptimer_start(gptimer1));   //start the timer to count
}
////////////////////////////////////////////////////////////////////////////////////

//------------Switch Handler debounce---------------------------------------------
//use me for debounce: https://github.com/khoih-prog/ESP32TimerInterrupt/blob/master/examples/SwitchDebounce/SwitchDebounce.ino
void switchHandlerDebounce () {
//---------------------switch 0-------------------------------------------------------
static unsigned int count_SW0_Pressed  = 0;
static unsigned int count_SW0_Released = 0;
if ( cur_Switch_State_0 == 1 ) { // Start debouncing counting debounceCountSWPressed and clear debounceCountSWReleased
    if (++count_SW0_Pressed >= DEBOUNCING_INTERVAL_MS) { // Call and flag SW0_Pressed
        if (!SW0_Pressed) {
            SW0_Pressed = true;
            count_SW0_Released = 0 ;
            // static bool ON;
            // ON = !ON;  
            // gpio_set_level(GPIO_LED, ON);
            ESP_DRAM_LOGI(TAG_INT, "Interrupt Handler Call: Switch 0 PRESS, state: %d, time: %d, switch press: %d",cur_Switch_State_0,count_SW0_Pressed,SW0_Pressed); //able to use in interrupt isr unlike esp_logi it crash code
        }
    } 
    if ( ( SW0_Pressed ) && (++count_SW0_Released >= RELEASE_PRESS_INTERVAL_MS)) {
        SW0_Pressed     = false;
       // ESP_DRAM_LOGI(TAG_INT, "Interrupt Handler Call: Switch 0 release: %d, time: %d",cur_Switch_State_0,count_SW0_Released); //able to use in interrupt isr unlike esp_logi it crash code
        count_SW0_Pressed = 0;
        cur_Switch_State_0 = 0;
    }
}
//--------switch 1---------------------------------------------------------------
static unsigned int count_SW1_Pressed  = 0;
static unsigned int count_SW1_Released = 0;
if ( cur_Switch_State_1 == 1 ) { // Start debouncing counting debounceCountSWPressed and clear debounceCountSWReleased
    if (++count_SW1_Pressed >= DEBOUNCING_INTERVAL_MS) { // Call and flag SW0_Pressed
        if (!SW1_Pressed) {
            SW1_Pressed = true;
            count_SW1_Released = 0 ;
            // static bool ON;
            // ON = !ON;  
            // gpio_set_level(GPIO_LED, ON);
            ESP_DRAM_LOGI(TAG_INT, "Interrupt Handler Call: Switch 1 PRESS, state: %d, time: %d, switch press: %d",cur_Switch_State_1,count_SW1_Pressed,SW1_Pressed); //able to use in interrupt isr unlike esp_logi it crash code
        }
    } 
    if ( ( SW1_Pressed ) && (++count_SW1_Released >= RELEASE_PRESS_INTERVAL_MS)) {
        SW1_Pressed     = false;
       // ESP_DRAM_LOGI(TAG_INT, "Interrupt Handler Call: Switch 1 release: %d, time: %d",cur_Switch_State_1,count_SW1_Released); //able to use in interrupt isr unlike esp_logi it crash code
        count_SW1_Pressed = 0;
        cur_Switch_State_1 = 0;
    }
}
//-----------switch 2-------------------------------------------------------------
static unsigned int count_SW2_Pressed  = 0;
static unsigned int count_SW2_Released = 0;
if ( cur_Switch_State_2 == 1 ) { // Start debouncing counting debounceCountSWPressed and clear debounceCountSWReleased
    if (++count_SW2_Pressed >= DEBOUNCING_INTERVAL_MS) { // Call and flag SW0_Pressed
        if (!SW2_Pressed) {
            SW2_Pressed = true;
            count_SW2_Released = 0 ;
            // static bool ON;
            // ON = !ON;  
            // gpio_set_level(GPIO_LED, ON);
            ESP_DRAM_LOGI(TAG_INT, "Interrupt Handler Call: Switch 2 PRESS, state: %d, time: %d, switch press: %d",cur_Switch_State_2,count_SW2_Pressed,SW2_Pressed); //able to use in interrupt isr unlike esp_logi it crash code
        }
    } 
    if ( ( SW2_Pressed ) && (++count_SW2_Released >= RELEASE_PRESS_INTERVAL_MS)) {
        SW2_Pressed     = false;
       // ESP_DRAM_LOGI(TAG_INT, "Interrupt Handler Call: Switch 2 release: %d, time: %d",cur_Switch_State_2,count_SW2_Released); //able to use in interrupt isr unlike esp_logi it crash code
        count_SW2_Pressed = 0;
        cur_Switch_State_2 = 0;
    }
}
///--------------switch 3---------------------------------------------------------
static unsigned int count_SW3_Pressed  = 0;
static unsigned int count_SW3_Released = 0;
if ( cur_Switch_State_3 == 1 ) { // Start debouncing counting debounceCountSWPressed and clear debounceCountSWReleased
    if (++count_SW3_Pressed >= DEBOUNCING_INTERVAL_MS) { // Call and flag SW0_Pressed
        if (!SW3_Pressed) {
            SW3_Pressed = true;
            count_SW3_Released = 0 ;
            // static bool ON;
            // ON = !ON;  
            // gpio_set_level(GPIO_LED, ON);
            ESP_DRAM_LOGI(TAG_INT, "Interrupt Handler Call: Switch 3 PRESS, state: %d, time: %d, switch press: %d",cur_Switch_State_3,count_SW3_Pressed,SW3_Pressed); //able to use in interrupt isr unlike esp_logi it crash code
        }
    } 
    if ( ( SW3_Pressed ) && (++count_SW3_Released >= RELEASE_PRESS_INTERVAL_MS)) {
        SW3_Pressed     = false;
       // ESP_DRAM_LOGI(TAG_INT, "Interrupt Handler Call: Switch 3 release: %d, time: %d",cur_Switch_State_3,count_SW3_Released); //able to use in interrupt isr unlike esp_logi it crash code
        count_SW3_Pressed = 0;
        cur_Switch_State_3 = 0;
    }
}
} //end of switchhandler
///////////////////////////////////////////////////////////////////////////////////