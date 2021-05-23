#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/spi_master.h"
#include <nvs_memory.h>
#include "pinconfig.h"
#include "i2c.hpp"
#include <esp_log.h>
#include "app.h"
#include "pinconfig.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include <system.h>
#include <net/esp32inet.h>
#include <i2c.hpp>
#include <device/display/ssd1306.h>
#include <device/sensor/dht11.h>

static const char *TAG = "proj2";
using namespace libesp;

extern "C" {
	void app_main();
}

#define ESP_INTR_FLAG_DEFAULT 0
static uint32_t s_led_state = 1;
static const uint32_t CONFIG_BLINK_PERIOD = 1000;

static void blink_led(void)
{
    /* Set the GPIO level according to the state (LOW or HIGH)*/
    gpio_set_level(BLINK_GPIO, s_led_state);
}

static void configure_led(void)
{
    ESP_LOGI(TAG, "Example configured to blink GPIO LED!");
    gpio_reset_pin(BLINK_GPIO);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
}


void app_main() {
	static const char *LOGTAG="APP_MAIN";
	gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);

	libesp::ErrorType et;
	et = MyApp::get().init();

	if(!et.ok()) {
		ESP_LOGE(LOGTAG,"init error: %s", et.toString());
	}

	//ESP32_I2CMaster::doIt();
	//ESP_LOGI(LOGTAG,"scan I2c1");
	//ESP32_I2CMaster I2c1(I2C_SCL2,I2C_SDA2,400000, I2C_NUM_1, 0, 16);
	//I2c1.init(false);
	//I2c1.scan();
	
	libesp::System::get().logSystemInfo();

	if(!(et=ESP32INet::get()->init()).ok()) {
		ESP_LOGE(TAG,"failed to init network %s", et.toString());
	} else {
		ESP32INet::dumpToLog();
	}

	configure_led();
	ESP32_I2CMaster I2CMaster(I2C_SCL, I2C_SDA, 400000, I2C_NUM_0,0,1024);
	SSD1306 Display;
	Display.init(&I2CMaster,true);
	I2CMaster.scan();
	Display.fill(SSD1306::COLOR_BLACK);
	Display.drawFilledCircle(64,30,15,SSD1306::COLOR_WHITE);
	Display.updateScreen();
	vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
	Display.fill(SSD1306::COLOR_BLACK);
	Display.gotoXY(1,23);
	Display.puts("Hello World!", &Font_11x18, SSD1306::COLOR_WHITE);
	Display.updateScreen();
	vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
	Display.fill(SSD1306::COLOR_BLACK);
	Display.updateScreen();
	//
	DHT11_init(TEMP_SENSOR);
	printf("Temperature is %d \n", DHT11_read().temperature);
   printf("Humidity is %d\n", DHT11_read().humidity);
   printf("Status code is %d\n", DHT11_read().status);

   do {
		et = MyApp::get().run();
		ESP_LOGI(TAG, "Turning the LED %s!", s_led_state == true ? "ON" : "OFF");
		blink_led();
		/* Toggle the LED state */
		s_led_state = !s_led_state;
		vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
		Display.fill(SSD1306::COLOR_BLACK);
		char buf[24] = {'\0'};
		sprintf(&buf[0],"T: %d", DHT11_read().temperature);
		Display.gotoXY(10,20);
		Display.puts(&buf[0], &Font_11x18, SSD1306::COLOR_WHITE);
		sprintf(&buf[0],"H: %d", DHT11_read().humidity);
		Display.gotoXY(10,40);
		Display.puts(&buf[0], &Font_11x18, SSD1306::COLOR_WHITE);
		Display.updateScreen();
		vTaskDelay(3000 / portTICK_RATE_MS);
	} while (et.ok());

}
