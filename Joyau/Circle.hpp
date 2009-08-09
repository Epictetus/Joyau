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

#ifndef JOYAU_CIRCLE
#define JOYAU_CIRCLE

#include "Shape.hpp"

class Circle: public FillableShape
{
public:
   void setCenter(int x, int y);
   void setRadius(int r);
   
   void draw();

   int getCenterX() const { return centerX; }
   int getCenterY() const { return centerY; }
   int getRadius() const { return _r; }
private:
   int centerX, centerY;
   int _r;
};

VALUE Circle_setCenter(VALUE self, VALUE x, VALUE y);
VALUE Circle_setRadius(VALUE self, VALUE r);

VALUE Circle_getCenterX(VALUE self);
VALUE Circle_getCenterY(VALUE self);
VALUE Circle_getRadius(VALUE self);

void defineCircle();

#endif
