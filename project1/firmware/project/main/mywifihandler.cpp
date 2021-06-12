/*
 * WiFiEventHandler.cpp
 *
 *  Created on: Feb 25, 2017
 *      Author: kolban
 */

#include "mywifihandler.h"
#include <esp_event.h>
#include <esp_wifi.h>
#include <esp_err.h>
#include <esp_log.h>
#include "sdkconfig.h"

static const char* LOG_TAG = "MyWiFiEventHandler";

static esp_err_t root_get_handler(httpd_req_t *req)
{
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, "<h1>Hello Secure World!</h1>", HTTPD_RESP_USE_STRLEN);

    return ESP_OK;
}

static const httpd_uri_t root = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = root_get_handler
};


MyWiFiEventHandler::MyWiFiEventHandler() : MyWebServer() {
} 

libesp::ErrorType MyWiFiEventHandler::init() {
	ESP_LOGI(LOG_TAG,"init wifi event handler");
	extern unsigned char cacert_pem_start[] asm("_binary_cacert_pem_start");
   extern unsigned char cacert_pem_end[]   asm("_binary_cacert_pem_end");
   uint8_t *cacert_pem = cacert_pem_start;
   uint32_t cacert_len = cacert_pem_end - cacert_pem_start;
   extern unsigned char prvtkey_pem_start[] asm("_binary_prvtkey_pem_start");
   extern unsigned char prvtkey_pem_end[]   asm("_binary_prvtkey_pem_end");
   uint8_t *prvtkey_pem = prvtkey_pem_start;
   uint32_t prvtkey_len = prvtkey_pem_end - prvtkey_pem_start;
	libesp::ErrorType et;
	et = MyWebServer.init(cacert_pem,cacert_len,prvtkey_pem,prvtkey_len);
	if(et.ok()) {
		return MyWebServer.registerHandle(root);
	} else {
		return et;
	}
}

/**
 * @brief Handle the Station Got IP event.
 * Handle having received/assigned an IP address when we are a station.
 * @param [in] event_sta_got_ip The Station Got IP event.
 * @return An indication of whether or not we processed the event successfully.
 */
libesp::ErrorType MyWiFiEventHandler::staGotIp(system_event_sta_got_ip_t info) {
    ESP_LOGD(LOG_TAG, "default staGotIp");
    return ESP_OK;
} // staGotIp


/**
 * @brief Handle the Access Point started event.
 * Handle an indication that the ESP32 has started being an access point.
 * @return An indication of whether or not we processed the event successfully.
 */
libesp::ErrorType MyWiFiEventHandler::apStart() {
    ESP_LOGD(LOG_TAG, "default apStart");
	 return MyWebServer.start();
} 


/**
 * @brief Handle the Access Point stop event.
 * Handle an indication that the ESP32 has stopped being an access point.
 * @return An indication of whether or not we processed the event successfully.
 */
libesp::ErrorType  MyWiFiEventHandler::apStop() {
    ESP_LOGD(LOG_TAG, "default apStop");
	 MyWebServer.stop();
	 return libesp::ErrorType();
} // apStop


libesp::ErrorType MyWiFiEventHandler::wifiReady() {
    ESP_LOGD(LOG_TAG, "default wifiReady");
    return ESP_OK;
} // wifiReady


libesp::ErrorType MyWiFiEventHandler::staStart() {
    ESP_LOGD(LOG_TAG, "default staStart");
    return ESP_OK;
} // staStart


libesp::ErrorType MyWiFiEventHandler::staStop() {
    ESP_LOGD(LOG_TAG, "default staStop");
    return ESP_OK;
} // staStop


libesp::ErrorType MyWiFiEventHandler::wpsERPinSuccess(wifi_event_sta_wps_er_success_t *info) {
    return ESP_OK;
}

libesp::ErrorType MyWiFiEventHandler::wpsERFailed(wifi_event_sta_wps_fail_reason_t *info) {
    return ESP_OK;
}

libesp::ErrorType MyWiFiEventHandler::wpsERPin(wifi_event_sta_wps_er_pin_t *info) {
	return ESP_OK;
}

