	.set noreorder

#include "pspimport.s"

	IMPORT_START	"sceRtc_driver",0x00010000
//	IMPORT_FUNC	"sceRtc_driver",0xC41C2853,sceRtc_driver_GetTickResolution
//	IMPORT_FUNC	"sceRtc_driver",0x3F7AD767,sceRtc_driver_GetCurrentTick
//	IMPORT_FUNC	"sceRtc_driver",0x029CA3B3,sceRtc_driver_GetAccumlativeTime
//	IMPORT_FUNC	"sceRtc_driver",0x4CFA57B0,sceRtc_driver_GetCurrentClock
	IMPORT_FUNC	"sceRtc_driver",0xE7C27D1B,sceRtc_driver_GetCurrentClockLocalTime
//	IMPORT_FUNC	"sceRtc_driver",0x34885E0D,sceRtc_driver_ConvertUtcToLocalTime
//	IMPORT_FUNC	"sceRtc_driver",0x779242A2,sceRtc_driver_ConvertLocalTimeToUTC
//	IMPORT_FUNC	"sceRtc_driver",0x42307A17,sceRtc_driver_IsLeapYear
//	IMPORT_FUNC	"sceRtc_driver",0x05EF322C,sceRtc_driver_GetDaysInMonth
//	IMPORT_FUNC	"sceRtc_driver",0x57726BC1,sceRtc_driver_GetDayOfWeek
//	IMPORT_FUNC	"sceRtc_driver",0x4B1B5E82,sceRtc_driver_CheckValid
//	IMPORT_FUNC	"sceRtc_driver",0x3A807CC8,sceRtc_driver_SetTime_t
//	IMPORT_FUNC	"sceRtc_driver",0x27C4594C,sceRtc_driver_GetTime_t
//	IMPORT_FUNC	"sceRtc_driver",0xF006F264,sceRtc_driver_SetDosTime
//	IMPORT_FUNC	"sceRtc_driver",0x36075567,sceRtc_driver_GetDosTime
//	IMPORT_FUNC	"sceRtc_driver",0x7ACE4C04,sceRtc_driver_SetWin32FileTime
//	IMPORT_FUNC	"sceRtc_driver",0xCF561893,sceRtc_driver_GetWin32FileTime
//	IMPORT_FUNC	"sceRtc_driver",0x7ED29E40,sceRtc_driver_SetTick
//	IMPORT_FUNC	"sceRtc_driver",0x6FF40ACC,sceRtc_driver_GetTick
//	IMPORT_FUNC	"sceRtc_driver",0x9ED0AE87,sceRtc_driver_CompareTick
//	IMPORT_FUNC	"sceRtc_driver",0x44F45E05,sceRtc_driver_TickAddTicks
//	IMPORT_FUNC	"sceRtc_driver",0x26D25A5D,sceRtc_driver_TickAddMicroseconds
//	IMPORT_FUNC	"sceRtc_driver",0xF2A4AFE5,sceRtc_driver_TickAddSeconds
//	IMPORT_FUNC	"sceRtc_driver",0xE6605BCA,sceRtc_driver_TickAddMinutes
//	IMPORT_FUNC	"sceRtc_driver",0x26D7A24A,sceRtc_driver_TickAddHours
//	IMPORT_FUNC	"sceRtc_driver",0xE51B4B7A,sceRtc_driver_TickAddDays
//	IMPORT_FUNC	"sceRtc_driver",0xCF3A2CA8,sceRtc_driver_TickAddWeeks
//	IMPORT_FUNC	"sceRtc_driver",0xDBF74F1B,sceRtc_driver_TickAddMonths
//	IMPORT_FUNC	"sceRtc_driver",0x42842C77,sceRtc_driver_TickAddYears
//	IMPORT_FUNC	"sceRtc_driver",0xC663B3B9,sceRtc_driver_FormatRFC2822
//	IMPORT_FUNC	"sceRtc_driver",0x7DE6711B,sceRtc_driver_FormatRFC2822LocalTime
//	IMPORT_FUNC	"sceRtc_driver",0x0498FB3C,sceRtc_driver_FormatRFC3339
//	IMPORT_FUNC	"sceRtc_driver",0x27F98543,sceRtc_driver_FormatRFC3339LocalTime
//	IMPORT_FUNC	"sceRtc_driver",0xDFBC5F16,sceRtc_driver_ParseDateTime
//	IMPORT_FUNC	"sceRtc_driver",0x28E1E988,sceRtc_driver_ParseRFC3339
//	IMPORT_FUNC	"sceRtc_driver",0x011F03C1,sceRtc_driver_GetAccumulativeTime
//	IMPORT_FUNC	"sceRtc_driver",0x17C26C00,sceRtc_driver_SetCurrentSecureTick
//	IMPORT_FUNC	"sceRtc_driver",0x1909C99B,sceRtc_driver_SetTime64_t
//	IMPORT_FUNC	"sceRtc_driver",0x203CEB0D,sceRtc_driver_GetLastReincarnatedTime
//	IMPORT_FUNC	"sceRtc_driver",0x48D07D70,sceRtc_driver_Resume
//	IMPORT_FUNC	"sceRtc_driver",0x62685E98,sceRtc_driver_GetLastAdjustedTime
//	IMPORT_FUNC	"sceRtc_driver",0x6A676D2D,sceRtc_driver_UnregisterCallback
//	IMPORT_FUNC	"sceRtc_driver",0x759937C5,sceRtc_driver_SetConf
//	IMPORT_FUNC	"sceRtc_driver",0x7D1FBED3,sceRtc_driver_SetAlarmTick
//	IMPORT_FUNC	"sceRtc_driver",0x81FCDA34,sceRtc_driver_IsAlarmed
//	IMPORT_FUNC	"sceRtc_driver",0x912BEE56,sceRtc_driver_Init
//	IMPORT_FUNC	"sceRtc_driver",0x9763C138,sceRtc_driver_SetCurrentTick
//	IMPORT_FUNC	"sceRtc_driver",0x9CC2797E,sceRtc_driver_Suspend
//	IMPORT_FUNC	"sceRtc_driver",0xB44BDAED,sceRtc_driver_GetCurrentSecureTick
//	IMPORT_FUNC	"sceRtc_driver",0xC2DDBEB5,sceRtc_driver_GetAlarmTick
//	IMPORT_FUNC	"sceRtc_driver",0xC499AF2F,sceRtc_driver_Reset
//	IMPORT_FUNC	"sceRtc_driver",0xCE27DE2F,sceRtc_driver_End
//	IMPORT_FUNC	"sceRtc_driver",0xE1C93E47,sceRtc_driver_GetTime64_t
//	IMPORT_FUNC	"sceRtc_driver",0xF0B5571C,sceRtc_driver_Synchronize
//	IMPORT_FUNC	"sceRtc_driver",0xFB3B18CD,sceRtc_driver_RegisterCallback
