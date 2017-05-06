===================================
 PSP-HUD v2.02 - August, 31st 2008.
===================================

This is a plugin for PSP CWF 4.01M33-2 (not tested on earlier versions) that displays some basic info on screen:
- FPS
- CPU/BUS speed and CPU usage
- notification on CPU speed change
- battery status (percent and time left)
- local time
Besides displaying info on screen this plugin can:
- change cpu speed
- take a screenshot


Installation:
-------------
Put hud.prx and hud.lang in seplugins directory on your PSP memory stick 
and append ms0:/seplugins/hud.prx to vsh.txt, game.txt and pops.txt files.

Usage:
------
Default configuration is to show cpu speed change notification, battery percent left, local time in bottom right corner.
Default button setup is:
- NOTE+LTRIG to access configuration menu.
- NOTE+RTRIG to turn HUD off and change between display methods
  (this will only change method currently, it will not be saved, to save it you must enter menu)
- NOTE+UP to take a screenshot
When you exit menu configuration will be automatically saved in ms0:/seplugins/hud.cfg
If button configuration is changed and you can't enter menu anymore (or can't rembember) delete hud.cfg and restart PSP.

Display methods:
----------------
Method 1: this is a new method that is not available always, it hooks to a system API to get accurate FPS reading and to draw hud display,
          when current running application/game is not using this API, plugin fallbacks to method 2 and FPS will always display 59 or 60
Method 2: this is an old method (from pre 2.0 versions)

In most cases Method 1 gives better display and sometimes worse, if there is a problem with one you can switch between methods from menu or by
pressing NOTE+RTRIG (default key configuration).


Version History:
----------------

August, 31st 2008. v2.02
------------------------
- changed option 'SHOW HUD', besides switching off HUD display it is used to switch between display methods
- new every info part has it's own custom position
- moving analog stick in menu, when positioned on FPS,CPU,BATTERY or TIME, will automatically switch that item to custom position

August, 31st 2008. v2.01
------------------------
- fixed a bug that caused a crashing when exiting a homebrew
- screenshots are placed in ms0:/PICTURE/ folder
- added french lanuguage (thanks to Mizou93)

August, 30th 2008. v2.0
-----------------------
- new drawing method to reduce flickering (thanks to Raphael for suggestion)
- when in options keys are disabled for background process
- to reduce memory usade languages are not stored in memory and are loaded on overy language change (this is causing slow language change)
- cpu and bus speed are joined in one option
- show time option is expanded with more formats (24h, 12h)
- alignment is no longer unique for all options, now every option can have different alignment
- order of displayed items can be changed (useful when displaying multiple options in one place)
- new option: display fps
- new option: cpu speed notification, if on, when cpu speed changes old speed and new speed will be shown for a few seconds and will be blinking
- new option: cpu speed changer for xmb/game/pops, when set it will monitor cpu speed continouosly and adjust it to specified speed when it is different than specified
- new option: take a screenshot without hud info


January, 18th 2008. v1.31
-------------------------
- updated for 3.80M33-4 (actually droped previously used NIDs for 3.71 and now using NIDs from pspsdk)
- new translations: german (two translations), dutch, italian, spanish, polish and turkish
  
  I won't be accepting any more translations and I'm planing to drop translations for future versions (it's easier for me).

January, 10th 2008. v1.3
------------------------
- added language menu option
- added serbian and french language (thanks to Mizou93 for french translation)

January, 7th 2008. v1.22
------------------------
- fixed cpu usage display

January, 6th 2008. v1.21
------------------------
- slightly improved display

January, 5th 2008. v1.2
-----------------------
- buttons for menu and HUD are configurable within configuration menu
- add custom alignment (adjust with analog)

January, 4th 2008. v1.1
-----------------------
- added cpu usage and bus speed
- menu is now accessed with NOTE+LTRIG
- toggle HUD with NOTE+RTRIG
- redesigned menu selection (looks nice now)

January, 4th 2008. v1.0
-----------------------
- initial version