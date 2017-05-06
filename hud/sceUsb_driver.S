	.set noreorder

#include "pspimport.s"

	IMPORT_START	"sceUsb_driver",0x00010000
	IMPORT_FUNC	"sceUsb_driver",0xAE5DE6AF,sceUsb_driver_Start
	IMPORT_FUNC	"sceUsb_driver",0xC2464FA0,sceUsb_driver_Stop
	IMPORT_FUNC	"sceUsb_driver",0xC21645A4,sceUsb_driver_GetState
//	IMPORT_FUNC	"sceUsb_driver",0x4E537366,sceUsb_driver_GetDrvList
//	IMPORT_FUNC	"sceUsb_driver",0x112CC951,sceUsb_driver_GetDrvState
	IMPORT_FUNC	"sceUsb_driver",0x586DB82C,sceUsb_driver_Activate
	IMPORT_FUNC	"sceUsb_driver",0xC572A9C8,sceUsb_driver_Deactivate
//	IMPORT_FUNC	"sceUsb_driver",0x5BE0E002,sceUsb_driver_WaitState
//	IMPORT_FUNC	"sceUsb_driver",0x1C360735,sceUsb_driver_WaitCancel
