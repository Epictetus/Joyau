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

#ifndef JOYAU_LINE
#define JOYAU_LINE

#include "Drawable.hpp"

class Line: public Drawable
{
public:
   void setPoint(int x, int y);

   void setColor(OSL_COLOR col);
   void setGradient(OSL_COLOR col[2]);

   OSL_COLOR *getColors();
   Point getPoint() const { return Point(_x2, _y2); }

   void draw();
private:
   OSL_COLOR _col[2];
   int _x2, _y2;
};

VALUE Line_setPoint(VALUE self, VALUE x, VALUE y);

VALUE Line_setColor(VALUE self, VALUE col);
VALUE Line_setGradient(VALUE self, VALUE col);

VALUE Line_getColors(VALUE self);
VALUE Line_getPoint(VALUE self);

void defineLine();

#endif
