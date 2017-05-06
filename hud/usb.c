#include <psptypes.h>
#include <pspkernel.h>
#include <pspusb.h>
#include <pspusbstor.h>
#include <stdio.h>
//#include <stdlib.h>
#include <string.h>
#include "main.h"

int usbModules = 0;
int usbmodloaded = 0;

int build_args(char *args, const char *execfile, int argc, char **argv)
{
	int loc = 0;
	int i;

	strcpy(args, execfile);
	loc += strlen(execfile) + 1;
	for(i = 0; i < argc; i++) {
		strcpy(&args[loc], argv[i]);
		printf("Arg %i => %s\n", i, argv[i]);
		loc += strlen(argv[i]) + 1;
	}

	return loc;
}

int loadStartModule(const char *name, int argc, char **argv)
{
	SceUID modid = sceKernelLoadModule(name, 0, NULL);	
	if(modid >= 0) {
		int status = 0;
		char args[128];
		int len = build_args(args, name, argc, argv);
		modid = sceKernelStartModule(modid, len, (void *) args, &status, NULL);
	}
	return modid;
}

void unloadUsb()
{
	sceUsb_driver_Deactivate(0x1c8);
	sceUsb_driver_Stop(PSP_USBSTOR_DRIVERNAME, 0, 0);
  	sceUsb_driver_Stop(PSP_USBBUS_DRIVERNAME, 0, 0);
	usbModules = 0;
}

int loadUsb() 
{
	if(!usbModules) {
		if(!usbmodloaded) {
			loadStartModule("flash0:/kd/semawm.prx", 0, NULL);
			loadStartModule("flash0:/kd/usbstor.prx", 0, NULL);
			loadStartModule("flash0:/kd/usbstormgr.prx", 0, NULL);
			loadStartModule("flash0:/kd/usbstorms.prx", 0, NULL);
			loadStartModule("flash0:/kd/usbstorboot.prx", 0, NULL);
			usbmodloaded = 1;
		}		
		sceUsb_driver_Start(PSP_USBBUS_DRIVERNAME, 0, 0); //sceUsbStart
		sceUsb_driver_Start(PSP_USBSTOR_DRIVERNAME, 0, 0); //sceUsbStart
		//sceUsb_driver_storBootSetCapacity(0x800000); //sceUsbstorBootSetCapacity
		sceKernelDelayThread(50000);
		usbModules = 1;
	}

	int state = sceUsb_driver_GetState();

	if(state & PSP_USB_ACTIVATED) {
		unloadUsb();
	}
	else {
		sceUsb_driver_Activate(0x1c8);
	}
	return 0;
}
