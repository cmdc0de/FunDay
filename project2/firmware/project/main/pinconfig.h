#ifndef PINCONFIG_H
#define PINCONFIG_H

#include "soc/gpio_struct.h"
#include "driver/gpio.h"

static gpio_num_t I2C_SCL = GPIO_NUM_22;
static gpio_num_t I2C_SDA = GPIO_NUM_21;

#define PIN_NUM_TOUCH_CLK  GPIO_NUM_16
#define PIN_NUM_TOUCH_CS   GPIO_NUM_4
#define PIN_NUM_TOUCH_IRQ  GPIO_NUM_13
#define PIN_NUM_TOUCH_MOSI GPIO_NUM_2
#define PIN_NUM_TOUCH_MISO GPIO_NUM_15

#define PIN_NUM_DISPLAY_MISO      GPIO_NUM_17
#define PIN_NUM_DISPLAY_MOSI      GPIO_NUM_18
#define PIN_NUM_DISPLAY_CLK       GPIO_NUM_5
#define PIN_NUM_DISPLAY_CS        GPIO_NUM_21
#define PIN_NUM_DISPLAY_DATA_CMD  GPIO_NUM_19
#define PIN_NUM_DISPLAY_BACKLIGHT NOPIN
#define PIN_NUM_DISPLAY_RESET     NOPIN

//#define PIN_RED GPIO_NUM_14
//#define PIN_BLUE GPIO_NUM_26
//#define PIN_GREEN GPIO_NUM_27

static gpio_num_t BLINK_GPIO = GPIO_NUM_25;
static gpio_num_t TEMP_SENSOR = GPIO_NUM_12;

#endif
