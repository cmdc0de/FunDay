#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include <cstring>
#include "driver/gpio.h"
#include <system.h>
#include <net/esp32inet.h>
#include <i2c.hpp>
#include <device/display/ssd1306.h>
#include <device/sensor/dht11.h>
#include <net/wifi.h>
#include "mywifihandler.h"

static const char *TAG = "proj1";
using namespace libesp;

extern "C" {
	void app_main(void);
}

static gpio_num_t BLINK_GPIO = GPIO_NUM_25;
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


void app_main(void) {
    // Initialize NVS
	esp_err_t ret = nvs_flash_init();
	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		ESP_ERROR_CHECK(nvs_flash_erase());
		ret = nvs_flash_init();
	}
	ESP_ERROR_CHECK( ret );

	System::get().logSystemInfo();
	ErrorType et;
	if(!(et=ESP32INet::get()->init()).ok()) {
		ESP_LOGE(TAG,"failed to init network %s", et.toString());
	} else {
		ESP32INet::get()->createWifiInterfaceAP();
		ESP_LOGI(TAG,"wifi interface created ");
		vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
		ESP32INet::dumpToLog();
	}

	if(ESP_OK==esp_event_loop_create_default()) {
		ESP_LOGI(TAG,"Default event loop created");
	} else {
		ESP_LOGE(TAG,"Failed to create default event loop");
	}

	configure_led();
	ESP32_I2CMaster I2CMaster(GPIO_NUM_22, GPIO_NUM_21, 400000, I2C_NUM_0,0,1024);
	SSD1306 Display;
	Display.init(&I2CMaster,true);
	I2CMaster.scan();
	Display.fill(SSD1306::COLOR_BLACK);
	//Display.drawLine(0,10,100,10,SSD1306::COLOR_WHITE);
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
	DHT11_init(GPIO_NUM_18);
	printf("Temperature is %d \n", DHT11_read().temperature);
   printf("Humidity is %d\n", DHT11_read().humidity);
   printf("Status code is %d\n", DHT11_read().status);

	MyWiFiEventHandler wifiHandler;
	WiFi MyWifi;
	if(MyWifi.init()) {
		MyWifi.setWifiEventHandler(&wifiHandler);
		ESP_LOGI(TAG,"starting AP and web server");
		MyWifi.startAP("funday","");
		ESP32INet::dumpToLog();
	} else {
		ESP_LOGE(TAG,"failed to init wifi");
	}

	while (1) {
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
	}
}
