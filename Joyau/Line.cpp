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

#include "Line.hpp"

Line::Line(): 
   Shape(2) 
{
   for (int i = 0; i < 2; ++i)
      _col[i] = RGBA(255, 255, 255, 255);
   setClass("Line");
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

bool Line::collide(Drawable &item)
{
   // When the lines are horizontal/vertical, we call the super method.
   if (_x == _x2 || _y == _y2)
      return Drawable::collide(item);

   // The first thing we should check is whether we are insde
   // the other object.
   if (item.isOn(_x, _y) || item.isOn(_x2, _y2))
      return true;

   // now, we need to calculate the equation of this Line.
   float k = ((float)(_y - _y2) / (float)(_x - _x2));
   float t = ((float)(_x2 * _y - _x * _y2) / (float)(_x2 - _x));

   // We'll start by checking the vertical lines.
   float sol = k * (float)item.getX() + t;
   if (sol >= item.getY() && sol <= item.getY() + item.getH() &&
       sol >= _x && sol <= _x2)
      return true;

   sol = k * (float)item.getX() + item.getW() + t;
   if (sol >= item.getY() && sol <= item.getY() + item.getH() &&
       sol >= _x && sol <= _x2)
      return true;

   // Now we'll check the horizontal lines.
   sol = ((float)item.getY() - t) / k;
   if (sol >= item.getX() && sol <= item.getX() + item.getW() &&
       sol >= _y && sol <= _y2)
      return true;

   sol = ((float)(item.getY() + item.getH()) - t) / k;
   if (sol >= item.getX() && sol <= item.getX() + item.getW() &&
       sol >= _y && sol <= _y2)
      return true;

   return false;
}

bool Line::isOn(int x, int y)
{
   if (_x == _x2)
      return y == _y && x >= _x && x <= _x + _w;
   else if (_y == _y2)
      return x == _x && y >= _y && y <= _y + _h;

   float k = ((float)(_y - _y2) / (float)(_x - _x2));
   float t = ((float)(_x2 * _y - _x * _y2) / (float)(_x2 - _x));

   float sol = k * (float)x + t;
   return sol > y - 0.7 && sol < y + 0.7 &&
	 x >= _x &&
	 y >= _y &&
	 x <= _x2 &&
	 y <= _y2;
}

void Line::draw()
{
   oslDrawGradientLine (getX(), getY(), _x2, _y2, 
			_col[0], _col[1]);
}

/*
  Document-class: Joyau::Line

  Class used in order to draw lines.
*/

/*
  call-seq: setPoint(x, y)

  Sets the line's second point (the first one is set with Drawable#setPos).
*/
VALUE Line_setPoint(VALUE self, VALUE x, VALUE y)
{
   Line &ref = getRef<Line>(self);
   int _x = FIX2INT(x);
   int _y = FIX2INT(y);

   ref.setPoint(_x, _y);
   return Qnil;
}

/*
  call-seq: point=(p)

  Sets the line's second point (the first one is set with Drawable#setPos).
*/
VALUE Line_setPointPoint(VALUE self, VALUE p)
 {
   Line &ref = getRef<Line>(self);
   Point &point = getRef<Point>(p);
   
   ref.setPoint(point);
   return p;
}

/*
  Returns the line's second point.
*/
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
   defAlias(cLine, "setPoint", "set_point");
}
