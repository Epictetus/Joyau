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

#ifndef JOYAU_TRIANGLE
#define JOYAU_TRIANGLE

#include "Drawable.hpp"

class Triangle: public Drawable
{
public:
   void setPoints(int x1, int y1, int x2, int y2, int x3, int y3);

   void setColor(OSL_COLOR col);
   void setGradient(OSL_COLOR col[3]);
   
   void draw();
private:
   int x[3];
   int y[3];

   OSL_COLOR _col[3];
};

VALUE Triangle_setPoints(VALUE self, VALUE x1, VALUE y1, VALUE x2, VALUE y2,
			 VALUE x3, VALUE y3);

VALUE Triangle_setColor(VALUE self, VALUE col);
VALUE Triangle_setGradient(VALUE self, VALUE col);

void defineTriangle();

#endif
