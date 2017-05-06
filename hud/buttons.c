#include <psptypes.h>
#include <pspctrl.h>
#include "buttons.h"
#include "lang.h"
#include <string.h>

char* getButtonName(unsigned int button, LANG* lang)
{
	if(lang == NULL) {
		switch(button) {
			case 0:                 return "NONE";
			case PSP_CTRL_SELECT:   return "SELECT";
			case PSP_CTRL_START:    return "START";
			case PSP_CTRL_UP:       return "UP";
			case PSP_CTRL_RIGHT:    return "RIGHT";
			case PSP_CTRL_DOWN:     return "DOWN";
			case PSP_CTRL_LEFT:     return "LEFT";
			case PSP_CTRL_LTRIGGER: return "LTRIGGER";
			case PSP_CTRL_RTRIGGER: return "RTRIGGER";
			case PSP_CTRL_TRIANGLE: return "TRIANGLE";
			case PSP_CTRL_CIRCLE:   return "CIRCLE";
			case PSP_CTRL_CROSS:    return "CROSS";
			case PSP_CTRL_SQUARE:   return "SQUARE";
			case PSP_CTRL_HOME:     return "HOME";
			case PSP_CTRL_NOTE:     return "NOTE";
			case PSP_CTRL_SCREEN:   return "SCREEN";
			case PSP_CTRL_VOLUP:    return "VOLUP";
			case PSP_CTRL_VOLDOWN:  return "VOLDOWN";
		}
	} else {
		switch(button) {
			case 0:                 return lang->CTRL_NONE;
			case PSP_CTRL_SELECT:   return lang->CTRL_SELECT;
			case PSP_CTRL_START:    return lang->CTRL_START;
			case PSP_CTRL_UP:       return lang->CTRL_UP;
			case PSP_CTRL_RIGHT:    return lang->CTRL_RIGHT;
			case PSP_CTRL_DOWN:     return lang->CTRL_DOWN;
			case PSP_CTRL_LEFT:     return lang->CTRL_LEFT;
			case PSP_CTRL_LTRIGGER: return lang->CTRL_LTRIGGER;
			case PSP_CTRL_RTRIGGER: return lang->CTRL_RTRIGGER;
			case PSP_CTRL_TRIANGLE: return lang->CTRL_TRIANGLE;
			case PSP_CTRL_CIRCLE:   return lang->CTRL_CIRCLE;
			case PSP_CTRL_CROSS:    return lang->CTRL_CROSS;
			case PSP_CTRL_SQUARE:   return lang->CTRL_SQUARE;
			case PSP_CTRL_HOME:     return lang->CTRL_HOME;
			case PSP_CTRL_NOTE:     return lang->CTRL_NOTE;
			case PSP_CTRL_SCREEN:   return lang->CTRL_SCREEN;
			case PSP_CTRL_VOLUP:    return lang->CTRL_VOLUP;
			case PSP_CTRL_VOLDOWN:  return lang->CTRL_VOLDOWN;
		}
	}
	return "UNKNOWN";
}

unsigned int getPrevButton(unsigned int button)
{
	switch(button) {
		case 0:                 return PSP_CTRL_VOLDOWN;
		case PSP_CTRL_SELECT:   return 0;
		case PSP_CTRL_START:    return PSP_CTRL_SELECT;
		case PSP_CTRL_UP:       return PSP_CTRL_START;
		case PSP_CTRL_RIGHT:    return PSP_CTRL_UP;
		case PSP_CTRL_DOWN:     return PSP_CTRL_RIGHT;
		case PSP_CTRL_LEFT:     return PSP_CTRL_DOWN;
		case PSP_CTRL_LTRIGGER: return PSP_CTRL_LEFT;
		case PSP_CTRL_RTRIGGER: return PSP_CTRL_LTRIGGER;
		case PSP_CTRL_TRIANGLE: return PSP_CTRL_RTRIGGER;
		case PSP_CTRL_CIRCLE:   return PSP_CTRL_TRIANGLE;
		case PSP_CTRL_CROSS:    return PSP_CTRL_CIRCLE;
		case PSP_CTRL_SQUARE:   return PSP_CTRL_CROSS;
		case PSP_CTRL_HOME:     return PSP_CTRL_SQUARE;
		case PSP_CTRL_NOTE:     return PSP_CTRL_HOME;
		case PSP_CTRL_SCREEN:   return PSP_CTRL_NOTE;
		case PSP_CTRL_VOLUP:    return PSP_CTRL_SCREEN;
		case PSP_CTRL_VOLDOWN:  return PSP_CTRL_VOLUP;
	}
	return PSP_CTRL_SELECT;
}

