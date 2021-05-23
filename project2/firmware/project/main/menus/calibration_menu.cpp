#include "calibration_menu.h"
#include "menu_state.h"
#include "../app.h"
#include <math/point.h>
#include <device/display/display_device.h>
#include <esp_log.h>

using libesp::ErrorType;
using libesp::BaseMenu;
using libesp::RGBColor;
using libesp::XPT2046;
using libesp::TouchNotification;
using libesp::Point2Ds;

const char *CalibrationMenu::LOGTAG = "CalibrationMenu";
static StaticQueue_t InternalQueue;
static uint8_t InternalQueueBuffer[CalibrationMenu::QUEUE_SIZE*CalibrationMenu::MSG_SIZE] = {0};

CalibrationMenu::CalibrationMenu(const char *NVSPartitionName) : AppBaseMenu(), CalibrationLocations(), CurrentIndex(TOTAL), CalibrationData(NVSPartitionName,"cd", false), Min(), Max(), Range() {
	InternalQueueHandler = xQueueCreateStatic(QUEUE_SIZE,MSG_SIZE,&InternalQueueBuffer[0],&InternalQueue);
}

ErrorType CalibrationMenu::loadCalibrationData() {
	size_t size = sizeof(CalibrationLocations);
	ErrorType et = CalibrationData.getBlob("caldata",&CalibrationLocations[0],size);
	if(et==ESP_ERR_NVS_NOT_FOUND) {
		et = ErrorType();
		ESP_LOGI(LOGTAG,"No Calibration Data");
	} else if(et.ok()) {
		ESP_LOGI(LOGTAG,"Loaded Calibration Data");
		for(int i=0;i<TOTAL;i++) {
			ESP_LOGI(LOGTAG,"POS: %d, X: %d Y: %d",i,
			(int)CalibrationLocations[i].getX(), 
			(int)CalibrationLocations[i].getY()); 
		}
		calculate();
	}
	return et;
}

bool CalibrationMenu::hasBeenCalibrated() {
	size_t size = sizeof(CalibrationLocations);
	return CalibrationData.getBlob("caldata",&CalibrationLocations[0],size)!=ESP_OK;
}

void CalibrationMenu::calculate() {
	int16_t minX = std::min(CalibrationLocations[TOP_LEFT].getX(),CalibrationLocations[BOTTOM_LEFT].getX());
	int16_t maxY = std::max(CalibrationLocations[TOP_LEFT].getY(),CalibrationLocations[TOP_RIGHT].getY());
	int16_t maxX = std::max(CalibrationLocations[TOP_RIGHT].getX(),CalibrationLocations[BOTTOM_RIGHT].getX());
	int16_t minY = std::min(CalibrationLocations[BOTTOM_LEFT].getY(),CalibrationLocations[BOTTOM_RIGHT].getY());

	Min = Point2Ds(minX,minY);
	Max = Point2Ds(maxX,maxY);
	Range = Point2Ds(maxX-minX,maxY-minY);

	ESP_LOGI(LOGTAG,"min(x,y):%d,%d max(x,y):%d,%d range(x,y):%d,%d",
		int32_t(Min.getX()), int32_t(Min.getY()),
		int32_t(Max.getX()), int32_t(Max.getY()),
		int32_t(Range.getX()), int32_t(Range.getY()));
}
	
Point2Ds CalibrationMenu::getPickPoint(const Point2Ds &pickPos) {
	ESP_LOGI(LOGTAG,"Pickpoint %d,%d", int32_t(pickPos.getX()),int32_t(pickPos.getY()));
	int16_t X = pickPos.getX()-Min.getX();
	int16_t Y = pickPos.getY()-Min.getY();
	X = X<0?0:X;
	Y = Y<0?0:Y;
	X = X>Range.getX()?Range.getX():X;
	Y = Y>Range.getY()?Range.getY():Y;
	ESP_LOGI(LOGTAG,"Pickpoint2 %d,%d", int32_t(X),int32_t(Y));
	float PctX = float(X)/float(Range.getX());
	//y is upside down
	float PctY = 1.0f- (float(Y)/float(Range.getY()));
	ESP_LOGI(LOGTAG,"Pickpoint pct %.2f,%.2f", PctX,PctY);
	float DisplayX = float(MyApp::get().getCanvasWidth())*PctX;
	float DisplayY = float(MyApp::get().getCanvasHeight())*PctY;
	return Point2Ds(int16_t(DisplayX),int16_t(DisplayY));
}

