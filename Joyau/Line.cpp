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

#include "Line.hpp"

Line::Line(): 
   Shape(2) 
{
   for (int i = 0; i < 2; ++i)
      _col[i] = RGBA(255, 255, 255, 255);
}

void Line::setPoint(int x, int y)
{
   _x2 = x;
   _y2 = y;

   _w = _x2 - getX();
   _h = _y2 - getY();
}

void Line::setPoint(const Point &p)
{
   setPoint(p.x, p.y);
}

void Line::draw()
{
   oslDrawGradientLine (getX(), getY(), _x2, _y2, 
			_col[0], _col[1]);
}

VALUE Line_setPoint(VALUE self, VALUE x, VALUE y)
{
   Line &ref = getRef<Line>(self);
   int _x = FIX2INT(x);
   int _y = FIX2INT(y);

   ref.setPoint(_x, _y);
   return Qnil;
}

VALUE Line_setPointPoint(VALUE self, VALUE p)
{
   Line &ref = getRef<Line>(self);
   Point &point = getRef<Point>(p);
   
   ref.setPoint(point);
   return Qnil;
}

VALUE Line_getPoint(VALUE self)
{
   Line &ref = getRef<Line>(self);
   Point point = ref.getPoint();

   return createObject(getClass("Point"), point);
}

void defineLine()
{
   VALUE cLine = defClass<Line>("Line", "Shape");
   defMethod(cLine, "setPoint", Line_setPoint, 2);
   defMethod(cLine, "point=", Line_setPointPoint, 1);
   defMethod(cLine, "getPoint", Line_getPoint, 0);

   defAlias(cLine, "getPoint", "point");
}
