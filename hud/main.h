#ifndef __MAIN_H__
#define __MAIN_H__

#include <pspctrl.h>
#include <pspkernel.h>
#include <psprtc.h>

int sceCtrl_driver_PeekBufferPositive(SceCtrlData *pad_data, int count);
int sceCtrl_driver_ReadBufferPositive(SceCtrlData *pad_data, int count);
int sceCtrl_driver_PeekBufferNegative(SceCtrlData *pad_data, int count);
int sceCtrl_driver_ReadBufferNegative(SceCtrlData *pad_data, int count);

int sceCtrl_driver_SetSamplingMode(int mode);
int sceCtrl_driver_GetSamplingMode(int *pmode);
void sceCtrl_driver_7CA723DC(unsigned int mask, unsigned type);

int sceDisplay_driver_GetFrameBuf(void **topaddr, int *bufferwidth, int *pixelformat, int sync);
int sceDisplay_driver_SetFrameBuf(void *topaddr, int bufferwidth, int pixelformat, int sync);
int sceDisplay_driver_GetMode(int *pmode, int *pwidth, int *pheight);
int sceDisplay_driver_WaitVblankStart(void);
float sceDisplay_driver_GetFramePerSec(void);

int scePower_driver_GetCpuClockFrequencyInt(void);
int scePower_driver_GetBusClockFrequencyInt(void);
int scePower_driver_SetClockFrequency(int pllfreq, int cpufreq, int busfreq);
int scePower_driver_GetBatteryLifeTime(void);
int scePower_driver_GetBatteryLifePercent(void);

int sceRtc_driver_GetCurrentClockLocalTime(pspTime *time);

/*
int sceUsb_driver_Start(const char* driverName, int size, void *args);
int sceUsb_driver_Stop(const char* driverName, int size, void *args);
int sceUsb_driver_Activate(u32 pid);
int sceUsb_driver_Deactivate(u32 pid);
int sceUsb_driver_GetState(void);
*/

#endif
