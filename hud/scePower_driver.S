	.set noreorder

#include "pspimport.s"

	IMPORT_START	"scePower_driver",0x00010000
//	IMPORT_FUNC	"scePower_driver",0x9CE06934,scePower_driver_Init
//	IMPORT_FUNC	"scePower_driver",0xAD5BB433,scePower_driver_End
//	IMPORT_FUNC	"scePower_driver",0x6D2CA84B,scePower_driver_WlanActivate
//	IMPORT_FUNC	"scePower_driver",0x23BB0A60,scePower_driver_WlanDeactivate
//	IMPORT_FUNC	"scePower_driver",0x2B51FE2F,scePower_driver_2B51FE2F
//	IMPORT_FUNC	"scePower_driver",0x442BFBAC,scePower_driver_442BFBAC
//	IMPORT_FUNC	"scePower_driver",0xE8685403,scePower_driver_E8685403
//	IMPORT_FUNC	"scePower_driver",0xEFD3C963,scePower_driver_Tick
//	IMPORT_FUNC	"scePower_driver",0xEDC13FE5,scePower_driver_GetIdleTimer
//	IMPORT_FUNC	"scePower_driver",0x1BA2FCAE,scePower_driver_SetIdleCallback
//	IMPORT_FUNC	"scePower_driver",0x7F30B3B1,scePower_driver_IdleTimerEnable
//	IMPORT_FUNC	"scePower_driver",0x972CE941,scePower_driver_IdleTimerDisable
//	IMPORT_FUNC	"scePower_driver",0x27F3292C,scePower_driver_BatteryUpdateInfo
//	IMPORT_FUNC	"scePower_driver",0xE8E4E204,scePower_driver_E8E4E204
//	IMPORT_FUNC	"scePower_driver",0xB999184C,scePower_driver_GetLowBatteryCapacity
//	IMPORT_FUNC	"scePower_driver",0x166922EC,scePower_driver_166922EC
//	IMPORT_FUNC	"scePower_driver",0xDD3D4DAC,scePower_driver_DD3D4DAC
//	IMPORT_FUNC	"scePower_driver",0x87440F5E,scePower_driver_IsPowerOnline
//	IMPORT_FUNC	"scePower_driver",0x0AFD0D8B,scePower_driver_IsBatteryExist
//	IMPORT_FUNC	"scePower_driver",0x1E490401,scePower_driver_IsBatteryCharging
//	IMPORT_FUNC	"scePower_driver",0xB4432BC8,scePower_driver_GetBatteryChargingStatus
//	IMPORT_FUNC	"scePower_driver",0xD3075926,scePower_driver_IsLowBattery
//	IMPORT_FUNC	"scePower_driver",0x78A1A796,scePower_driver_78A1A796
//	IMPORT_FUNC	"scePower_driver",0x94F5A53F,scePower_driver_GetBatteryRemainCapacity
//	IMPORT_FUNC	"scePower_driver",0xFD18A0FF,scePower_driver_FD18A0FF
	IMPORT_FUNC	"scePower_driver",0x2085D15D,scePower_driver_GetBatteryLifePercent
	IMPORT_FUNC	"scePower_driver",0x8EFB3FA2,scePower_driver_GetBatteryLifeTime
//	IMPORT_FUNC	"scePower_driver",0x28E12023,scePower_driver_GetBatteryTemp
//	IMPORT_FUNC	"scePower_driver",0x862AE1A6,scePower_driver_GetBatteryElec
//	IMPORT_FUNC	"scePower_driver",0x483CE86B,scePower_driver_GetBatteryVolt
//	IMPORT_FUNC	"scePower_driver",0x23436A4A,scePower_driver_23436A4A
//	IMPORT_FUNC	"scePower_driver",0x0CD21B1F,scePower_driver_0CD21B1F
//	IMPORT_FUNC	"scePower_driver",0x165CE085,scePower_driver_165CE085
//	IMPORT_FUNC	"scePower_driver",0xD6D016EF,scePower_driver_Lock
//	IMPORT_FUNC	"scePower_driver",0xCA3D34C1,scePower_driver_Unlock
//	IMPORT_FUNC	"scePower_driver",0x79DB9421,scePower_driver_RebootStart
//	IMPORT_FUNC	"scePower_driver",0xDB62C9CF,scePower_driver_CancelRequest
//	IMPORT_FUNC	"scePower_driver",0x7FA406DD,scePower_driver_IsRequest
//	IMPORT_FUNC	"scePower_driver",0x2B7C7CF4,scePower_driver_RequestStandby
//	IMPORT_FUNC	"scePower_driver",0xAC32C9CC,scePower_driver_RequestSuspend
//	IMPORT_FUNC	"scePower_driver",0x2875994B,scePower_driver_2875994B
//	IMPORT_FUNC	"scePower_driver",0x3951AF53,scePower_driver_EncodeUBattery
//	IMPORT_FUNC	"scePower_driver",0x0074EF9B,scePower_driver_GetResumeCount
//	IMPORT_FUNC	"scePower_driver",0xF535D928,scePower_driver_F535D928
//	IMPORT_FUNC	"scePower_driver",0x04B7766E,scePower_driver_RegisterCallback
//	IMPORT_FUNC	"scePower_driver",0xDFA8BAF8,scePower_driver_UnregisterCallback
//	IMPORT_FUNC	"scePower_driver",0xDB9D28DD,scePower_driver_UnregitserCallback
//	IMPORT_FUNC	"scePower_driver",0xD24E6BEB,scePower_driver__driver_D24E6BEB
//	IMPORT_FUNC	"scePower_driver",0x35B7662E,scePower_driver_GetSectionDescriptionEntry
//	IMPORT_FUNC	"scePower_driver",0xF9B4DEA1,scePower_driver_LimitPllClock
//	IMPORT_FUNC	"scePower_driver",0x843FBF43,scePower_driver_SetCpuClockFrequency
//	IMPORT_FUNC	"scePower_driver",0xB8D7B3FB,scePower_driver_SetBusClockFrequency
//	IMPORT_FUNC	"scePower_driver",0xFEE03A2F,scePower_driver_GetCpuClockFrequency
//	IMPORT_FUNC	"scePower_driver",0x478FE6F5,scePower_driver_GetBusClockFrequency
	IMPORT_FUNC	"scePower_driver",0xFDB5BFE9,scePower_driver_GetCpuClockFrequencyInt
	IMPORT_FUNC	"scePower_driver",0xBD681969,scePower_driver_GetBusClockFrequencyInt
//	IMPORT_FUNC	"scePower_driver",0xB1A52C83,scePower_driver_GetCpuClockFrequencyFloat
//	IMPORT_FUNC	"scePower_driver",0x9BADB3EB,scePower_driver_GetBusClockFrequencyFloat
	IMPORT_FUNC	"scePower_driver",0x737486F2,scePower_driver_SetClockFrequency
//	IMPORT_FUNC	"scePower_driver",0xE0B7A95D,scePower_driver_E0B7A95D
//	IMPORT_FUNC	"scePower_driver",0xC23AC778,scePower_driver_C23AC778
//	IMPORT_FUNC	"scePower_driver",0x23C31FFE,scePower_driver_VolatileMemLock
//	IMPORT_FUNC	"scePower_driver",0xFA97A599,scePower_driver_VolatileMemTryLock
//	IMPORT_FUNC	"scePower_driver",0xB3EDD801,scePower_driver_VolatileMemUnlock
