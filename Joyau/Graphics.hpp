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

#ifndef __GRAPHICS__
#define __GRAPHCIS__

#include "StdInclude.hpp"

VALUE Graphics_init(VALUE self);
VALUE Graphics_stop(VALUE self);

VALUE lib_start(VALUE self);
VALUE lib_stop(VALUE self);

VALUE Graphics_mayPlay(VALUE self);
VALUE Graphics_startDraw(VALUE self);
VALUE Graphics_endDraw(VALUE self);
VALUE Graphics_sync(VALUE self);

VALUE Graphics_drawLine(VALUE self, VALUE x1, VALUE y1, VALUE x2, 
			VALUE y2, VALUE color);
VALUE Graphics_drawRect(VALUE self, VALUE x1, VALUE y1, VALUE x2, 
			VALUE y2, VALUE color);
VALUE Graphics_drawFillRect(VALUE self, VALUE x1, VALUE y1, VALUE x2, 
			    VALUE y2, VALUE color);

VALUE Graphics_clear(VALUE self);

VALUE setTextBackground(VALUE self, VALUE color);
VALUE setTextColor(VALUE self, VALUE color);
VALUE drawText(VALUE self, VALUE x, VALUE y, VALUE text);

void defineGraphics();

#endif
