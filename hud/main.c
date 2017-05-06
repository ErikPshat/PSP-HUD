#include <pspsdk.h>
#include <pspkernel.h>
#include <pspinit.h>
#include <pspctrl.h>
#include <pspmoduleinfo.h>
//#include <pspsysmem.h>
#include <stdio.h>
#include <string.h>
#include "apihook.h"
#include "screenshot.h"
#include "main.h"
#include "buttons.h"
#include "config.h"
#include "draw.h"
#include "lang.h"
//#include "usb.h"

PSP_MODULE_INFO("PSP-HUD", PSP_MODULE_KERNEL, 1, 0);
PSP_MAIN_THREAD_ATTR(0);
PSP_MAIN_THREAD_STACK_SIZE_KB(0);

#define MAX(X,Y) ((X)>(Y)?(X):(Y))
#define MIN(X,Y) ((X)<(Y)?(X):(Y))
#define SWITCH(X) X=(X+1)&1
#define SWAP(X,Y,TEMP) TEMP=X,X=Y,Y=TEMP

#define PSP_INIT_KEYCONFIG_VSH  0x100
#define PSP_INIT_KEYCONFIG_GAME 0x200
#define PSP_INIT_KEYCONFIG_POPS 0x300

u32 WHITE = RGB(255, 255, 255);
u32 SELECTION_FILL = RGB(200, 200, 0);
u32 SELECTION_BORDER = RGB(0, 0, 0);
u32 ORDER_SELECTION_FILL = RGB(100, 200, 0);

CONFIG cfg;
LANG lang;
CANVAS displayCanvas;
char text[255];
int initKeyConfig = 0;
int drawing_mode = 0; // 0 - main thread, 1 - display hook
int wait = 0;


#define MAX_SPEEDS 9
#define DEFAULT_MENU_SPEED 5
int CPU_SPEED[MAX_SPEEDS] = { 80, 80, 100, 133, 166, 222, 266, 300, 333 };
int BUS_SPEED[MAX_SPEEDS] = { 40, 40,  50,  66,  83, 111, 133, 150, 166 };

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  CPU UPSAGE CALCULATION
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/*
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
*/

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  CPU UPSAGE CALCULATION
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

