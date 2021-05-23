#ifndef APP_BASE_MENU_H
#define APP_BASE_MENU_H

#include <app/basemenu.h>
#include <device/display/gui.h>
#include <freertos.h>

namespace libesp {
class TouchNotification;
class GUIListData;
}

class AppBaseMenu : public libesp::BaseMenu {
public:
	static const char *LOGTAG;
public:
	AppBaseMenu() : libesp::BaseMenu() {}
	virtual ~AppBaseMenu(){}
protected:
	static void clearListBuffer();
	static char *getRow(uint8_t row);
protected:
	libesp::TouchNotification *processTouch(QueueHandle_t &queH, libesp::GUIListData &guiList, uint16_t itemCount, bool &penUp, bool &headerHit);
private:
	static char ListBuffer[10][64]; //height then width
	static uint8_t NumRows;
};

#endif
