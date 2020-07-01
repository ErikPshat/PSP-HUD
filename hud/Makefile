TARGET = hud
OBJS = main.o apihook.o buttons.o config.o draw.o lang.o screenshot.o exports.o sceCtrl_driver.o sceDisplay_driver.o scePower_driver.o sceRtc_driver.o sceUsb_driver.o

BUILD_PRX=1
USE_KERNEL_LIBC=1
USE_KERNEL_LIBS=1

CFLAGS = -Os -G0 -Wall -fno-strict-aliasing -fno-builtin-printf -D_PSPSLIM -D_ENGLISH_ONLY
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

INCDIR =
LIBDIR =
LDFLAGS = -mno-crt0 -nostartfiles
LIBS =
#LIBS = -lpspusb -lpspusbstor
#LIBS = -lpspsysmem_user 

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build_prx.mak

