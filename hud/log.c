#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void logAppendLine(char* text)
{
	SceUID fd = sceIoOpen("ms0:/log.txt", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_APPEND, 0777);
	if(fd >= 0) {
		pspTime time;
		sceRtc_driver_GetCurrentClockLocalTime(&time);
		char stime[32];
		sprintf(stime, "%02i:%02i.%02i  : ", time.hour, time.minutes, (int)(time.microseconds / 10000));
		sceIoWrite(fd, stime, strlen(stime));
		sceIoWrite(fd, text, strlen(text));
		sceIoWrite(fd, "\r\n", 2);
		sceIoClose (fd);
	}
}