libesp::ErrorType MyWiFiEventHandler::apProbeReq(wifi_event_ap_probe_req_rx_t *info) {
	return ESP_OK;
}

libesp::ErrorType MyWiFiEventHandler::ftmReport(wifi_event_ftm_report_t *info) {
	return ESP_OK;
}

libesp::ErrorType MyWiFiEventHandler::staBSSRSSILow(wifi_event_bss_rssi_low_t *info) {
	return ESP_OK;
}

libesp::ErrorType MyWiFiEventHandler::actionTXStatus(wifi_event_action_tx_status_t *info) {
	return ESP_OK;

}

libesp::ErrorType MyWiFiEventHandler::rocDone(wifi_event_roc_done_t *info) {
	return ESP_OK;
}

libesp::ErrorType MyWiFiEventHandler::staWPSERTimeout() {
	return ESP_OK;
}

libesp::ErrorType MyWiFiEventHandler::staBeaconTimeout() {
	return ESP_OK;
}

/**
 * @brief Handle the Station Connected event.
 * Handle having connected to remote AP.
 * @param [in] event_connected system_event_sta_connected_t.
 * @return An indication of whether or not we processed the event successfully.
 */
libesp::ErrorType MyWiFiEventHandler::staConnected(system_event_sta_connected_t *info) {
	ESP_LOGD(LOG_TAG, "default staConnected");
	return ESP_OK;
} // staConnected


/**
 * @brief Handle the Station Disconnected event.
 * Handle having disconnected from remote AP.
 * @param [in] event_disconnected system_event_sta_disconnected_t.
 * @return An indication of whether or not we processed the event successfully.
 */
libesp::ErrorType MyWiFiEventHandler::staDisconnected(system_event_sta_disconnected_t *info) {
    ESP_LOGD(LOG_TAG, "default staDisconnected");
    return ESP_OK;
} // staDisconnected


/**
 * @brief Handle a Station Connected to AP event.
 * Handle having a station connected to ESP32 soft-AP.
 * @param [in] event_sta_connected system_event_ap_staconnected_t.
 * @return An indication of whether or not we processed the event successfully.
 */
libesp::ErrorType MyWiFiEventHandler::apStaConnected(wifi_event_ap_staconnected_t *info) {
	ESP_LOGI(LOG_TAG, "station "MACSTR" join, AID=%d", MAC2STR(info->mac), info->aid);
    return ESP_OK;
} // apStaConnected


/**
 * @brief Handle a Station Disconnected from AP event.
 * Handle having a station disconnected from ESP32 soft-AP.
 * @param [in] event_sta_disconnected system_event_ap_stadisconnected_t.
 * @return An indication of whether or not we processed the event successfully.
 */
libesp::ErrorType MyWiFiEventHandler::apStaDisconnected(wifi_event_ap_stadisconnected_t *info) {
	ESP_LOGI(LOG_TAG, "station "MACSTR" join, AID=%d", MAC2STR(info->mac), info->aid);
    return ESP_OK;
} // apStaDisconnected


/**
 * @brief Handle a Scan for APs done event.
 * Handle having an ESP32 station scan (APs) done.
 * @param [in] event_scan_done system_event_sta_scan_done_t.
 * @return An indication of whether or not we processed the event successfully.
 */
libesp::ErrorType MyWiFiEventHandler::staScanDone(system_event_sta_scan_done_t *info) {
    ESP_LOGD(LOG_TAG, "default staScanDone");
    return ESP_OK;
} // staScanDone


/**
 * @brief Handle the auth mode of APs change event.
 * Handle having the auth mode of AP ESP32 station connected to changed.
 * @param [in] event_auth_change system_event_sta_authmode_change_t.
 * @return An indication of whether or not we processed the event successfully.
 */
libesp::ErrorType MyWiFiEventHandler::staAuthChange(system_event_sta_authmode_change_t *info) {
    ESP_LOGD(LOG_TAG, "default staAuthChange");
    return ESP_OK;
} // staAuthChange

libesp::ErrorType MyWiFiEventHandler::wpsPCBOverLap() {
	return ESP_OK;
}

MyWiFiEventHandler::~MyWiFiEventHandler() {
} 
