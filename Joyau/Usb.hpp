/*Copyright (C) 2009-2010 Verhetsel Kilian

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.*/

#ifndef JOYAU_USB
#define JOYAU_USB

#include "StdInclude.hpp"

bool loadStartModule(const std::string &prx);

VALUE initUsb(VALUE self);
VALUE stopUsb(VALUE self);

VALUE usbConnect(VALUE self);
VALUE usbDisconnect(VALUE self);

VALUE usbState(VALUE self);

VALUE Usb_activated(VALUE self);
VALUE USb_established(VALUE self);
VALUE Usb_connected(VALUE self);

void defineUsb();

#endif
