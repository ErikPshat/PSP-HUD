#ifndef __CONFIG_H__
#define __CONFIG_H__

typedef struct CONFIG {
	int language;
	int showHUD;     // 0 - off, 1 - method 1 (new), 2 - method 2 (old)
	int showFPS;     // 0 - off, 1 - number only, 2 - number and prefix, 3 - number and suffix
	int showCPU;     // 0 - off, 1 - cpu, 2 - cpu/bus, 3 - usage, 4 - cpu/usage, 5 - cpu/bus/usage
	int showCPUNotofication; // 0 - 0ff, 1 - cpu, 2 - cpu/bus
	int showBattery; // 0 - off, 1 - percent, 2 - time, 3 - percent/time
	int showTime;    // 0 - off, 1 - icon,24h, 2 - icon,12h,suffix, 3 - icon,12h,no suffix, 4 - 24h, 5 - 12h,suffix, 6 - 12h,no suffix
	int order[4];
	int alignment[4]; // 0 - top left, 1 - top center, 2 - top right, 3 - bottom left, 4 - bottom center, 5 - bottom right, 6 - custom
	float custom_x[4];
	float custom_y[4];
	unsigned int speedXMB; // 0 - default, ...
	unsigned int speedGAME; // 0 - default, ...
	unsigned int speedPOPS; // 0 - defailt, ...
	unsigned int buttonCombo;
	unsigned int buttonMenu;
	unsigned int buttonHUD;
	unsigned int buttonScreenshot;
} CONFIG;

void load(CONFIG *config);
void save(CONFIG *config);

#endif
