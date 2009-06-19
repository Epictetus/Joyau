/*Copyright (C) 2009 Verhetsel Kilian

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

#ifndef __PSP_RUBY_USB__
#define __PSP_RUBY_USB__

#include "StdInclude.hpp"

VALUE Usb_Init(VALUE self);

VALUE Usb_mount(VALUE self);
VALUE Usb_umount(VALUE self);

VALUE Usb_state(VALUE self);

void defineUsb();

#endif
