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

OBJS = main.o Manager.o Graphics.o Keys.o Sprite.o Audio.o Kernel.o Usb.o \
	MessageBox.o Scrolling.o RubyWrapper.o

INCDIR =
CFLAGS += -O2 -G0 -Wall -D_PSP_
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR =
LIBS = -losl -lpng -lz -lpspsdk -lpspvfpu -lpspusb -lpspusbstor -lpspctrl \
	-lpspumd -lpsprtc -lpsppower -lpspgu -lpspmpeg -lpspaudiocodec \
	-lpspaudiolib -lpspaudio -lruby -lm -lstdc++
LDFLAGS =

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = Joyau
PSP_EBOOT_ICON = eboot/ICON0.png
PSP_EBOOT_PIC1 = eboot/PIC1.png

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
