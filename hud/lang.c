#include <string.h>
#include <stdio.h>
#include "main.h"
#include "lang.h"
#include "draw.h"

int readLine(SceUID fd, char* text, int len)
{
	if(fd >= 0 && text != NULL && len > 10) {
		unsigned char c = ' ', prev = ' ';
		int r = 0, i = 0;
		text[0] = 0;
		r = sceIoRead(fd, &c, 1);
		if(r == 0) return -1;
		while(r > 0 && i < len - 1) {
			if(c == '\r' || c == '\n') {
				if(prev == '\r' || prev == '\n') break;
			} else {
				text[i] = convertChar(c);
				text[i+1] = 0;
				i++;
			}
			prev = c;
			r = sceIoRead(fd, &c, 1);
		}
		return strlen(text);
	}
	return -1;
}

void getEnglishLang(LANG *lang)
{
	if(lang != NULL) {
		sprintf(lang->name, "ENGLISH");

		sprintf(lang->LANGUAGE, "LANGUAGE");
		sprintf(lang->SHOW, "SHOW");
		sprintf(lang->HUD, "HUD");
		sprintf(lang->FPS, "FPS");
		sprintf(lang->CPU, "CPU");
		sprintf(lang->CPU_NOTIFICATION, "CPU NOTIFICATION");
		sprintf(lang->BATTERY, "BATTERY");
		sprintf(lang->TIME, "TIME");
		sprintf(lang->ORDER, "ORDER");
		sprintf(lang->COMBO_BUTTON, "COMBO BUTTON");
		sprintf(lang->MENU_BUTTON, "MENU BUTTON");
		sprintf(lang->HUD_BUTTON, "HUD BUTTON");
		sprintf(lang->SCREENSHOT_BUTTON, "SCREENSHOT BUTTON");

		sprintf(lang->ON, "ON");
		sprintf(lang->OFF, "OFF");
		sprintf(lang->METHOD, "METHOD");
		sprintf(lang->FPS_NUMBER_ONLY, "NUMBER ONLY");
		sprintf(lang->FPS_PREFIX, "NUMBER WITH PREFIX");
		sprintf(lang->FPS_SUFFIX, "NUMBER WITH SUFFIX");
		sprintf(lang->CPU_SPEED, "CPU SPEED");
		sprintf(lang->CPU_BUS_SPEED, "CPU/BUS SPEED");
		sprintf(lang->CPU_USAGE, "CPU USAGE");
		sprintf(lang->CPU_SPEED_AND_USAGE, "CPU SPEED AND USAGE");
		sprintf(lang->CPU_BUS_SPEED_AND_USAGE, "CPU/BUS SPEED AND USAGE");
		sprintf(lang->PERCENT, "PERCENT");
		sprintf(lang->TIME_LEFT, "TIME LEFT");
		sprintf(lang->PERCENT_AND_TIME_LEFT, "PERCENT AND TIME LEFT");
		sprintf(lang->TIME_ICON_24H, "ICON/24H");
		sprintf(lang->TIME_ICON_12H_SUFFIX, "ICON/12H/SUFFIX");
		sprintf(lang->TIME_ICON_12H_NOSUFFIX, "ICON/12H");
		sprintf(lang->TIME_NOICON_24H, "24H");
		sprintf(lang->TIME_NOICON_12H_SUFFIX, "12H/SUFFIX");
		sprintf(lang->TIME_NOICON_12H_NOSUFFIX, "12H");

		sprintf(lang->XMB, "XMB");
		sprintf(lang->GAME, "GAME");
		sprintf(lang->POPS, "POPS");
		sprintf(lang->CLOSE, "CLOSE AND SAVE");
		sprintf(lang->ANALOG, "ANALOG");
		sprintf(lang->CHANGE_SELECTION, "CHANGE SELECTION");
		sprintf(lang->CHANGE_OPTION, "CHANGE OPTION");
		sprintf(lang->CHANGE_ALIGNMENT, "CHANGE ALIGNMENT");
		sprintf(lang->CHANGE_ALIGNMENT_FOR_ALL, "CHANGE ALIGNMENT FOR ALL");
		sprintf(lang->CHANGE_CUSTOM_POSITION, "CHANGE CUSTOM POSITION");
		sprintf(lang->CHANGE_ORDER, "CHANGE ORDER");
		sprintf(lang->RESET_ORDER, "RESET ORDER");

		sprintf(lang->CTRL_NONE, "NONE");
		sprintf(lang->CTRL_SELECT, "SELECT");
		sprintf(lang->CTRL_START, "START");
		sprintf(lang->CTRL_UP, "UP");
		sprintf(lang->CTRL_RIGHT, "RIGHT");
		sprintf(lang->CTRL_DOWN, "DOWN");
		sprintf(lang->CTRL_LEFT, "LEFT");
		sprintf(lang->CTRL_LTRIGGER, "LTRIGGER");
		sprintf(lang->CTRL_RTRIGGER, "RTRIGGER");
		sprintf(lang->CTRL_TRIANGLE, "TRIANGLE");
		sprintf(lang->CTRL_CIRCLE, "CIRCLE");
		sprintf(lang->CTRL_CROSS, "CROSS");
		sprintf(lang->CTRL_SQUARE, "SQUARE");
		sprintf(lang->CTRL_HOME, "HOME");
		sprintf(lang->CTRL_NOTE, "NOTE");
		sprintf(lang->CTRL_SCREEN, "SCREEN");
		sprintf(lang->CTRL_VOLUP, "VOLUP");
		sprintf(lang->CTRL_VOLDOWN, "VOLDOWN");
	}
}

