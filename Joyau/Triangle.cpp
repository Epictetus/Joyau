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

#include "Triangle.hpp"

Triangle::Triangle():
   Shape(3)
{
   for (int i = 0; i < 3; ++i)
      _col[i] = RGBA(255, 255, 255, 255);
   setClass("Triangle");
}

void Triangle::setPoints(int x1, int y1, int x2, int y2, int x3, int y3)
{
   x[0] = x1;
   x[1] = x2;
   x[2] = x3;

   y[0] = y1;
   y[1] = y2;
   y[2] = y3;

   setPos(x[0], y[0]);

   int maxX = -1000;
   int maxY = -1000;
   int minX = 10000;
   int minY = 10000;

   for (int i = 0; i < 3; ++i)
   {
      if (maxX < x[i])
	 maxX = x[i];
      if (maxY < y[i])
	 maxY = y[i];

      if (minX > x[i])
	 minX = x[i];
      if (minY > y[i])
	 minY = y[i];
   }

   _w = maxX - minX;
   _h = maxY - minY;
}

Point* Triangle::getPoints() const
{
   Point *points = new Point[3];
   for (int i = 0; i < 3; ++i)
      points[i] = Point(x[i], y[i]);
   return points;
}

void Triangle::draw()
{
   oslDrawGradientTriangle(x[0], y[0], x[1], y[1], x[2], y[2], 
			    _col[0], _col[1], _col[2]);
}

void Triangle::move(int x_pos, int y_pos)
{
   Drawable::move(x_pos, y_pos); // For cancel move
   for (int i = 0; i < 3; ++i) {
      x[i] += x_pos;
      y[i] += y_pos;
   }
}

VALUE Triangle_setPoints(VALUE self, VALUE x1, VALUE y1, VALUE x2, VALUE y2,
			 VALUE x3, VALUE y3)
{
   Triangle &ref = getRef<Triangle>(self);

   int _x1 = INT2FIX(x1);
   int _x2 = INT2FIX(x2);
   int _x3 = INT2FIX(x3);

   int _y1 = INT2FIX(y1);
   int _y2 = INT2FIX(y2);
   int _y3 = INT2FIX(y3);

   ref.setPoints(_x1, _y1, _x2, _y2, _x3, _y3);
   return Qnil;
}

VALUE Triangle_getPoints(VALUE self)
{
   Triangle &ref = getRef<Triangle>(self);
   Point *points = ref.getPoints();

   VALUE hash = rb_ary_new();
   for (int i = 0; i < 3; ++i)
      rb_ary_push(hash, createObject(getClass("Point"), points[i]));

   return hash;
}

void defineTriangle()
{
   VALUE cTriangle = defClass<Triangle>("Triangle", "Shape");
   defMethod(cTriangle, "setPoints", Triangle_setPoints, 6);
   defMethod(cTriangle, "getPoints", Triangle_getPoints, 0);

   defAlias(cTriangle, "getPoints", "points");
}
