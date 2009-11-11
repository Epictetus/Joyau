#Copyright (C) 2009 Verhetsel Kilian
#
#This program is free software; you can redistribute it and/or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation; either version 2 of the License, or
#(at your option) any later version.
#
#This program is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#GNU General Public License for more details.
#
#You should have received a copy of the GNU General Public License along
#with this program; if not, write to the Free Software Foundation, Inc.,
#51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

TARGET = Joyau

OBJS = main.o Manager.o Graphics.o Keys.o Sprite.o Audio.o Kernel.o \
	MessageBox.o Scrolling.o Particles.o GameMap.o CircleMenu.o \
	Timer.o Drawable.o DrawableRect.o Circle.o Line.o Triangle.o \
	DrawableText.o Shape.o Usb.o RubyDrawable.o MsgSelecter.o \
	MultiSelecter.o Font.o Console.o

OBJS += intrafont/libccc.o intrafont/intraFont.o
OBJS += console/console2d.o console/console.o console/librairies/framebuffer.o \
	console/librairies/danzeff.o console/librairies/graphics.o

INCDIR += ./intrafont/

CFLAGS += -O2 -G0 -Wall -D_PSP_ -DHAVE_STRUCT_TIMESPEC
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

ifdef JOYAU_PSPLINK
BUILD_PRX = 1
CFLAGS += -g
CXXFLAGS += -g
endif

LIBS = -lruby -lpthread-psp
LIBS +=	-losl -lpng -lz -lpspsdk -lpspvfpu -lpspctrl -lpspumd -lpsprtc \
	-lpsppower -lpspgu -lpspmpeg -lpspaudiocodec -lpspaudiolib -lalut \
	-lOpenAL32 -lvorbisfile -lvorbis -lpspaudio -lpspusb -lpspusbstor \
	-lpsphprm -lpsputility -lruby -lpspgum -lm -logg -lpsprtc -lstdc++

PSP_FW_VERSION = 371

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = Joyau
PSP_EBOOT_ICON = eboot/ICON0.png
PSP_EBOOT_PIC1 = eboot/PIC1.png

PSP_LARGE_MEMORY = 1

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak

check-syntax:
	$(CXX) $(CXXFLAGS) -Wextra -fsyntax-only $(CHK_SOURCES)

