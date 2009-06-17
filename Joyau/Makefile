TARGET = Joyau
OBJS = main.o Manager.o Sprite.o Audio.o Graphics.o Keys.o

INCDIR =
CFLAGS = -O2 -G0 -Wall -D_PSP_
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR =
LIBS = -losl -lpng -lz -lpspsdk -lpspvfpu -lpspctrl -lpspumd -lpsprtc -lpsppower -lpspgu -lpspmpeg -lpspaudiocodec -lpspaudiolib -lpspaudio -lruby -lm -lstdc++
LDFLAGS =

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = Joyau

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
