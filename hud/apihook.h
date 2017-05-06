#ifndef __APIHOOK_H__
#define __APIHOOK_H__

//void saveLibraryEntryTableInfo(SceUID uid, char *filename);
u32 libsFindExportByName(SceUID uid, const char *library, const char *name);
u32 apiHookByName(SceUID uid, const char *library, const char *name, void *func);

#endif
