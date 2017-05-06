	.set noreorder

#include "pspimport.s"

	IMPORT_START	"sceDisplay_driver",0x00010000
//	IMPORT_FUNC	"sceDisplay_driver",0x206276C2,sceDisplay_driver_Init
//	IMPORT_FUNC	"sceDisplay_driver",0x7A10289D,sceDisplay_driver_End
//	IMPORT_FUNC	"sceDisplay_driver",0x0E20F177,sceDisplay_driver_SetMode
	IMPORT_FUNC	"sceDisplay_driver",0xDEA197D4,sceDisplay_driver_GetMode
	IMPORT_FUNC	"sceDisplay_driver",0xDBA6C4C4,sceDisplay_driver_GetFramePerSec
//	IMPORT_FUNC	"sceDisplay_driver",0x432D133F,sceDisplay_driver_Enable
//	IMPORT_FUNC	"sceDisplay_driver",0x681EE6A7,sceDisplay_driver_Disable
//	IMPORT_FUNC	"sceDisplay_driver",0x7ED59BC4,sceDisplay_driver_SetHoldMode
//	IMPORT_FUNC	"sceDisplay_driver",0xA544C486,sceDisplay_driver_SetResumeMode
//	IMPORT_FUNC	"sceDisplay_driver",0x63E22A26,sceDisplay_driver_63E22A26
//	IMPORT_FUNC	"sceDisplay_driver",0x5B5AEFAD,sceDisplay_driver_5B5AEFAD
	IMPORT_FUNC	"sceDisplay_driver",0x289D82FE,sceDisplay_driver_SetFrameBuf
	IMPORT_FUNC	"sceDisplay_driver",0xEEDA2E54,sceDisplay_driver_GetFrameBuf
//	IMPORT_FUNC	"sceDisplay_driver",0xB4F378FA,sceDisplay_driver_IsForeground
//	IMPORT_FUNC	"sceDisplay_driver",0xAC14F1FF,sceDisplay_driver_GetForegroundLevel
//	IMPORT_FUNC	"sceDisplay_driver",0x9E3C6DC6,sceDisplay_driver_SetBrightness
//	IMPORT_FUNC	"sceDisplay_driver",0x31C4BAA8,sceDisplay_driver_GetBrightness
//	IMPORT_FUNC	"sceDisplay_driver",0x9C6EAAD7,sceDisplay_driver_GetVcount
//	IMPORT_FUNC	"sceDisplay_driver",0x4D4E10EC,sceDisplay_driver_IsVblank
//	IMPORT_FUNC	"sceDisplay_driver",0x69B53541,sceDisplay_driver_GetVblankRest
//	IMPORT_FUNC	"sceDisplay_driver",0x36CDFADE,sceDisplay_driver_WaitVblank
//	IMPORT_FUNC	"sceDisplay_driver",0x8EB9EC49,sceDisplay_driver_WaitVblankCB
	IMPORT_FUNC	"sceDisplay_driver",0x984C27E7,sceDisplay_driver_WaitVblankStart
//	IMPORT_FUNC	"sceDisplay_driver",0x46F186C3,sceDisplay_driver_WaitVblankStartCB
//	IMPORT_FUNC	"sceDisplay_driver",0x773DD3A3,sceDisplay_driver_GetCurrentHcount
//	IMPORT_FUNC	"sceDisplay_driver",0x210EAB3A,sceDisplay_driver_GetAccumulatedHcount