unsigned int getNextButton(unsigned int button)
{
	switch(button) {
		case 0:                 return PSP_CTRL_SELECT;
		case PSP_CTRL_SELECT:   return PSP_CTRL_START;
		case PSP_CTRL_START:    return PSP_CTRL_UP;
		case PSP_CTRL_UP:       return PSP_CTRL_RIGHT;
		case PSP_CTRL_RIGHT:    return PSP_CTRL_DOWN;
		case PSP_CTRL_DOWN:     return PSP_CTRL_LEFT;
		case PSP_CTRL_LEFT:     return PSP_CTRL_LTRIGGER;
		case PSP_CTRL_LTRIGGER: return PSP_CTRL_RTRIGGER;
		case PSP_CTRL_RTRIGGER: return PSP_CTRL_TRIANGLE;
		case PSP_CTRL_TRIANGLE: return PSP_CTRL_CIRCLE;
		case PSP_CTRL_CIRCLE:   return PSP_CTRL_CROSS;
		case PSP_CTRL_CROSS:    return PSP_CTRL_SQUARE;
		case PSP_CTRL_SQUARE:   return PSP_CTRL_HOME;
		case PSP_CTRL_HOME:     return PSP_CTRL_NOTE;
		case PSP_CTRL_NOTE:     return PSP_CTRL_SCREEN;
		case PSP_CTRL_SCREEN:   return PSP_CTRL_VOLUP;
		case PSP_CTRL_VOLUP:    return PSP_CTRL_VOLDOWN;
		case PSP_CTRL_VOLDOWN:  return 0;
	}
	return PSP_CTRL_SELECT;
}

/*
int getButtonsPressed(SceCtrlData *pad, char *text)
{
	text[0] = 0;
	if(pad != NULL) {
		int buttons = pad->Buttons;
		if(buttons & PSP_CTRL_SELECT  ) strcat(text, "SELECT, "  );
		if(buttons & PSP_CTRL_START   ) strcat(text, "START, "   );
		if(buttons & PSP_CTRL_UP      ) strcat(text, "UP, "      );
		if(buttons & PSP_CTRL_RIGHT   ) strcat(text, "RIGHT, "   );
		if(buttons & PSP_CTRL_DOWN    ) strcat(text, "DOWN, "    );
		if(buttons & PSP_CTRL_LEFT    ) strcat(text, "LEFT, "    );
		if(buttons & PSP_CTRL_LTRIGGER) strcat(text, "LTRIGGER, ");
		if(buttons & PSP_CTRL_RTRIGGER) strcat(text, "RTRIGGER, ");
		if(buttons & PSP_CTRL_TRIANGLE) strcat(text, "TRIANGLE, ");
		if(buttons & PSP_CTRL_CIRCLE  ) strcat(text, "CIRCLE, "  );
		if(buttons & PSP_CTRL_CROSS   ) strcat(text, "CROSS, "   );
		if(buttons & PSP_CTRL_SQUARE  ) strcat(text, "SQUARE, "  );
		if(buttons & PSP_CTRL_HOME    ) strcat(text, "HOME, "    );
		if(buttons & PSP_CTRL_NOTE    ) strcat(text, "NOTE, "    );
		if(buttons & PSP_CTRL_SCREEN  ) strcat(text, "SCREEN, "  );
		if(buttons & PSP_CTRL_VOLUP   ) strcat(text, "VOLUP, "   );
		if(buttons & PSP_CTRL_VOLDOWN ) strcat(text, "VOLDOWN, " );
	}
	int len = strlen(text);
	if(len > 1) text[len-2] = 0;
	return 0;
}
*/
