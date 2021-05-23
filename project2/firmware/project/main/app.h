/*
 * app.h
 *
 * Author: cmdc0de
 */

#ifndef LIGHTBOX_APP_H
#define LIGHTBOX_APP_H

#include <app/app.h>

namespace libesp {
class GUI;
class DisplayDevice;
class DisplayMessageState;
class XPT2046;
};

enum ERRORS {
	APP_OK = libesp::ErrorType::APP_OK
	, OTA_INIT_FAIL = libesp::ErrorType::APP_BASE + 1
	, BT_INIT_FAIL
	, GAME_TASK_INIT_FAIL
	, EXPLOIT_TASK_INIT_FAIL
	, WIFI_TASK_INIT_FAIL
	, BUTTON_INIT_FAIL
	, TOP_BOARD_INIT_FAIL
};

class MyErrorMap : public libesp::IErrorDetail {
public:
	virtual const char *toString(int32_t err);
	virtual ~MyErrorMap() {}
};

class CalibrationMenu;
class MenuState;
class TimerMenu;
class RunningTimer;

class MyApp : public libesp::App {
public:
	struct MyAppMsg {
		int dummy;
	};
public:
	static const char *LOGTAG;
	static const int QUEUE_SIZE = 10;
	static const int ITEM_SIZE = sizeof(MyAppMsg);
	static const char *sYES;
	static const char *sNO;
	static const uint16_t DISPLAY_HEIGHT		= 240;
	static const uint16_t DISPLAY_WIDTH			= 320;
	//static const uint16_t FRAME_BUFFER_HEIGHT	= DISPLAY_HEIGHT;
	//static const uint16_t FRAME_BUFFER_WIDTH	= DISPLAY_WIDTH;
	//reminder ESP32 has 160KiB static and DRAM So a 1:1 buffer doesn't fit.
	static const uint16_t FRAME_BUFFER_HEIGHT	= 144;
	static const uint16_t FRAME_BUFFER_WIDTH	= 192;
	//static const uint16_t FRAME_BUFFER_HEIGHT	= 168;
	//static const uint16_t FRAME_BUFFER_WIDTH	= 224;
	static MyApp &get();
public:
	virtual ~MyApp();
	uint16_t getCanvasWidth();
	uint16_t getCanvasHeight();
	uint16_t getLastCanvasWidthPixel();
	uint16_t getLastCanvasHeightPixel();
	libesp::DisplayDevice &getDisplay();
	libesp::GUI &getGUI();
	MenuState *getMenuState();
	TimerMenu *getTimerMenu();
	CalibrationMenu *getCalibrationMenu();
	RunningTimer *getRunningTimerMenu();
	libesp::DisplayMessageState *getDisplayMessageState(libesp::BaseMenu *, const char *msg, uint32_t msDisplay);
	libesp::XPT2046 &getTouch();
	uint8_t *getBackBuffer();
	uint32_t getBackBufferSize();

protected:
	MyApp();
	virtual libesp::ErrorType onInit();
	virtual libesp::ErrorType onRun();
private:
	static MyApp mSelf;
	uint32_t MyNvsHandle;
	MyErrorMap AppErrors;
};

#endif /* DC27_APP_H */