SceUInt32 cpu_last_clock = 0, cpu_last_idle = 0;
int last_cpu_usage = -1;
int getCpuUsage()
{
	int cpu_usage = last_cpu_usage;
	SceKernelSysClock clock;
	sceKernelGetSystemTime(&clock);
	SceKernelSystemStatus status;
	status.size = sizeof(SceKernelSystemStatus);
	sceKernelReferSystemStatus(&status);
	if((clock.low - cpu_last_clock) >= 1000000) {
		if(cpu_last_clock > 0 && cpu_last_idle > 0 && cpu_last_clock < clock.low && cpu_last_idle < status.idleClocks.low) {
			int el_clock = clock.low - cpu_last_clock;
			int el_idle = status.idleClocks.low - cpu_last_idle;
			if(el_clock > 0) cpu_usage = (int)(100.0f - ((float)el_idle / (float)el_clock) * 100.0f);
		}
		cpu_last_clock = clock.low;
		cpu_last_idle = status.idleClocks.low;
	}
	if(cpu_usage > 100) cpu_usage = 100;
	last_cpu_usage = cpu_usage;
	return cpu_usage;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  FPS CALCULATION
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

SceUInt32 fps_last_clock = 0;
int fps_counter = 0;
int fps_last_counter = 0;
int last_fps = -1;
int getFPS()
{
	int current_fps = last_fps;
	SceKernelSysClock clock;
	sceKernelGetSystemTime(&clock);
	if((clock.low - fps_last_clock) >= 1000000) {
		if(fps_last_clock > 0 && fps_last_clock < clock.low) {
			int el_clock = clock.low - fps_last_clock;
			if(el_clock > 0) current_fps = (int)((fps_counter - fps_last_counter) * 1000000 / el_clock);
		}
		fps_last_clock = clock.low;
		fps_last_counter = fps_counter;
	}
	last_fps = current_fps;
	return current_fps;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  WLAN HOOKING
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/*

int wlan_hooked = 0;

int hook_counter1 = 0;
int hook_counter2 = 0;
int hook_counter3 = 0;
int hook_counter4 = 0;

typedef uint8_t sa_family_t;
typedef uint32_t socklen_t;

struct sockaddr {
	uint8_t	sa_len;
	sa_family_t	sa_family;
	char		sa_data[14];
};

size_t	(*sceNetInetRecv)(int s, void *buf, size_t len, int flags);
size_t	(*sceNetInetRecvfrom)(int s, void *buf, size_t flags, int unk, struct sockaddr *from, socklen_t *fromlen);
size_t	(*sceNetInetSend)(int s, const void *buf, size_t len, int flags);
size_t	(*sceNetInetSendto)(int s, const void *buf, size_t len, int flags, const struct sockaddr *to, socklen_t tolen);

size_t	net_inet_recv(int s, void *buf, size_t len, int flags)
{
	hook_counter1++;
	if(sceNetInetRecv != NULL) return sceNetInetRecv(s, buf, len, flags);
	return 0;
}
size_t	net_inet_recvfrom(int s, void *buf, size_t flags, int unk, struct sockaddr *from, socklen_t *fromlen)
{
	hook_counter2++;
	if(sceNetInetRecvfrom != NULL) return sceNetInetRecvfrom(s, buf, flags, unk, from, fromlen);
	return 0;
}
size_t	net_inet_send(int s, const void *buf, size_t len, int flags)
{
	hook_counter3++;
	if(sceNetInetSend != NULL) return sceNetInetSend(s, buf, len, flags);
	return 0;
}
size_t	net_inet_sendto(int s, const void *buf, size_t len, int flags, const struct sockaddr *to, socklen_t tolen)
{
	hook_counter4++;
	if(sceNetInetSendto != NULL) return sceNetInetSendto(s, buf, len, flags, to, tolen);
	return 0;
}

int loadStartModule(char *path)
{
    u32 loadResult;
    u32 startResult;
    int status;

    loadResult = sceKernelLoadModule(path, 0, NULL);
    if (loadResult & 0x80000000)
	return -1;
    else
	startResult =
	    sceKernelStartModule(loadResult, 0, NULL, &status, NULL);

    if (loadResult != startResult)
	return -2;

    return 0;
}

void hookWlan()
{
    loadStartModule( "flash0:/kd/ifhandle.prx" );
    loadStartModule( "flash0:/kd/pspnet.prx" );
    loadStartModule( "flash0:/kd/pspnet_inet.prx" );
    loadStartModule( "flash0:/kd/pspnet_apctl.prx" );
	loadStartModule( "flash0:/kd/pspnet_resolver.prx" );

	SceModule *pMod = sceKernelFindModuleByName("sceNetInet_Library");
	if(pMod != NULL) {
		saveLibraryEntryTableInfo(pMod->modid, "ms0:/sceNetInet_log.txt");

		sceNetInetRecv     = (void*)apiHookByName(pMod->modid, "sceNetInet", "sceNetInetRecv",     net_inet_recv);
		sceNetInetRecvfrom = (void*)apiHookByName(pMod->modid, "sceNetInet", "sceNetInetRecvfrom", net_inet_recvfrom);
		sceNetInetSend     = (void*)apiHookByName(pMod->modid, "sceNetInet", "sceNetInetSend",     net_inet_send);
		sceNetInetSendto   = (void*)apiHookByName(pMod->modid, "sceNetInet", "sceNetInetSendto",   net_inet_sendto);

		if(sceNetInetRecv     != 0) wlan_hooked++;
		if(sceNetInetRecvfrom != 0) wlan_hooked++;
		if(sceNetInetSend     != 0) wlan_hooked++;
		if(sceNetInetSendto   != 0) wlan_hooked++;
	}

}

*/

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  CONTROLLER HOOKING
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int controller_hooked = 0;
int disable_controller = 0;
int (*vshCtrlReadBufferPositive)(SceCtrlData *pad_data, int count);

int read_buffer_positive(SceCtrlData *pad_data, int count)
{
	if(disable_controller == 1) return 0;
	return sceCtrl_driver_ReadBufferPositive(pad_data, count);
}

int peek_buffer_positive(SceCtrlData *pad_data, int count)
{
	if(disable_controller == 1) return 0;
	return sceCtrl_driver_PeekBufferPositive(pad_data, count);
}

int read_buffer_negative(SceCtrlData *pad_data, int count)
{
	if(disable_controller == 1) return 0;
	return sceCtrl_driver_ReadBufferNegative(pad_data, count);
}

int peek_buffer_negative(SceCtrlData *pad_data, int count)
{
	if(disable_controller == 1) return 0;
	return sceCtrl_driver_PeekBufferNegative(pad_data, count);
}

int vsh_read_buffer_positive(SceCtrlData *pad_data, int count)
{
	if(disable_controller == 1) return 0;
	if(vshCtrlReadBufferPositive != NULL) return vshCtrlReadBufferPositive(pad_data, count);
	return sceCtrl_driver_ReadBufferPositive(pad_data, count);
}

void hookController()
{
	SceModule *pMod1 = sceKernelFindModuleByName("sceController_Service");
	if(pMod1 != NULL) {
		//saveLibraryEntryTableInfo(pMod1->modid, "ms0:/sceController_log.txt");

		if(apiHookByName(pMod1->modid, "sceCtrl", "sceCtrlReadBufferPositive", read_buffer_positive) != 0) controller_hooked++;
		if(apiHookByName(pMod1->modid, "sceCtrl", "sceCtrlPeekBufferPositive", peek_buffer_positive) != 0) controller_hooked++;
		if(apiHookByName(pMod1->modid, "sceCtrl", "sceCtrlReadBufferNegative", read_buffer_negative) != 0) controller_hooked++;
		if(apiHookByName(pMod1->modid, "sceCtrl", "sceCtrlPeekBufferNegative", peek_buffer_negative) != 0) controller_hooked++;
	}

	SceModule *pMod2 = sceKernelFindModuleByName("sceVshBridge_Driver");
	if(pMod2 != NULL) {
		//saveLibraryEntryTableInfo(pMod2->modid, "ms0:/sceVshBridge_log.txt");

		vshCtrlReadBufferPositive = (void*)apiHookByName(pMod2->modid, "sceVshBridge","vshCtrlReadBufferPositive", vsh_read_buffer_positive);
		if(vshCtrlReadBufferPositive != NULL) controller_hooked++;
	}
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  MENU NAVIGATION
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

SceCtrlData pad;
int pad_old = 0;
int showMenu = 0;
int selection = 0;
int order_selection = 0;
int display_width = 480;
int display_height = 272;
int display_max_x[4] = { 480, 480, 480, 480 };
int display_max_y[4] = { 272, 272, 272, 272 };
int menu_min_x = -1;
int screenshot_timer = -1;

void menuNavigation()
{
	int infoIndex = -1;
	if(selection == 2) infoIndex = 0;
	else if(selection == 3 || selection == 4) infoIndex = 1;
	else if(selection == 5) infoIndex = 2;
	else if(selection == 6) infoIndex = 3;

	sceCtrl_driver_PeekBufferPositive(&pad, 1);

	//
	// handle pressed buttons
	//
	if(pad.Buttons != pad_old) {

		if(showMenu == 0 && (cfg.buttonCombo != 0 || cfg.buttonMenu != 0) && (cfg.buttonCombo == 0 || (pad.Buttons & cfg.buttonCombo)) && (cfg.buttonMenu == 0 || (pad.Buttons & cfg.buttonMenu))) {
			showMenu = 1; 
			selection = 0;
			disable_controller = 1;
		} else if(showMenu == 0 && (cfg.buttonCombo != 0 || cfg.buttonHUD != 0) && (cfg.buttonCombo == 0 || (pad.Buttons & cfg.buttonCombo)) && (cfg.buttonHUD == 0 || (pad.Buttons & cfg.buttonHUD))) {
			cfg.showHUD++;
			if(cfg.showHUD > 2) cfg.showHUD = 0;
		} else if(showMenu == 0 && (cfg.buttonCombo != 0 || cfg.buttonScreenshot != 0) && (cfg.buttonCombo == 0 || (pad.Buttons & cfg.buttonCombo)) && (cfg.buttonScreenshot == 0 || (pad.Buttons & cfg.buttonScreenshot))) {
			screenshot_timer = 10;
		} else if(showMenu == 1 && (pad.Buttons & PSP_CTRL_UP)) {
			selection--; 
			if(selection < 0) selection = 15;
		} else if(showMenu == 1 && (pad.Buttons & PSP_CTRL_DOWN)) {
			selection++; 
			if(selection > 15) selection = 0;
		} else if(showMenu == 1 && ((pad.Buttons & PSP_CTRL_LEFT) || (pad.Buttons & PSP_CTRL_RIGHT))) {
			switch(selection) {
				case 0:
					if(pad.Buttons & PSP_CTRL_LEFT) cfg.language--;
					if(pad.Buttons & PSP_CTRL_RIGHT) cfg.language++;
					cfg.language = loadLang(&lang, cfg.language);
					menu_min_x = -1;
					break;
				case 1:
					if(pad.Buttons & PSP_CTRL_LEFT) cfg.showHUD--;
					if(pad.Buttons & PSP_CTRL_RIGHT) cfg.showHUD++;
					if(cfg.showHUD < 0) cfg.showHUD = 2;
					if(cfg.showHUD > 2) cfg.showHUD = 0;
					break;
				case 2:
					if(pad.Buttons & PSP_CTRL_LEFT) cfg.showFPS--;
					if(pad.Buttons & PSP_CTRL_RIGHT) cfg.showFPS++;
					if(cfg.showFPS < 0) cfg.showFPS = 3;
					if(cfg.showFPS > 3) cfg.showFPS = 0;
					break;
				case 3:
					if(pad.Buttons & PSP_CTRL_LEFT) cfg.showCPU--;
					if(pad.Buttons & PSP_CTRL_RIGHT) cfg.showCPU++;
					if(cfg.showCPU < 0) cfg.showCPU = 5;
					if(cfg.showCPU > 5) cfg.showCPU = 0;
					break;
				case 4:
					if(pad.Buttons & PSP_CTRL_LEFT) cfg.showCPUNotofication--;
					if(pad.Buttons & PSP_CTRL_RIGHT) cfg.showCPUNotofication++;
					if(cfg.showCPUNotofication < 0) cfg.showCPUNotofication = 2;
					if(cfg.showCPUNotofication > 2) cfg.showCPUNotofication = 0;
					break;
				case 5:
					if(pad.Buttons & PSP_CTRL_LEFT) cfg.showBattery--;
					if(pad.Buttons & PSP_CTRL_RIGHT) cfg.showBattery++;
					if(cfg.showBattery < 0) cfg.showBattery = 3;
					if(cfg.showBattery > 3) cfg.showBattery = 0;
					break;
				case 6:
					if(pad.Buttons & PSP_CTRL_LEFT) cfg.showTime--;
					if(pad.Buttons & PSP_CTRL_RIGHT) cfg.showTime++;
					if(cfg.showTime < 0) cfg.showTime = 6;
					if(cfg.showTime > 6) cfg.showTime = 0;
					break;
				case 7:
					if(pad.Buttons & PSP_CTRL_CROSS) {
						int temp = 0;
						if((pad.Buttons & PSP_CTRL_LEFT) && order_selection > 0) {
							SWAP(cfg.order[order_selection], cfg.order[order_selection-1], temp);
							order_selection--;
						} else if((pad.Buttons & PSP_CTRL_RIGHT) && order_selection < 3) {
							SWAP(cfg.order[order_selection], cfg.order[order_selection+1], temp);
							order_selection++;
						}
					} else {
						if((pad.Buttons & PSP_CTRL_LEFT) && order_selection > 0) order_selection--;
						if((pad.Buttons & PSP_CTRL_RIGHT)  && order_selection < 3) order_selection++;
					}					
					break;
				case 8:
					if((pad.Buttons & PSP_CTRL_LEFT) && cfg.speedXMB > 0) cfg.speedXMB--;
					if((pad.Buttons & PSP_CTRL_RIGHT)&& cfg.speedXMB < (MAX_SPEEDS - 1)) cfg.speedXMB++;
					break;
				case 9:
					if((pad.Buttons & PSP_CTRL_LEFT) && cfg.speedGAME > 0) cfg.speedGAME--;
					if((pad.Buttons & PSP_CTRL_RIGHT)&& cfg.speedGAME < (MAX_SPEEDS - 1)) cfg.speedGAME++;
					break;
				case 10:
					if((pad.Buttons & PSP_CTRL_LEFT) && cfg.speedPOPS > 0) cfg.speedPOPS--;
					if((pad.Buttons & PSP_CTRL_RIGHT)&& cfg.speedPOPS < (MAX_SPEEDS - 1)) cfg.speedPOPS++;
					break;
				case 11:
					if(pad.Buttons & PSP_CTRL_LEFT) cfg.buttonCombo = getPrevButton(cfg.buttonCombo);
					if(pad.Buttons & PSP_CTRL_RIGHT) cfg.buttonCombo = getNextButton(cfg.buttonCombo);
					break;
				case 12:
					if(pad.Buttons & PSP_CTRL_LEFT) cfg.buttonMenu = getPrevButton(cfg.buttonMenu);
					if(pad.Buttons & PSP_CTRL_RIGHT) cfg.buttonMenu = getNextButton(cfg.buttonMenu);
					break;
				case 13:
					if(pad.Buttons & PSP_CTRL_LEFT) cfg.buttonHUD = getPrevButton(cfg.buttonHUD);
					if(pad.Buttons & PSP_CTRL_RIGHT) cfg.buttonHUD = getNextButton(cfg.buttonHUD);
					break;
				case 14:
					if(pad.Buttons & PSP_CTRL_LEFT) cfg.buttonScreenshot = getPrevButton(cfg.buttonScreenshot);
					if(pad.Buttons & PSP_CTRL_RIGHT) cfg.buttonScreenshot = getNextButton(cfg.buttonScreenshot);
					break;
			}
		} else if(showMenu == 1 && infoIndex >= 0 && infoIndex < 4 && (pad.Buttons & PSP_CTRL_SQUARE)) {
			cfg.alignment[infoIndex]++;
			if(cfg.alignment[infoIndex] > 6) cfg.alignment[infoIndex] = 0;
		} else if(showMenu == 1 && infoIndex >= 0 && infoIndex < 4 && (pad.Buttons & PSP_CTRL_TRIANGLE)) {
			cfg.alignment[0]++;
			if(cfg.alignment[0] > 5) cfg.alignment[0] = 0;
			cfg.alignment[1] = cfg.alignment[0];
			cfg.alignment[2] = cfg.alignment[0];
			cfg.alignment[3] = cfg.alignment[0];
		} else if(showMenu == 1 && selection == 7 && (pad.Buttons & PSP_CTRL_TRIANGLE)) {
			cfg.order[0] = 0;
			cfg.order[1] = 1;
			cfg.order[2] = 2;
			cfg.order[3] = 3;
		} else if(showMenu == 1 && (pad.Buttons & PSP_CTRL_CIRCLE)) {
			save(&cfg);
			showMenu = 0;
			disable_controller = 0;
		}
	}
	pad_old = pad.Buttons;


	//
	// handle custom alignment
	//
	if(showMenu && (infoIndex >= 0 && infoIndex < 4) && display_width > 0 && display_height > 0) {
		int mode = -1;
		sceCtrl_driver_GetSamplingMode(&mode);
		if(mode == 0) sceCtrl_driver_SetSamplingMode(1);
		if(pad.Lx < (128 - 18) || pad.Lx > (128 + 18)) {
			cfg.alignment[infoIndex] = 6;
			cfg.custom_x[infoIndex] += ((float)pad.Lx / 256.0f - 0.5f);
			if(cfg.custom_x[infoIndex] < 0) cfg.custom_x[infoIndex] = 0;
			if(cfg.custom_x[infoIndex] > MIN(display_max_x[infoIndex], display_width)) cfg.custom_x[infoIndex] = MIN(display_max_x[infoIndex], display_width);
		}
		if(pad.Ly < (128 - 18) || pad.Ly > (128 + 18)) {
			cfg.alignment[infoIndex] = 6;
			cfg.custom_y[infoIndex] += ((float)pad.Ly / 256.0f - 0.5f);
			if(cfg.custom_y[infoIndex] < 0) cfg.custom_y[infoIndex] = 0;
			if(cfg.custom_y[infoIndex] > MIN(display_max_y[infoIndex], display_height)) cfg.custom_y[infoIndex] = MIN(display_max_y[infoIndex], display_height);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  MENU DRAWING
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void drawAlignment(CANVAS *canvas, int x, int y, int width, int height, int alignment)
{
	if(canvas != NULL) {
		fillRectangle(canvas, x, y, width, height, WHITE, SELECTION_BORDER);
		int w2 = width/2;
		int h2 = height/2;
		int w4 = width/4;
		//int h4 = height/4;
		if(alignment == 0) { // top left
			fillRectangle(canvas, x, y, w2, h2, SELECTION_BORDER, SELECTION_BORDER);
		} else if(alignment == 1) { // top center
			fillRectangle(canvas, x+w4, y, w2, h2, SELECTION_BORDER, SELECTION_BORDER);
		} else if(alignment == 2) { // top right
			fillRectangle(canvas, x+w2, y, w2, h2, SELECTION_BORDER, SELECTION_BORDER);

		} else if(alignment == 3) { // bottom left
			fillRectangle(canvas, x, y+h2, w2, h2, SELECTION_BORDER, SELECTION_BORDER);
		} else if(alignment == 4) { // bottom center
			fillRectangle(canvas, x+w4, y+h2, w2, h2, SELECTION_BORDER, SELECTION_BORDER);
		} else if(alignment == 5) { // bottom right
			fillRectangle(canvas, x+w2, y+h2, w2, h2, SELECTION_BORDER, SELECTION_BORDER);

		} else if(alignment == 6) { // custom
			//fillRectangle(canvas, x+w4, y+h4, w2, h2, SELECTION_BORDER, SELECTION_BORDER);
			drawSmallFont(canvas, "C", x+w2-3, y+h2-3);
		}
	}
}

void checkMenuMinimumSize()
{
	if(draw_min_x >= 0 && menu_min_x > 0) {
		if((draw_min_x - 10) < menu_min_x) menu_min_x = draw_min_x - 10;
		if(display_width > 0 && (display_width - draw_max_x - 10) < menu_min_x) menu_min_x = display_width - draw_max_x - 10;
	}
}

void drawMenu(CANVAS *canvas)
{
	if(showMenu && canvas != NULL) {
		int i = 0, x = 212, y = 25;
		int sel_x = 120, sel_w = 480 - sel_x * 2;
		if(menu_min_x >= 0 && display_width > 0 && sel_x > menu_min_x) {
			sel_x = menu_min_x;
			sel_w = display_width - sel_x * 2;
		} else if(menu_min_x < 0 && display_width > 0) {
			menu_min_x = display_width;
		}

		sprintf(text, "%s: %s", lang.LANGUAGE, lang.name);
		if(selection == 0) fillRectangle(canvas, sel_x, y-2, sel_w, 11, SELECTION_FILL, SELECTION_BORDER);
		drawSmallFont(canvas, text, x - strlen(lang.LANGUAGE)*6, y);
		checkMenuMinimumSize();

		y += 10;
		if(cfg.showHUD > 0) {
			sprintf(text, "%s %s: %s %i", lang.SHOW, lang.HUD, lang.METHOD, cfg.showHUD);
		} else {
			sprintf(text, "%s %s: %s", lang.SHOW, lang.HUD, lang.OFF);
		}
		if(selection == 1) fillRectangle(canvas, sel_x, y-2, sel_w, 11, SELECTION_FILL, SELECTION_BORDER);
		drawSmallFont(canvas, text, x - strlen(lang.SHOW)*6 - strlen(lang.HUD)*6 - 6, y);
		checkMenuMinimumSize();

		y += 10;
		sprintf(text, "%s %s:    %s", lang.SHOW, lang.FPS, (cfg.showFPS == 0 ? lang.OFF : cfg.showFPS == 1 ? lang.FPS_NUMBER_ONLY : cfg.showFPS == 2 ? lang.FPS_PREFIX : lang.FPS_SUFFIX));
		if(selection == 2) fillRectangle(canvas, sel_x, y-2, sel_w, 11, SELECTION_FILL, SELECTION_BORDER);
		drawSmallFont(canvas, text, x - strlen(lang.SHOW)*6 - strlen(lang.FPS)*6 - 6, y);
		drawAlignment(canvas, x+10, y-2, 16, 11, cfg.alignment[0]);
		checkMenuMinimumSize();

		y += 10;
		sprintf(text, "%s %s:    %s", lang.SHOW, lang.CPU, (cfg.showCPU == 0 ? lang.OFF : cfg.showCPU == 1 ? lang.CPU_SPEED : cfg.showCPU == 2 ? lang.CPU_BUS_SPEED : cfg.showCPU == 3 ? lang.CPU_USAGE : cfg.showCPU == 4 ? lang.CPU_SPEED_AND_USAGE : lang.CPU_BUS_SPEED_AND_USAGE));
		if(selection == 3) fillRectangle(canvas, sel_x, y-2, sel_w, 11, SELECTION_FILL, SELECTION_BORDER);
		drawSmallFont(canvas, text, x - strlen(lang.SHOW)*6 - strlen(lang.CPU)*6 - 6, y);
		drawAlignment(canvas, x+10, y-2, 16, 11, cfg.alignment[1]);
		checkMenuMinimumSize();

		y += 10;
		sprintf(text, "%s %s:    %s", lang.SHOW, lang.CPU_NOTIFICATION, (cfg.showCPUNotofication == 0 ? lang.OFF : cfg.showCPUNotofication == 1 ? lang.CPU_SPEED : lang.CPU_BUS_SPEED));
		if(selection == 4) fillRectangle(canvas, sel_x, y-2, sel_w, 11, SELECTION_FILL, SELECTION_BORDER);
		drawSmallFont(canvas, text, x - strlen(lang.SHOW)*6 - strlen(lang.CPU_NOTIFICATION)*6 - 6, y);
		//drawAlignment(canvas, x+10, y-2, 16, 11, cfg.alignment[1]);
		checkMenuMinimumSize();

		y += 10;
		sprintf(text, "%s %s:    %s", lang.SHOW, lang.BATTERY, (cfg.showBattery == 0 ? lang.OFF : cfg.showBattery == 1 ? lang.PERCENT : cfg.showBattery == 2 ? lang.TIME_LEFT : lang.PERCENT_AND_TIME_LEFT));
		if(selection == 5) fillRectangle(canvas, sel_x, y-2, sel_w, 11, SELECTION_FILL, SELECTION_BORDER);
		drawSmallFont(canvas, text, x - strlen(lang.SHOW)*6 - strlen(lang.BATTERY)*6 - 6, y);
		drawAlignment(canvas, x+10, y-2, 16, 11, cfg.alignment[2]);
		checkMenuMinimumSize();

		y += 10;
		sprintf(text, "%s %s:    %s", lang.SHOW, lang.TIME, (cfg.showTime == 0 ? lang.OFF : cfg.showTime == 1 ? lang.TIME_ICON_24H : cfg.showTime == 2 ? lang.TIME_ICON_12H_SUFFIX : cfg.showTime == 3 ? lang.TIME_ICON_12H_NOSUFFIX : cfg.showTime == 4 ? lang.TIME_NOICON_24H : cfg.showTime == 5 ? lang.TIME_NOICON_12H_SUFFIX : lang.TIME_NOICON_12H_NOSUFFIX));
		if(selection == 6) fillRectangle(canvas, sel_x, y-2, sel_w, 11, SELECTION_FILL, SELECTION_BORDER);
		drawSmallFont(canvas, text, x - strlen(lang.SHOW)*6 - strlen(lang.TIME)*6 - 6, y);
		drawAlignment(canvas, x+10, y-2, 16, 11, cfg.alignment[3]);
		checkMenuMinimumSize();

		y += 10;
		sprintf(text, "%s: ", lang.ORDER);
		int sel1 = 0, sel2 = 0;
		for(i = 0; i < 4; i++) {
			if(order_selection == i) sel1 = strlen(text) - 1;
			if(cfg.order[i] == 0) strcat(text, lang.FPS);
			if(cfg.order[i] == 1) strcat(text, lang.CPU);
			if(cfg.order[i] == 2) strcat(text, lang.BATTERY);
			if(cfg.order[i] == 3) strcat(text, lang.TIME);
			strcat(text, "  ");
			if(order_selection == i) sel2 = strlen(text) - 1;
		}
		if(selection == 7) {
			fillRectangle(canvas, sel_x, y-2, sel_w, 11, SELECTION_FILL, SELECTION_BORDER);
			fillRectangle(canvas, x - strlen(lang.ORDER)*6 + sel1*6, y-3, (sel2-sel1)*6, 13, ORDER_SELECTION_FILL, SELECTION_BORDER);
		}
		drawSmallFont(canvas, text, x - strlen(lang.ORDER)*6, y);
		checkMenuMinimumSize();

		y += 20;
		if(cfg.speedXMB > 0 && cfg.speedXMB < MAX_SPEEDS) {
			sprintf(text, "%s %s: %i/%i", lang.CPU_SPEED, lang.XMB, CPU_SPEED[cfg.speedXMB], BUS_SPEED[cfg.speedXMB]);
		} else {
			sprintf(text, "%s %s: %s", lang.CPU_SPEED, lang.XMB, lang.OFF);
		}
		if(selection == 8) fillRectangle(canvas, sel_x, y-2, sel_w, 11, SELECTION_FILL, SELECTION_BORDER);
		drawSmallFont(canvas, text, x - strlen(lang.CPU_SPEED)*6 - strlen(lang.XMB)*6 - 6, y);
		checkMenuMinimumSize();

		y += 10;
		if(cfg.speedGAME > 0 && cfg.speedGAME < MAX_SPEEDS) {
			sprintf(text, "%s %s: %i/%i", lang.CPU_SPEED, lang.GAME, CPU_SPEED[cfg.speedGAME], BUS_SPEED[cfg.speedGAME]);
		} else {
			sprintf(text, "%s %s: %s", lang.CPU_SPEED, lang.GAME, lang.OFF);
		}
		if(selection == 9) fillRectangle(canvas, sel_x, y-2, sel_w, 11, SELECTION_FILL, SELECTION_BORDER);
		drawSmallFont(canvas, text, x - strlen(lang.CPU_SPEED)*6 - strlen(lang.GAME)*6 - 6, y);
		checkMenuMinimumSize();

		y += 10;
		if(cfg.speedPOPS > 0 && cfg.speedPOPS < MAX_SPEEDS) {
			sprintf(text, "%s %s: %i/%i", lang.CPU_SPEED, lang.POPS, CPU_SPEED[cfg.speedPOPS], BUS_SPEED[cfg.speedPOPS]);
		} else {
			sprintf(text, "%s %s: %s", lang.CPU_SPEED, lang.POPS, lang.OFF);
		}
		if(selection == 10) fillRectangle(canvas, sel_x, y-2, sel_w, 11, SELECTION_FILL, SELECTION_BORDER);
		drawSmallFont(canvas, text, x - strlen(lang.CPU_SPEED)*6 - strlen(lang.POPS)*6 - 6, y);
		checkMenuMinimumSize();

		y += 20;
		sprintf(text, "%s: %s", lang.COMBO_BUTTON, getButtonName(cfg.buttonCombo, &lang));
		if(selection == 11) fillRectangle(canvas, sel_x, y-2, sel_w, 11, SELECTION_FILL, SELECTION_BORDER);
		drawSmallFont(canvas, text, x - strlen(lang.COMBO_BUTTON)*6, y);
		checkMenuMinimumSize();

		y += 10;
		sprintf(text, "%s: %s", lang.MENU_BUTTON, getButtonName(cfg.buttonMenu, &lang));
		if(selection == 12) fillRectangle(canvas, sel_x, y-2, sel_w, 11, SELECTION_FILL, SELECTION_BORDER);
		drawSmallFont(canvas, text, x - strlen(lang.MENU_BUTTON)*6, y);
		checkMenuMinimumSize();

		y += 10;
		sprintf(text, "%s: %s", lang.HUD_BUTTON, getButtonName(cfg.buttonHUD, &lang));
		if(selection == 13) fillRectangle(canvas, sel_x, y-2, sel_w, 11, SELECTION_FILL, SELECTION_BORDER);
		drawSmallFont(canvas, text, x - strlen(lang.HUD_BUTTON)*6, y);
		checkMenuMinimumSize();

		y += 10;
		sprintf(text, "%s: %s", lang.SCREENSHOT_BUTTON, getButtonName(cfg.buttonScreenshot, &lang));
		if(selection == 14) fillRectangle(canvas, sel_x, y-2, sel_w, 11, SELECTION_FILL, SELECTION_BORDER);
		drawSmallFont(canvas, text, x - strlen(lang.SCREENSHOT_BUTTON)*6, y);
		checkMenuMinimumSize();

		x -= 6;
		y += 20;
		sprintf(text, "%s - %s", lang.CTRL_CIRCLE, lang.CLOSE);
		drawSmallFont(canvas, text, x - strlen(lang.CTRL_CIRCLE)*6, y);
		checkMenuMinimumSize();
		y += 10;
		sprintf(text, "%s/%s - %s", lang.CTRL_UP, lang.CTRL_DOWN, lang.CHANGE_SELECTION);
		drawSmallFont(canvas, text, x - strlen(lang.CTRL_UP)*6 - 6 - strlen(lang.CTRL_DOWN)*6, y);
		checkMenuMinimumSize();
		y += 10;
		sprintf(text, "%s/%s - %s", lang.CTRL_LEFT, lang.CTRL_RIGHT, lang.CHANGE_OPTION);
		drawSmallFont(canvas, text, x - strlen(lang.CTRL_LEFT)*6 - 6 - strlen(lang.CTRL_RIGHT)*6, y);
		checkMenuMinimumSize();
		if(selection >= 2 && selection <= 6) {
			y += 10;
			sprintf(text, "%s - %s", lang.CTRL_SQUARE, lang.CHANGE_ALIGNMENT);
			drawSmallFont(canvas, text, x - strlen(lang.CTRL_SQUARE)*6, y);
			checkMenuMinimumSize();
			y += 10;
			sprintf(text, "%s - %s", lang.ANALOG, lang.CHANGE_CUSTOM_POSITION);
			drawSmallFont(canvas, text, x - strlen(lang.ANALOG)*6, y);
			checkMenuMinimumSize();
			y += 10;
			sprintf(text, "%s - %s", lang.CTRL_TRIANGLE, lang.CHANGE_ALIGNMENT_FOR_ALL);
			drawSmallFont(canvas, text, x - strlen(lang.CTRL_TRIANGLE)*6, y);
			checkMenuMinimumSize();			
		}
		if(selection == 7) {
			y += 10;
			sprintf(text, "%s + %s/%s - %s", lang.CTRL_CROSS, lang.CTRL_LEFT, lang.CTRL_RIGHT, lang.CHANGE_ORDER);
			drawSmallFont(canvas, text, x - strlen(lang.CTRL_CROSS)*6 - strlen(lang.CTRL_LEFT)*6 - strlen(lang.CTRL_RIGHT)*6 - 24, y);
			checkMenuMinimumSize();
			y += 10;
			sprintf(text, "%s - %s", lang.CTRL_TRIANGLE, lang.RESET_ORDER);
			drawSmallFont(canvas, text, x - strlen(lang.CTRL_TRIANGLE)*6, y);
			checkMenuMinimumSize();
		}

	}
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  CPU SPEED THROTTLING
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int cpu_frequency = 0, bus_frequency = 0;
int prev_cpu_frequency = 0, prev_bus_frequency = 0, check_cpu_frequency = 0;
int notification_counter = 0;
int speed_change = 0;
int speed_change_counter = 0;

void updateCpuSpeed()
{
	cpu_frequency = scePower_driver_GetCpuClockFrequencyInt();
	bus_frequency = scePower_driver_GetBusClockFrequencyInt();

	if(prev_cpu_frequency == 0) prev_cpu_frequency = cpu_frequency;
	if(prev_bus_frequency == 0) prev_bus_frequency = bus_frequency;

	if(prev_cpu_frequency != cpu_frequency) {
		if(notification_counter == 0) {
			notification_counter = 359;
			check_cpu_frequency = cpu_frequency;
		} else if(notification_counter <= 1) {
			notification_counter = 0;
			prev_cpu_frequency = cpu_frequency;
			prev_bus_frequency = bus_frequency;
		} else {
			notification_counter--;
			if(check_cpu_frequency != cpu_frequency) {
				notification_counter = 359;
				prev_cpu_frequency = check_cpu_frequency;
				check_cpu_frequency = cpu_frequency;
			}
		}
	}

	if(speed_change > 0 && speed_change < MAX_SPEEDS && speed_change_counter == 0) {
		scePower_driver_SetClockFrequency(CPU_SPEED[speed_change], CPU_SPEED[speed_change], BUS_SPEED[speed_change]);
		speed_change = 0;
	} else {

		if(showMenu == 1) {

			int diff = CPU_SPEED[DEFAULT_MENU_SPEED] - cpu_frequency;
			// change only if less than default
			if(speed_change_counter == 0 && diff >= 5) {
				speed_change = DEFAULT_MENU_SPEED;
				speed_change_counter = 60;
			}

		} else {

			int speed = 0;
			if(initKeyConfig == PSP_INIT_KEYCONFIG_VSH) {
				if(cfg.speedXMB > 0 && cfg.speedXMB < MAX_SPEEDS) speed = cfg.speedXMB;
			} else if(initKeyConfig == PSP_INIT_KEYCONFIG_GAME) {
				if(cfg.speedGAME > 0 && cfg.speedGAME < MAX_SPEEDS) speed = cfg.speedGAME;
			} else if(initKeyConfig == PSP_INIT_KEYCONFIG_POPS) {
				if(cfg.speedPOPS > 0 && cfg.speedPOPS < MAX_SPEEDS) speed = cfg.speedPOPS;
			}

			if(speed > 0 && speed < MAX_SPEEDS) {
				int diff = CPU_SPEED[speed] - cpu_frequency;
				if(diff > -5 && diff < 5) speed = 0;
				if(speed_change != speed) {
					speed_change = speed;
					speed_change_counter = 60;
				}
			}
		}
		if(speed_change_counter > 0) speed_change_counter--;
	}
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  INFO
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


int fps = 0;
int cpu_usage = 0;
int battery_percent = 0;
int battery_time = 0;
pspTime local_time;

void prepareInfoData()
{
	fps = getFPS();
	if(cfg.showCPU >= 3) { // 0 - off, 1 - cpu, 2 - cpu/bus, 3 - usage, 4 - cpu/usage, 5 - cpu/bus/usage
		cpu_usage = getCpuUsage();
	}
	if(cfg.showBattery > 0) {
		battery_percent = scePower_driver_GetBatteryLifePercent();
		battery_time = scePower_driver_GetBatteryLifeTime();
	}
	if(cfg.showTime > 0) {
		sceRtc_driver_GetCurrentClockLocalTime(&local_time);
	}
}

char info[6][255];
int info_len[6], info_x[6], info_y[6];
char cinfo[4][255];
int cinfo_len[4], cinfo_x[4], cinfo_y[4];

char *getInfoForAlignment(int alignment, int infoIndex)
{
	if(alignment < 0) alignment = 0;
	if(alignment > 6) alignment = 6;
	if(infoIndex < 0) infoIndex = 0;
	if(infoIndex > 3) infoIndex = 3;
	if(alignment == 6) {
		return cinfo[infoIndex];
	} else {
		cfg.custom_x[infoIndex] = info_x[alignment];
		cfg.custom_y[infoIndex] = info_y[alignment];
	}
	return info[alignment];
}

void getInfoFPS()
{
	// 0 - off, 1 - number only, 2 - number and prefix, 3 - number and suffix
	if(cfg.showFPS > 0) {
		char *info = getInfoForAlignment(cfg.alignment[0], 0);

		int fps_display = (fps > 0 ? fps : sceDisplay_driver_GetFramePerSec());

		if(cfg.showFPS == 1) {
			sprintf(text, "%02i ", fps_display);
			strcat(info, text);
		} else if(cfg.showFPS == 2) {
			sprintf(text, "\x0A\x0B%02i ", fps_display);
			strcat(info, text);
		} else if(cfg.showFPS == 3) {
			sprintf(text, "%02i\x0A\x0B ", fps_display);
			strcat(info, text);
		}
	}
}
void getInfoCPU()
{
	// 0 - off, 1 - cpu, 2 - cpu/bus, 3 - usage, 4 - cpu/usage, 5 - cpu/bus/usage
	if(cfg.showCPU > 0 || (cfg.showCPUNotofication > 0 && notification_counter > 0 && prev_cpu_frequency != cpu_frequency)) {
		char *info = getInfoForAlignment(cfg.alignment[1], 1);

		int showCPU = (cfg.showCPU == 1 || cfg.showCPU == 4 ? 1 : cfg.showCPU == 2 || cfg.showCPU == 5 ? 2 : 0);
		int showUsage = (cfg.showCPU >= 3 ? 1 : 0);

		if(cfg.showCPUNotofication > 0 && notification_counter > 0 && prev_cpu_frequency != cpu_frequency) {

			char blink[255];
			blink[0] = 0;
			strcat(blink, "\x01\x02\x03");
			if(cfg.showCPUNotofication == 1) {
				sprintf(text, "%i\x10%i\x07\x08\x09 ", prev_cpu_frequency, cpu_frequency);
				strcat(blink, text);
			} else if(cfg.showCPUNotofication == 2) {
				sprintf(text, "%i/%i\x10%i/%i\x07\x08\x09 ", prev_cpu_frequency, prev_bus_frequency, cpu_frequency, bus_frequency);
				strcat(blink, text);
			}

			if((notification_counter % 120 > 90) && cfg.showCPUNotofication > 0) {
				int i = 0;
				for(i = 0; i < strlen(blink); i++) blink[i] = ' ';
			}
			strcat(info, blink);

		} else {

			strcat(info, "\x01\x02\x03");
			if(showCPU == 1) {
				sprintf(text, "%i\x07\x08\x09 ", cpu_frequency);
				strcat(info, text);
			} else if(showCPU == 2) {
				sprintf(text, "%i/%i\x07\x08\x09 ", cpu_frequency, bus_frequency);
				strcat(info, text);
			}

		}

		if(showUsage == 1) {
			if(cpu_usage < 0) {
				sprintf(text, "--%% ");
			} if(cpu_usage >= 100) {
				sprintf(text, "%i%% ", 100);
			} else {
				sprintf(text, "%02i%% ", cpu_usage);
			}
			strcat(info, text);
		}
	}
}
void getInfoBattery()
{
	// 0 - off, 1 - percent, 2 - time, 3 - percent/time

	if(cfg.showBattery > 0) {
		char *info = getInfoForAlignment(cfg.alignment[2], 2);

		if(battery_percent < 0 || battery_percent > 100) {
			sprintf(text, "\x1D ");
		} else {
			if(cfg.showBattery == 1) {
				sprintf(text, "\x1A%i%% ", battery_percent);
			} else if(cfg.showBattery == 2) {
				if(battery_time == 0) {
					sprintf(text, "\x1A-:-- ");
				} else {
					sprintf(text, "\x1A%01i:%02i ", battery_time/60, battery_time%60);
				}
			} else {
				if(battery_time == 0) {
					sprintf(text, "\x1A%i%% -:-- ", battery_percent);
				} else {
					sprintf(text, "\x1A%i%% %01i:%02i ", battery_percent, battery_time/60, battery_time%60);
				}
			}
			if(battery_percent < 30) text[0] = '\x1B';
			if(battery_percent < 10) text[0] = '\x1C';
		}
		strcat(info, text);
	}
}
void getInfoTime()
{
	// 0 - off, 1 - icon,24h, 2 - icon,12h,suffix, 3 - icon,12h,no suffix, 4 - 24h, 5 - 12h,suffix, 6 - 12h,no suffix
	if(cfg.showTime > 0) {
		char *info = getInfoForAlignment(cfg.alignment[3], 3);
		int hour = local_time.hour % 12;
		if(hour == 0) hour = 12;
		int showIcon = (cfg.showTime >= 1 && cfg.showTime <= 3 ? 1 : 0);
		int format24h = (cfg.showTime == 1 || cfg.showTime == 4 ? 1 : 0);
		int suffix = (cfg.showTime == 2 || cfg.showTime == 5 ? 1 : 0);

		sprintf(text, "%s%02i:%02i%s ", 
			(showIcon == 1 ? "\x1E\x1F" : ""), 
			(format24h == 1 ? local_time.hour : hour), 
			local_time.minutes, 
			(suffix == 1 ? (local_time.hour < 12 ? "\x0E\x0F" : "\x0C\x0D") : ""));
		strcat(info, text);
	}
}

void getInfo()
{
	int i = 0;
	for(i = 0; i < 6; i++) info[i][0] = 0;
	for(i = 0; i < 4; i++) cinfo[i][0] = 0;

	if(cfg.showHUD > 0 || showMenu) {
		for(i = 0; i < 4; i++) {
			if(cfg.order[i] == 0) getInfoFPS();
			if(cfg.order[i] == 1) getInfoCPU();
			if(cfg.order[i] == 2) getInfoBattery();
			if(cfg.order[i] == 3) getInfoTime();
		}
	}

	for(i = 0; i < 6; i++) {
		char *text = info[i];
		int len = strlen(text);
		if(len > 0) {
			if(text[len-1] == ' ') { text[len-1] = 0; len--; }
			int maxx = display_width - len*6 - 2;
			int maxy = display_height - 8;
			switch(i) {
				case 0: // top left
					info_x[i] = 1;
					info_y[i] = 1;
					break;
				case 1: // top center
					info_x[i] = maxx/2;
					info_y[i] = 1;
					break;
				case 2: // top right
					info_x[i] = maxx;
					info_y[i] = 1;
					break;
				case 3: // bottom left
					info_x[i] = 1;
					info_y[i] = maxy;
					break;
				case 4: // bottom center
					info_x[i] = maxx/2;
					info_y[i] = maxy;
					break;
				case 5: // bottom right
					info_x[i] = maxx;
					info_y[i] = maxy;
					break;
			}
		}
		info_len[i] = len;
	}

	for(i = 0; i < 4; i++) {
		char *text = cinfo[i];
		int len = strlen(text);
		if(len > 0) {
			if(text[len-1] == ' ') { text[len-1] = 0; len--; }
			display_max_x[i] = display_width - len*6 - 2;
			display_max_y[i] = display_height - 8;
			cinfo_x[i] = MIN((int)cfg.custom_x[i], display_max_x[i]);
			cinfo_y[i] = MIN((int)cfg.custom_y[i], display_max_y[i]);			
		}
		cinfo_len[i] = len;
	}

}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  INFO DRAWING
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void drawInfo(CANVAS *canvas)
{
	if(canvas != NULL) {
		int i = 0;
		for(i = 0; i < 6; i++) {
			if(info_len[i] > 0) drawSmallFont(canvas, info[i], info_x[i], info_y[i]);
		}
		for(i = 0; i < 4; i++) {
			if(cinfo_len[i] > 0) drawSmallFont(canvas, cinfo[i], cinfo_x[i], cinfo_y[i]);
		}

/*
		char s[255];
		sprintf(s, "VER: %08lX", (u32)sceKernelDevkitVersion());
		drawSmallFont(canvas, s, 0, 10);
		float ram = (float)sceKernelTotalFreeMemSize() / (float)1000.0;
		sprintf(s, "MEM: %e", ram);
		drawSmallFont(canvas, s, 0, 20);

*/
/*
		sprintf(s, "MODE: %i (%i)", drawing_mode, fps_counter);
		drawSmallFont(canvas, s, 0, 0);
*/
/*
		char s[255];
		int cx = 0, cy = 0;
		sprintf(s, "MODE: %i", drawing_mode);
		for(cx = 0; cx < 480; cx+=80) {
			for(cy = 0; cy < 272; cy += 34) {
				drawSmallFont(canvas, s, cx, cy);
			}
		}
*/
	}
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  DISPLAY HOOKING
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int display_hooked = 0;

int display_set_frame_buf(void *topaddr, int bufferwidth, int pixelformat, int sync)
{
	fps_counter++;
	if(drawing_mode == 1 && wait == 0 && screenshot_timer < 0) {
		wait = 1;
		int mode = 0;
		sceDisplay_driver_GetMode(&mode, &(displayCanvas.width), &(displayCanvas.height));
		displayCanvas.buffer = topaddr;
		displayCanvas.lineWidth = bufferwidth;
		displayCanvas.pixelFormat = pixelformat;
		display_width = displayCanvas.width;
		display_height = displayCanvas.height;
		getInfo();
		drawInfo(&displayCanvas);
		drawMenu(&displayCanvas);
		wait = 0;
	}
	return sceDisplay_driver_SetFrameBuf(topaddr, bufferwidth, pixelformat, sync);
}

void hookDisplay()
{
	SceModule *pMod = sceKernelFindModuleByName("sceDisplay_Service");
	if(pMod != NULL) {
		if(apiHookByName(pMod->modid, "sceDisplay", "sceDisplaySetFrameBuf", display_set_frame_buf) != 0) display_hooked = 1;
	}
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  MAIN THREAD
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int main_thread(SceSize argc, void* argp)
{

	// default options
	cfg.language = 0;
	cfg.showHUD = 1;
	cfg.showFPS = 0;
	cfg.showCPU = 0;
	cfg.showCPUNotofication = 1;
	cfg.showBattery = 1;
	cfg.showTime = 1;
	cfg.order[0] = 0;
	cfg.order[1] = 1;
	cfg.order[2] = 2;
	cfg.order[3] = 3;
	cfg.alignment[0] = 5;
	cfg.alignment[1] = 5;
	cfg.alignment[2] = 5;
	cfg.alignment[3] = 5;
	cfg.custom_x[0] = 0;
	cfg.custom_x[1] = 0;
	cfg.custom_x[2] = 0;
	cfg.custom_x[3] = 0;
	cfg.custom_y[0] = 0;
	cfg.custom_y[1] = 0;
	cfg.custom_y[2] = 0;
	cfg.custom_y[3] = 0;
	cfg.speedXMB = 0;
	cfg.speedGAME = 0;
	cfg.speedPOPS = 0;
	cfg.buttonCombo = PSP_CTRL_NOTE;
	cfg.buttonMenu = PSP_CTRL_LTRIGGER;
	cfg.buttonHUD = PSP_CTRL_RTRIGGER;
	cfg.buttonScreenshot = PSP_CTRL_UP;
	// load saved options
	load(&cfg);
	cfg.language = loadLang(&lang, cfg.language);
	SceKernelSysClock clock_start, clock_end;

	sceKernelDelayThread(2000000);
	hookDisplay();
	hookController();

	while(1) {

		sceKernelGetSystemTime(&clock_start);
		updateCpuSpeed();
		menuNavigation();
		prepareInfoData();
		if(screenshot_timer > 0) {
			screenshot_timer--;
		} else if(screenshot_timer == 0) {
			screenshot();
			screenshot_timer = -1;
		}

		drawing_mode = (fps > 0 && cfg.showHUD == 1 ? 1 : 0);

		if(drawing_mode == 0) {
			//do { sceDisplay_driver_WaitVblankStart(); } while(wait == 1);
			if(screenshot_timer < 0 && wait == 0) {
				sceDisplay_driver_WaitVblankStart();
				wait = 1;
				if((cfg.showHUD > 0 || showMenu) && getCanvas(&displayCanvas)) {
					display_width = displayCanvas.width;
					display_height = displayCanvas.height;

					// get info, measure time for info retreival
					getInfo();
					int vblank = (int)(1000000.0f / sceDisplay_driver_GetFramePerSec());
					int vblank_min = vblank / 10;
					int vblank_max = vblank - vblank_min;

					sceKernelGetSystemTime(&clock_end);
					int calc_speed = (int)clock_end.low - (int)clock_start.low;

					// draw info and menu, measure time for drawing
					sceKernelGetSystemTime(&clock_start);
					drawInfo(&displayCanvas);
					drawMenu(&displayCanvas);
					sceKernelGetSystemTime(&clock_end);

					// calculate dalay time
					int draw_speed = (int)clock_end.low - (int)clock_start.low;
					int delay = vblank - draw_speed*3 - calc_speed;
					if(delay < vblank_min) delay = vblank_min;
					if(delay > vblank_max) delay = vblank_max;
		
					sceKernelDelayThread(delay);
					drawInfo(&displayCanvas);
					drawMenu(&displayCanvas);
				} else {
					sceKernelDelayThread(10000);
				}
				wait = 0;
			} else {
				sceKernelDelayThread(10000);
			}
		} else {
			sceKernelDelayThread(10000);
		}
	}
	sceKernelExitDeleteThread(0);
	return 0;
}

int module_start(SceSize args, void *argp)
{
	//logAppendLine("module start");
	nkLoad();
	initKeyConfig = sceKernelInitKeyConfig();
	SceUID thid = sceKernelCreateThread("main_thread", main_thread, 1, 0x8000, 0, NULL);
	if(thid >= 0) sceKernelStartThread(thid, args, argp);
	return 0;
}


int module_stop(SceSize args, void *argp)
{
	//logAppendLine("module stop");
	return 0;
}
