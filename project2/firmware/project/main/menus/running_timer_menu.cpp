#include "running_timer_menu.h"
#include "timer_menu.h"
#include "../app.h"
#include <math/rectbbox.h>
#include "gui_list_processor.h"
#include "calibration_menu.h"
#include "menu_state.h"
#include <esp_log.h>

using libesp::ErrorType;
using libesp::BaseMenu;
using libesp::RGBColor;
using libesp::XPT2046;
using libesp::Point2Ds;
using libesp::TouchNotification;

static StaticQueue_t InternalQueue;
static uint8_t InternalQueueBuffer[RunningTimer::QUEUE_SIZE*RunningTimer::MSG_SIZE] = {0};
const char *RunningTimer::LOGTAG = "RunningTimerMenu";

static libesp::RectBBox2D Clock(Point2Ds(96,58), 90, 32);
static libesp::CountDownTimer ClockWidget(&Clock, (const char *)"Clock", uint16_t(0),300);
static libesp::RectBBox2D Start(Point2Ds(162,125), 36, 15);
static libesp::Button StartButton((const char *)"RESUME", uint16_t(1),&Start,RGBColor::RED, RGBColor::BLUE);
static libesp::RectBBox2D Stop(Point2Ds(162,125), 36, 15);
static libesp::Button StopButton((const char *)"PAUSE", uint16_t(2),&Start,RGBColor::RED, RGBColor::BLUE);
static libesp::AABBox2D Close(Point2Ds(185,7),6);
static libesp::Button CloseButton((const char *)"X", uint16_t(1000), &Close,RGBColor::RED, RGBColor::BLUE);

static libesp::Widget *RunningInterfaceElements[] = {&ClockWidget,&CloseButton, &StartButton, &StopButton};
static const int8_t RUNNING_NUM_INTERFACE_ITEMS = sizeof(RunningInterfaceElements)/sizeof(RunningInterfaceElements[0]);

RunningTimer::RunningTimer() :
	AppBaseMenu(), MyLayout(&RunningInterfaceElements[0],RUNNING_NUM_INTERFACE_ITEMS, MyApp::get().getLastCanvasWidthPixel(), MyApp::get().getLastCanvasHeightPixel(), false) {

	InternalQueueHandler = xQueueCreateStatic(QUEUE_SIZE,MSG_SIZE,&InternalQueueBuffer[0],&InternalQueue);
	MyLayout.reset();
}

RunningTimer::~RunningTimer() {

}


ErrorType RunningTimer::onInit() {
	MyApp::get().getDisplay().fillScreen(RGBColor::BLACK);
	//empty queue
	TouchNotification *pe = nullptr;
	for(int i=0;i<2;i++) {
		if(xQueueReceive(InternalQueueHandler, &pe, 0)) {
			delete pe;
		}
	}
	MyApp::get().getTouch().addObserver(InternalQueueHandler);
	MyLayout.reset();
	ClockWidget.setTimerMS(MyApp::get().getTimerMenu()->getTimerMS());
	ClockWidget.startTimer();
	ClockWidget.showMS(true);
	StartButton.hide();

	return ErrorType();
}

libesp::BaseMenu::ReturnStateContext RunningTimer::onRun() {
	BaseMenu *nextState = this;

	TouchNotification *pe = nullptr;
	Point2Ds TouchPosInBuf;
	libesp::Widget *widgetHit = nullptr;
	bool penUp = false;
	ClockWidget.update();
	if(xQueueReceive(InternalQueueHandler, &pe, 0)) {
		ESP_LOGI(LOGTAG,"que");
		Point2Ds screenPoint(pe->getX(),pe->getY());
		TouchPosInBuf = MyApp::get().getCalibrationMenu()->getPickPoint(screenPoint);
		ESP_LOGI(LOGTAG,"TouchPoint: X:%d Y:%d PD:%d", int32_t(TouchPosInBuf.getX()),
								 int32_t(TouchPosInBuf.getY()), pe->isPenDown()?1:0);
		penUp = !pe->isPenDown();
		delete pe;
		widgetHit = MyLayout.pick(TouchPosInBuf);
	}
	if(widgetHit && penUp) {
		ESP_LOGI(LOGTAG, "Widget %s hit\n", widgetHit->getName());
		switch(widgetHit->getWidgetID()) {
		case 1:
			StartButton.hide();
			StopButton.unHide();
			ClockWidget.unPause();
			break;
		case 2:
			StartButton.unHide();
			StopButton.hide();
			ClockWidget.pause();
			break;
		case 1000:
			nextState = MyApp::get().getMenuState();
			break;
		default:
			break;
		}
	}
	if(ClockWidget.isDone()) {
		StartButton.unHide();
		StopButton.hide();
	}

	MyLayout.draw(&MyApp::get().getDisplay());

	return BaseMenu::ReturnStateContext(nextState);
}

ErrorType RunningTimer::onShutdown() {
	MyApp::get().getTouch().removeObserver(InternalQueueHandler);
	return ErrorType();
}

