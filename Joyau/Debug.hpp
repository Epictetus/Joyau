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

#ifndef JOYAU_DEBUG
#define JOYAU_DEBUG

#include "StdInclude.hpp"

VALUE Debug_init(VALUE self);

VALUE Debug_puts(int argc, VALUE *argv, VALUE self);
VALUE Debug_print(int argc, VALUE *argv, VALUE self);

VALUE Debug_clear(VALUE self);

VALUE Debug_enableBackground(VALUE self);
VALUE Debug_disableBackground(VALUE self);
VALUE Debug_setBackground(VALUE self, VALUE val);

VALUE Debug_setX(VALUE self, VALUE x);
VALUE Debug_setY(VALUE self, VALUE y);
VALUE Debug_setPos(VALUE self, VALUE pos);

VALUE Debug_x(VALUE self);
VALUE Debug_y(VALUE self);
VALUE Debug_pos(VALUE self);

VALUE Debug_setBgColor(VALUE self, VALUE col);
VALUE Debug_setTextColor(VALUE self, VALUE col);

void defineDebug();

#endif
