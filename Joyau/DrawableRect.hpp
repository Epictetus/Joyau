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

#ifndef JOYAU_RECT
#define JOYAU_RECT

#include "Drawable.hpp"

class DrawableRect: public Drawable
{
public:
   DrawableRect(): filled(true) {}

   void toggleFilled() { filled = !filled; }
   void setColor(OSL_COLOR col);
   void setGradient(OSL_COLOR col[4]);
   
   void resize(int w, int h);

   void draw();

   void setCorner(int x, int y);

   OSL_COLOR *getColors();
   Point getCorner() const { return Point(_x2, _y2); }
private:
   bool filled;
   int _x2, _y2;
   OSL_COLOR _col[4];
};

VALUE DrawableRect_toggleFilled(VALUE self);

VALUE DrawableRect_setColor(VALUE self, VALUE col);
VALUE DrawableRect_setGradient(VALUE self, VALUE col);

VALUE DrawableRect_resize(VALUE self, VALUE w, VALUE h);

VALUE DrawableRect_setCorner(VALUE self, VALUE x, VALUE y);

VALUE DrawableRect_getColors(VALUE self);
VALUE DrawableRect_getCorner(VALUE self);

void defineDrawableRect();

#endif
