	.set noreorder

#include "pspimport.s"

	IMPORT_START "sceCtrl_driver",0x00010000
//	IMPORT_FUNC	 "sceCtrl_driver",0x3E65A0EA,sceCtrl_driver_Init
//	IMPORT_FUNC	 "sceCtrl_driver",0xE03956E9,sceCtrl_driver_End
//	IMPORT_FUNC	 "sceCtrl_driver",0xC3F607F3,sceCtrl_driver_Suspend
//	IMPORT_FUNC	 "sceCtrl_driver",0xC245B57B,sceCtrl_driver_Resume
//	IMPORT_FUNC	 "sceCtrl_driver",0x6A2774F3,sceCtrl_driver_SetSamplingCycle
//	IMPORT_FUNC	 "sceCtrl_driver",0x02BAAD91,sceCtrl_driver_GetSamplingCycle
	IMPORT_FUNC	 "sceCtrl_driver",0x1F4011E6,sceCtrl_driver_SetSamplingMode
	IMPORT_FUNC	 "sceCtrl_driver",0xDA6B76A1,sceCtrl_driver_GetSamplingMode
	IMPORT_FUNC	 "sceCtrl_driver",0x3A622550,sceCtrl_driver_PeekBufferPositive
	IMPORT_FUNC	 "sceCtrl_driver",0xC152080A,sceCtrl_driver_PeekBufferNegative
	IMPORT_FUNC	 "sceCtrl_driver",0x1F803938,sceCtrl_driver_ReadBufferPositive
	IMPORT_FUNC	 "sceCtrl_driver",0x60B81F86,sceCtrl_driver_ReadBufferNegative
//	IMPORT_FUNC	 "sceCtrl_driver",0xB1D0E5CD,sceCtrl_driver_PeekLatch
//	IMPORT_FUNC	 "sceCtrl_driver",0x0B588501,sceCtrl_driver_ReadLatch
//	IMPORT_FUNC	 "sceCtrl_driver",0xA88E8D22,sceCtrl_driver_A88E8D22
//	IMPORT_FUNC	 "sceCtrl_driver",0xB7CEAED4,sceCtrl_driver_B7CEAED4
//	IMPORT_FUNC	 "sceCtrl_driver",0x348D99D4,sceCtrl_driver_348D99D4
//	IMPORT_FUNC	 "sceCtrl_driver",0xAF5960F3,sceCtrl_driver_AF5960F3
//	IMPORT_FUNC	 "sceCtrl_driver",0xA68FD260,sceCtrl_driver_A68FD260
//	IMPORT_FUNC	 "sceCtrl_driver",0x6841BE1A,sceCtrl_driver_6841BE1A
	IMPORT_FUNC	 "sceCtrl_driver",0x7CA723DC,sceCtrl_driver_7CA723DC
//	IMPORT_FUNC	 "sceCtrl_driver",0x5E77BC8A,sceCtrl_driver_5E77BC8A
//	IMPORT_FUNC	 "sceCtrl_driver",0x5C56C779,sceCtrl_driver_5C56C779
