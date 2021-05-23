#ifndef TIMER_STATE_H
#define TIMER_STATE_H

#include "appbase_menu.h"
#include <device/touch/XPT2046.h>
#include <device/display/layout.h>

class TimerMenu: public AppBaseMenu {
public:
	static const int QUEUE_SIZE = 10;
	static const int MSG_SIZE = sizeof(libesp::TouchNotification*);
	static const char *LOGTAG;
	TimerMenu();
	int64_t getTimerMS();
	virtual ~TimerMenu();
protected:
	virtual libesp::ErrorType onInit();
	virtual libesp::BaseMenu::ReturnStateContext onRun();
	virtual libesp::ErrorType onShutdown();
private:
	QueueHandle_t InternalQueueHandler;
	libesp::StaticGridLayout MyLayout;
};


#endif
