#include <pspsdk.h>
#include <pspkernel.h>
#include <psputilsforkernel.h>
#include <pspctrl.h>
#include <pspmoduleinfo.h>
#include <stdio.h>
#include <string.h>

struct SyscallHeader
{
	void *unk;
	unsigned int basenum;
	unsigned int topnum;
	unsigned int size;
};
/*
void saveLibraryEntryTableInfo(SceUID uid, char *filename)
{
	struct SceLibraryEntryTable *entry;
	SceModule *pMod;
	void *entTab;
	int entLen;

	pMod = sceKernelFindModuleByUID(uid);
	if(pMod != NULL)
	{
		SceUID fd = sceIoOpen(filename, PSP_O_WRONLY | PSP_O_CREAT, 0777);
		if(fd >= 0) {

			int i = 0;

			entTab = pMod->ent_top;
			entLen = pMod->ent_size;
			while(i < entLen)
			{
				entry = (struct SceLibraryEntryTable *) (entTab + i);

				if(entry->libname) {
					sceIoWrite(fd, entry->libname, strlen(entry->libname));
					sceIoWrite(fd, "\x0A\0x0D", 2);
				}

				i += (entry->len * 4);

			}
			sceIoClose(fd);
		}
	}
}
*/

static struct SceLibraryEntryTable *_libsFindLibrary(SceUID uid, const char *library)
{
	struct SceLibraryEntryTable *entry;
	SceModule *pMod;
	void *entTab;
	int entLen;

	pMod = sceKernelFindModuleByUID(uid);
	if(pMod != NULL)
	{
		int i = 0;

		entTab = pMod->ent_top;
		entLen = pMod->ent_size;
		while(i < entLen)
		{
			entry = (struct SceLibraryEntryTable *) (entTab + i);

			if((entry->libname) && (strcmp(entry->libname, library) == 0))
			{
				return entry;
			}
			else if(!entry->libname && !library)
			{
				return entry;
			}

			i += (entry->len * 4);
		}
	}

	return NULL;
}

void* libsFindExportAddrByNid(SceUID uid, const char *library, u32 nid)
{
	struct SceLibraryEntryTable *entry;
	u32 *addr = NULL;

	entry = _libsFindLibrary(uid, library);
	if(entry)
	{
		int count;
		int total;
		unsigned int *vars;

		total = entry->stubcount + entry->vstubcount;
		vars = entry->entrytable;

		if(entry->stubcount > 0)
		{
			for(count = 0; count < entry->stubcount; count++)
			{
				if(vars[count] == nid)
				{
					return &vars[count+total];
				}
			}
		}
	}

	return addr;
}

u32 libsNameToNid(const char *name)
{
	u8 digest[20];
	u32 nid;

	if(sceKernelUtilsSha1Digest((u8 *) name, strlen(name), digest) >= 0)
	{
		nid = digest[0] | (digest[1] << 8) | (digest[2] << 16) | (digest[3] << 24);
		return nid;
	}

	return 0;
}

u32 libsFindExportByName(SceUID uid, const char *library, const char *name)
{
	u32 nid = libsNameToNid(name);
	u32 *addr = libsFindExportAddrByNid(uid, library, nid);

	if(!addr)
	{
		return 0;
	}

	return *addr;
}

void *find_syscall_addr(u32 addr)
{
	struct SyscallHeader *head;
	u32 *syscalls;
	void **ptr;
	int size;
	int i;

	asm(
			"cfc0 %0, $12\n"
			: "=r"(ptr)
	   );

	if(!ptr)
	{
		return NULL;
	}

	head = (struct SyscallHeader *) *ptr;
	syscalls = (u32*) (*ptr + 0x10);
	size = (head->size - 0x10) / sizeof(u32);

	for(i = 0; i < size; i++)
	{
		if(syscalls[i] == addr)
		{
			return &syscalls[i];
		}
	}

	return NULL;
}

static void *apiHookAddr(u32 *addr, void *func)
{
	int intc;

	if(!addr)
	{
		return NULL;
	}

	intc = pspSdkDisableInterrupts();
	*addr = (u32) func;
	sceKernelDcacheWritebackInvalidateRange(addr, sizeof(addr));
	sceKernelIcacheInvalidateRange(addr, sizeof(addr));
	pspSdkEnableInterrupts(intc);

	return addr;
}

u32 apiHookByName(SceUID uid, const char *library, const char *name, void *func)
{
	u32 addr;

	addr = libsFindExportByName(uid, library, name);
	if(addr)
	{
		if(!apiHookAddr(find_syscall_addr(addr), func))
		{
			addr = 0;
		}
	}

	return addr;
}