ErrorType CalibrationMenu::initNVS() {
	ErrorType et = CalibrationData.init();
	if(!et.ok()) {
		ESP_LOGI(LOGTAG, "1st InitNVS failed bc %s\n", et.toString());
		et = CalibrationData.initStorage();
		if(et.ok()) {
			et = CalibrationData.init();
		} else {
			ESP_LOGI(LOGTAG, "initStorage failed %s\n", et.toString());
		}
	}
	if(et.ok()) {
		CalibrationData.logInfo();
		et = loadCalibrationData();
	}
	return et;
}

CalibrationMenu::~CalibrationMenu() {

}

ErrorType CalibrationMenu::onInit() {
	MyApp::get().getDisplay().fillScreen(RGBColor::BLACK);
	CurrentIndex = MID;
	//empty queue
	TouchNotification *pe = nullptr;
	for(int i=0;i<QUEUE_SIZE;i++) {
		if(xQueueReceive(InternalQueueHandler, &pe, 0)) {
			delete pe;
		}
	}
	MyApp::get().getTouch().addObserver(InternalQueueHandler);
	return ErrorType();
}

void CalibrationMenu::drawCrossHairs() {
	uint16_t x = 0,y = 0;
	switch(CurrentIndex) {
	case MID:
		x = MyApp::get().getCanvasWidth()/2;
		y = MyApp::get().getCanvasHeight()/2;
		MyApp::get().getDisplay().drawVerticalLine(x, y-4, 8 , RGBColor::RED);
		MyApp::get().getDisplay().drawHorizontalLine(x-3, y, 8 , RGBColor::RED);
		break;
	case TOP_LEFT:
		x = 0;
		y = 0;
		MyApp::get().getDisplay().drawVerticalLine(0, 0, 5 , RGBColor::RED);
		MyApp::get().getDisplay().drawHorizontalLine(0, 0, 5 , RGBColor::RED);
		break;
	case TOP_RIGHT:
		x = MyApp::get().getLastCanvasWidthPixel();
		y = 0;
		MyApp::get().getDisplay().drawVerticalLine(x, y, 5 , RGBColor::RED);
		MyApp::get().getDisplay().drawHorizontalLine(x-5, y, 5 , RGBColor::RED);
		break;
	case BOTTOM_LEFT:
		x = 0;
		y = MyApp::get().getLastCanvasHeightPixel();
		MyApp::get().getDisplay().drawVerticalLine(x, y-5, 5 , RGBColor::RED);
		MyApp::get().getDisplay().drawHorizontalLine(x, y, 5 , RGBColor::RED);
		break;
	case BOTTOM_RIGHT:
		x = MyApp::get().getLastCanvasWidthPixel();
		y = MyApp::get().getLastCanvasHeightPixel();
		MyApp::get().getDisplay().drawVerticalLine(x-1, y-5, 5 , RGBColor::RED);
		MyApp::get().getDisplay().drawHorizontalLine(x-4, y-2, 5 , RGBColor::RED);
		break;
	default:
		break;
	}
}

libesp::BaseMenu::ReturnStateContext CalibrationMenu::onRun() {
	BaseMenu *nextState = this;

	drawCrossHairs();
	TouchNotification *pe = nullptr;
	if(xQueueReceive(InternalQueueHandler, &pe, 0)) {
		ESP_LOGI(LOGTAG,"pd:%d",pe->isPenDown()?1:0);
		if(!pe->isPenDown()) {
			MyApp::get().getDisplay().fillScreen(RGBColor::BLACK);
			CalibrationLocations[CurrentIndex] = libesp::Point2Ds(pe->getX(), pe->getY());
			ESP_LOGI(LOGTAG,"CurrentIndex %d, x: %d, y:%d", (int32_t)CurrentIndex,
				(int32_t)CalibrationLocations[CurrentIndex].getX(),
				(int32_t)CalibrationLocations[CurrentIndex].getY());
			CurrentIndex++;	
		}
		delete pe;
	}

	if(CurrentIndex>=TOTAL) {
		//save array
		ErrorType et = CalibrationData.setBlob("caldata",&CalibrationLocations[0],sizeof(CalibrationLocations));
		if(et.ok()) {
			ESP_LOGI(LOGTAG,"Calibation data saved successfully!");
			calculate();
		}
		nextState = MyApp::get().getMenuState();
	}
	return BaseMenu::ReturnStateContext(nextState);
}

ErrorType CalibrationMenu::onShutdown() {
	//MenuList.selectedItem = 0;
	MyApp::get().getTouch().removeObserver(InternalQueueHandler);
	return ErrorType();
}

