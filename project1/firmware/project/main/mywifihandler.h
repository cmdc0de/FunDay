#ifndef MYWIFIEVENTHANDLER_H_
#define MYWIFIEVENTHANDLER_H_

#include <net/wifieventhandler.h>
#include <net/webserver.h>
#include <error_type.h>


class MyWiFiEventHandler : public libesp::WiFiEventHandler {
public:
	MyWiFiEventHandler();
	libesp::ErrorType init();
	virtual ~MyWiFiEventHandler();
	virtual libesp::ErrorType apStaConnected(wifi_event_ap_staconnected_t *info);
	virtual libesp::ErrorType apStaDisconnected(wifi_event_ap_stadisconnected_t *info);
	virtual libesp::ErrorType apStart();
	virtual libesp::ErrorType apStop();
	virtual libesp::ErrorType staConnected(system_event_sta_connected_t *info);
	virtual libesp::ErrorType staDisconnected(system_event_sta_disconnected_t *info);
	virtual libesp::ErrorType staGotIp(system_event_sta_got_ip_t info);
	virtual libesp::ErrorType staScanDone(system_event_sta_scan_done_t *info);
	virtual libesp::ErrorType staAuthChange(system_event_sta_authmode_change_t *info);
	virtual libesp::ErrorType wpsERPinSuccess(wifi_event_sta_wps_er_success_t *info) override;
	virtual libesp::ErrorType wpsERFailed(wifi_event_sta_wps_fail_reason_t *info) override;
	virtual libesp::ErrorType wpsERPin(wifi_event_sta_wps_er_pin_t *info);
	virtual libesp::ErrorType apProbeReq(wifi_event_ap_probe_req_rx_t *info);
	virtual libesp::ErrorType ftmReport(wifi_event_ftm_report_t *info);
	virtual libesp::ErrorType staBSSRSSILow(wifi_event_bss_rssi_low_t *info);
	virtual libesp::ErrorType actionTXStatus(wifi_event_action_tx_status_t *info);
	virtual libesp::ErrorType rocDone(wifi_event_roc_done_t *info);
   virtual libesp::ErrorType staWPSERTimeout();
   virtual libesp::ErrorType staBeaconTimeout();
	virtual libesp::ErrorType wpsPCBOverLap();
	virtual libesp::ErrorType staStart();
	virtual libesp::ErrorType staStop();
	virtual libesp::ErrorType wifiReady();
protected:
	libesp::HTTPWebServer MyWebServer;
};

#endif 
