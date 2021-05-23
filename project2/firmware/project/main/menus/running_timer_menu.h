#ifndef RUNNING_TIMER_MENU_H
#define RUNNING_TIMER_MENU_H

#include "appbase_menu.h"
#include <device/touch/XPT2046.h>
#include <device/display/layout.h>

class RunningTimer : public AppBaseMenu {
public:
	static const int QUEUE_SIZE = 10;
	static const int MSG_SIZE = sizeof(libesp::TouchNotification*);
	static const char *LOGTAG;
	RunningTimer();
	virtual ~RunningTimer();
protected:
	virtual libesp::ErrorType onInit();
	virtual libesp::BaseMenu::ReturnStateContext onRun();
	virtual libesp::ErrorType onShutdown();
private:
	QueueHandle_t InternalQueueHandler;
	libesp::StaticGridLayout MyLayout;
};

#endif
