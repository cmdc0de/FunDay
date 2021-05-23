/*
 * gui_list_processor.cpp
 *
 *      Author: cmdc0de
 */

#include "gui_list_processor.h"
#include <device/display/gui.h>
#include "../app.h"
#include <device/display/display_device.h>
#include <esp_log.h>

using namespace libesp;
static const char *LOGTAG = "GUIListProcessor";

int32_t GUIListProcessor::process(libesp::Point2Ds &p, libesp::GUIListData *pl, uint16_t itemC) {
	uint16_t yMin = pl->y;
	uint16_t yMax = yMin+pl->h;
	if(p.getY()<yMin||p.getY()>yMax) return false;
	if(p.getX()<pl->x||p.getX()>(pl->x+pl->w)) return false;
	uint16_t currentY = pl->y;
	uint16_t perRow = MyApp::get().getDisplay().getFont()->FontHeight;
	//skip over header
	if(pl->header) {
		currentY+=perRow;
		if(p.getY()<currentY) {
			return GUI_HEADER_HIT;
		}
	}
	for(int i=0;i<itemC;++i,currentY+=perRow) {
		//ESP_LOGI(LOGTAG,"%d %d",int32_t(currentY),int32_t(currentY+perRow));
		if(p.getY()>=currentY && p.getY()<(currentY+perRow)) {
			if(p.getX()>=pl->x&&p.getX()<(pl->x+pl->w)) {
				pl->selectedItem = i;
				return GUI_ITEM_HIT;
			}
			return NO_GUI_ITEM_HIT;
		}
	}
	return NO_GUI_ITEM_HIT;
}


