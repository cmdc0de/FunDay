#include "number_menu.h"
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
static uint8_t InternalQueueBuffer[NumberMenu::QUEUE_SIZE*NumberMenu::MSG_SIZE] = {0};
const char *NumberMenu::LOGTAG = "NumberMenu";

static libesp::AABBox2D One(Point2Ds(25,40), 15);
static libesp::Button OneButton((const char *)"1", uint16_t(0), &One,RGBColor::BLUE, RGBColor::RED);
static libesp::AABBox2D Two(Point2Ds(65,40), 15);
static libesp::Button TwoButton((const char *)"2", uint16_t(1), &Two,RGBColor::BLUE, RGBColor::RED);
static libesp::AABBox2D Three(Point2Ds(105,40), 15);
static libesp::Button ThreeButton((const char *)"3", uint16_t(2), &Three,RGBColor::BLUE, RGBColor::RED);
static libesp::AABBox2D Four(Point2Ds(145,40), 15);
static libesp::Button FourButton((const char *)"4", uint16_t(3), &Four,RGBColor::BLUE, RGBColor::RED);

static libesp::AABBox2D Close(Point2Ds(185,7),6);
static libesp::Button CloseButton((const char *)"X", uint16_t(1000), &Close,RGBColor::RED, RGBColor::BLUE);

static libesp::Widget *InterfaceElements[] = {
  &OneButton
    ,&TwoButton
    ,&ThreeButton
    ,&FourButton
    ,&CloseButton
};
static const int8_t NUM_INTERFACE_ITEMS = sizeof(InterfaceElements)/sizeof(InterfaceElements[0]);

NumberMenu::NumberMenu() :
	AppBaseMenu(), MyLayout(&InterfaceElements[0],NUM_INTERFACE_ITEMS, MyApp::get().getLastCanvasWidthPixel(), MyApp::get().getLastCanvasHeightPixel(), false) {
	
	InternalQueueHandler = xQueueCreateStatic(QUEUE_SIZE,MSG_SIZE,&InternalQueueBuffer[0],&InternalQueue);
	MyLayout.reset();
}

NumberMenu::~NumberMenu() {

}

ErrorType NumberMenu::onInit() {
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

	return ErrorType();
}

libesp::BaseMenu::ReturnStateContext NumberMenu::onRun() {
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
				OneButton.setSelected(false);
			} else {
				OneButton.setSelected(true);
			}
			break;
		case 1:
			if(penUp) {
				TwoButton.setSelected(false);
			} else {
				TwoButton.setSelected(true);
			}
			break;
		case 2:
			if(penUp) {
				ThreeButton.setSelected(false);
			} else {
        ThreeButton.setSelected(true);
      }
			break;
		case 3:
			if(penUp) {
				FourButton.setSelected(false);
			} else {
				FourButton.setSelected(true);
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

ErrorType NumberMenu::onShutdown() {
	MyApp::get().getTouch().removeObserver(InternalQueueHandler);
	return ErrorType();
}

