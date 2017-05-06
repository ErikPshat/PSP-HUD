#include <string.h>
#include "main.h"
#include "config.h"

char CONFIG_ID[6] = { 'H', 'U', 'D', '2', '0', '2' };

CONFIG lastConfig;

void load(CONFIG *config)
{
	if(config != NULL) {
		SceUID fd = sceIoOpen("ms0:/seplugins/hud.cfg", PSP_O_RDONLY, 0777);
		if(fd >= 0) {
			char ID[6];
			sceIoRead(fd, ID, 6);
			if(ID[0] == CONFIG_ID[0] && ID[1] == CONFIG_ID[1] && ID[2] == CONFIG_ID[2] && ID[3] == CONFIG_ID[3] && ID[4] == CONFIG_ID[4] && ID[5] == CONFIG_ID[5]) {
				sceIoRead(fd, config, sizeof(CONFIG));
			}
			sceIoClose(fd);
		}
		memcpy(&lastConfig, config, sizeof(CONFIG));
	}
}

void save(CONFIG *config)
{
	if(config != NULL) {
		if(memcmp(&lastConfig, config, sizeof(CONFIG)) != 0) {
			SceUID fd = sceIoOpen("ms0:/seplugins/hud.cfg", PSP_O_WRONLY | PSP_O_CREAT, 0777);
			if(fd >= 0) {
				sceIoWrite(fd, CONFIG_ID, 6);
				sceIoWrite(fd, config, sizeof(CONFIG));
				sceIoClose(fd);
			}
			memcpy(&lastConfig, config, sizeof(CONFIG));
		}
	}
}
