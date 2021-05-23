#include "timer_menu.h"
#include "running_timer_menu.h"
#include "../app.h"
#include <math/rectbbox.h>
#include "gui_list_processor.h"
#include "calibration_menu.h"
#include <app/display_message_state.h>
#include "menu_state.h"
#include <esp_log.h>

using libesp::ErrorType;
using libesp::BaseMenu;
using libesp::RGBColor;
using libesp::XPT2046;
using libesp::Point2Ds;
using libesp::TouchNotification;

static StaticQueue_t InternalQueue;
static uint8_t InternalQueueBuffer[TimerMenu::QUEUE_SIZE*TimerMenu::MSG_SIZE] = {0};
const char *TimerMenu::LOGTAG = "TimerMenu";

static libesp::AABBox2D UpArrow(Point2Ds(140,35), 20);
static libesp::Button UpArrowButton((const char *)"Up", uint16_t(0), &UpArrow,RGBColor::BLUE, RGBColor::RED);
static libesp::AABBox2D DownArrow(Point2Ds(140,80), 20);
static libesp::Button DownArrowButton((const char *)"Down", uint16_t(1), &DownArrow,RGBColor::BLUE, RGBColor::RED);
static libesp::RectBBox2D Clock(Point2Ds(60,58), 50, 42);
static libesp::CountDownTimer ClockWidget(&Clock, (const char *)"Clock", uint16_t(2),300);
static libesp::AABBox2D SetMin(Point2Ds(45,125),15);
static libesp::Button SetMinButton((const char *)"Min", uint16_t(3), &SetMin,RGBColor::BLUE, RGBColor::RED);
static libesp::AABBox2D SetSec(Point2Ds(85,125),15);
static libesp::Button SetSecButton((const char *)"Sec", uint16_t(4), &SetSec,RGBColor::BLUE, RGBColor::RED);
static libesp::RectBBox2D Start(Point2Ds(165,125), 30, 15);
static libesp::Button StartButton((const char *)"START", uint16_t(5),&Start,RGBColor::RED, RGBColor::BLUE);

static libesp::AABBox2D Close(Point2Ds(185,7),6);
static libesp::Button CloseButton((const char *)"X", uint16_t(1000), &Close,RGBColor::RED, RGBColor::BLUE);

static libesp::Widget *InterfaceElements[] = {&ClockWidget,&UpArrowButton,&DownArrowButton,&SetMinButton, &SetSecButton, &CloseButton, &StartButton};
static const int8_t NUM_INTERFACE_ITEMS = sizeof(InterfaceElements)/sizeof(InterfaceElements[0]);

TimerMenu::TimerMenu() :
	AppBaseMenu(), MyLayout(&InterfaceElements[0],NUM_INTERFACE_ITEMS, MyApp::get().getLastCanvasWidthPixel(), MyApp::get().getLastCanvasHeightPixel(), false) {
	
	InternalQueueHandler = xQueueCreateStatic(QUEUE_SIZE,MSG_SIZE,&InternalQueueBuffer[0],&InternalQueue);
	MyLayout.reset();
}

TimerMenu::~TimerMenu() {

}

int64_t TimerMenu::getTimerMS() {
	return ClockWidget.getTimerMS();
}


ErrorType TimerMenu::onInit() {
	MyApp::get().getDisplay().fillScreen(RGBColor::BLACK);
	//empty queue
	TouchNotification *pe = nullptr;
	for(int i=0;i<2;i++) {
		if(xQueueReceive(InternalQueueHandler, &pe, 0)) {
			delete pe;
		}
	}
	SetMinButton.setSelected(true);
	SetSecButton.setSelected(false);
	MyApp::get().getTouch().addObserver(InternalQueueHandler);
	MyLayout.reset();

	return ErrorType();
}

libesp::BaseMenu::ReturnStateContext TimerMenu::onRun() {
	BaseMenu *nextState = this;

	TouchNotification *pe = nullptr;
	Point2Ds TouchPosInBuf;
	libesp::Widget *widgetHit = nullptr;
	bool penUp = false;
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
	if(widgetHit) {
		ESP_LOGI(LOGTAG, "Widget %s hit\n", widgetHit->getName());
		switch(widgetHit->getWidgetID()) {
		case 0:
			if(penUp) {
				UpArrowButton.setSelected(false);
			} else {
				UpArrowButton.setSelected(true);
				if(SetMinButton.isSelected()) {
					ClockWidget.incrementTime(60);
				} else {
					ClockWidget.incrementTime(10);
				}
			}
			break;
		case 1:
			if(penUp) {
				DownArrowButton.setSelected(false);
			} else {
				DownArrowButton.setSelected(true);
				if(SetMinButton.isSelected()) {
					ClockWidget.incrementTime(-60);
				} else {
					ClockWidget.incrementTime(-10);
				}
			}
			break;
		case 3:
			if(penUp) {
				SetMinButton.setSelected(true);
				SetSecButton.setSelected(false);
			}
			break;
		case 4:
			if(penUp) {
				SetMinButton.setSelected(false);
				SetSecButton.setSelected(true);
			}
			break;
		case 5:
			if(penUp) {
				nextState = MyApp::get().getRunningTimerMenu();
			}
			break;
		case 1000:
			if(penUp) {
				nextState = MyApp::get().getMenuState();
			}
			break;
		default:
			break;
		}
	}
	MyLayout.draw(&MyApp::get().getDisplay());

	return BaseMenu::ReturnStateContext(nextState);
}

ErrorType TimerMenu::onShutdown() {
	MyApp::get().getTouch().removeObserver(InternalQueueHandler);
	return ErrorType();
}

