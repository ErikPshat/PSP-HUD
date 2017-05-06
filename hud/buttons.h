#ifndef __BUTTONS_H__
#define __BUTTONS_H__
#include <pspctrl.h>
#include "lang.h"

char* getButtonName(unsigned int button, LANG* lang);
unsigned int getPrevButton(unsigned int button);
unsigned int getNextButton(unsigned int button);
//int getButtonsPressed(SceCtrlData *pad, char *text);

#endif