void readLang(SceUID fd, LANG *lang)
{
	if(fd >= 0 && lang != NULL) {

		readLine(fd, lang->LANGUAGE, MAX_LANG_TEXT);
		readLine(fd, lang->SHOW, MAX_LANG_TEXT);
		readLine(fd, lang->HUD, MAX_LANG_TEXT);
		readLine(fd, lang->FPS, MAX_LANG_TEXT);
		readLine(fd, lang->CPU, MAX_LANG_TEXT);
		readLine(fd, lang->CPU_NOTIFICATION, MAX_LANG_TEXT);
		readLine(fd, lang->BATTERY, MAX_LANG_TEXT);
		readLine(fd, lang->TIME, MAX_LANG_TEXT);
		readLine(fd, lang->ORDER, MAX_LANG_TEXT);
		readLine(fd, lang->COMBO_BUTTON, MAX_LANG_TEXT);
		readLine(fd, lang->MENU_BUTTON, MAX_LANG_TEXT);
		readLine(fd, lang->HUD_BUTTON, MAX_LANG_TEXT);
		readLine(fd, lang->SCREENSHOT_BUTTON, MAX_LANG_TEXT);

		readLine(fd, lang->ON, MAX_LANG_TEXT);
		readLine(fd, lang->OFF, MAX_LANG_TEXT);
		readLine(fd, lang->METHOD, MAX_LANG_TEXT);
		readLine(fd, lang->FPS_NUMBER_ONLY, MAX_LANG_TEXT);
		readLine(fd, lang->FPS_PREFIX, MAX_LANG_TEXT);
		readLine(fd, lang->FPS_SUFFIX, MAX_LANG_TEXT);
		readLine(fd, lang->CPU_SPEED, MAX_LANG_TEXT);
		readLine(fd, lang->CPU_BUS_SPEED, MAX_LANG_TEXT);
		readLine(fd, lang->CPU_USAGE, MAX_LANG_TEXT);
		readLine(fd, lang->CPU_SPEED_AND_USAGE, MAX_LANG_TEXT);
		readLine(fd, lang->CPU_BUS_SPEED_AND_USAGE, MAX_LANG_TEXT);
		readLine(fd, lang->PERCENT, MAX_LANG_TEXT);
		readLine(fd, lang->TIME_LEFT, MAX_LANG_TEXT);
		readLine(fd, lang->PERCENT_AND_TIME_LEFT, MAX_LANG_TEXT);
		readLine(fd, lang->TIME_ICON_24H, MAX_LANG_TEXT);
		readLine(fd, lang->TIME_ICON_12H_SUFFIX, MAX_LANG_TEXT);
		readLine(fd, lang->TIME_ICON_12H_NOSUFFIX, MAX_LANG_TEXT);
		readLine(fd, lang->TIME_NOICON_24H, MAX_LANG_TEXT);
		readLine(fd, lang->TIME_NOICON_12H_SUFFIX, MAX_LANG_TEXT);
		readLine(fd, lang->TIME_NOICON_12H_NOSUFFIX, MAX_LANG_TEXT);

		readLine(fd, lang->XMB, MAX_LANG_TEXT);
		readLine(fd, lang->GAME, MAX_LANG_TEXT);
		readLine(fd, lang->POPS, MAX_LANG_TEXT);
		readLine(fd, lang->CLOSE, MAX_LANG_TEXT);
		readLine(fd, lang->ANALOG, MAX_LANG_TEXT);
		readLine(fd, lang->CHANGE_SELECTION, MAX_LANG_TEXT);
		readLine(fd, lang->CHANGE_OPTION, MAX_LANG_TEXT);
		readLine(fd, lang->CHANGE_ALIGNMENT, MAX_LANG_TEXT);
		readLine(fd, lang->CHANGE_ALIGNMENT_FOR_ALL, MAX_LANG_TEXT);
		readLine(fd, lang->CHANGE_CUSTOM_POSITION, MAX_LANG_TEXT);
		readLine(fd, lang->CHANGE_ORDER, MAX_LANG_TEXT);
		readLine(fd, lang->RESET_ORDER, MAX_LANG_TEXT);

		readLine(fd, lang->CTRL_NONE, MAX_LANG_TEXT);
		readLine(fd, lang->CTRL_SELECT, MAX_LANG_TEXT);
		readLine(fd, lang->CTRL_START, MAX_LANG_TEXT);
		readLine(fd, lang->CTRL_UP, MAX_LANG_TEXT);
		readLine(fd, lang->CTRL_RIGHT, MAX_LANG_TEXT);
		readLine(fd, lang->CTRL_DOWN, MAX_LANG_TEXT);
		readLine(fd, lang->CTRL_LEFT, MAX_LANG_TEXT);
		readLine(fd, lang->CTRL_LTRIGGER, MAX_LANG_TEXT);
		readLine(fd, lang->CTRL_RTRIGGER, MAX_LANG_TEXT);
		readLine(fd, lang->CTRL_TRIANGLE, MAX_LANG_TEXT);
		readLine(fd, lang->CTRL_CIRCLE, MAX_LANG_TEXT);
		readLine(fd, lang->CTRL_CROSS, MAX_LANG_TEXT);
		readLine(fd, lang->CTRL_SQUARE, MAX_LANG_TEXT);
		readLine(fd, lang->CTRL_HOME, MAX_LANG_TEXT);
		readLine(fd, lang->CTRL_NOTE, MAX_LANG_TEXT);
		readLine(fd, lang->CTRL_SCREEN, MAX_LANG_TEXT);
		readLine(fd, lang->CTRL_VOLUP, MAX_LANG_TEXT);
		readLine(fd, lang->CTRL_VOLDOWN, MAX_LANG_TEXT);
	}
}

int loadLang(LANG *lang, int index)
{
	if(lang != NULL) {

		if(index > 0) {
			SceUID fd = sceIoOpen("ms0:/seplugins/hud.lang", PSP_O_RDONLY, 0777);
			if(fd >= 0) {
				char text[255];
				int count = 0;
				int i = 0;
				while(readLine(fd, text, 255) >= 0) {
					int len = strlen(text);
					if(len > 0 && text[0] == '[') {
						for(i = 0; i < len; i++) {
							if(text[i] == ']') { text[i] = 0; break; }
						}
						char *temp = &text[i];
						if(strlen(temp) < MAX_LANG_TEXT) {
							count++;
							if(count == index) {
								lang->name[0] = 0;
								strcat(lang->name, &text[1]);
								readLang(fd, lang);
								return index;
							}
						}
					}
				}
				sceIoClose(fd);
			}
		}
	}
	getEnglishLang(lang);
	return 0;
}
