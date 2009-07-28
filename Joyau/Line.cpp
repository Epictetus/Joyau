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

void Line::setPoint(int x, int y)
{
   _x2 = x;
   _y2 = y;

   _w = _x2 - getX();
   _h = _y2 - getY();
}

void Line::setColor(OSL_COLOR col)
{
   for (int i = 0; i < 2; ++i)
      _col[i] = col;
}

void Line::setGradient(OSL_COLOR col[2])
{
   for (int i = 0; i < 2; ++i)
      _col[i] = col[i];
}

OSL_COLOR* Line::getColors()
{
   OSL_COLOR *col = new OSL_COLOR[2];
   for (int i = 0; i < 2; ++i)
      col[i] = _col[i];
   return col;
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

VALUE Line_setColor(VALUE self, VALUE col)
{
   Line &ref = getRef<Line>(self);
   OSL_COLOR _col = hash2col(col);

   ref.setColor(_col);
   return Qnil;
}

VALUE Line_setGradient(VALUE self, VALUE col)
{
   Line &ref = getRef<Line>(self);
   OSL_COLOR _col[2];

   for (int i = 0; i < 2; ++i)
      _col[i] = hash2col(rb_ary_entry(col, INT2FIX(i)));

   ref.setGradient(_col);
   return Qnil;
}

VALUE Line_getColors(VALUE self)
{
   Line &ref = getRef<Line>(self);
   OSL_COLOR *col = ref.getColors();
   VALUE array = rb_ary_new();
   for (int i = 0; i < 2; ++i)
      rb_ary_push(array, col2hash(col[i]));
   return array;
}

VALUE Line_getPoint(VALUE self)
{
   Line &ref = getRef<Line>(self);
   Point point = ref.getPoint();

   return createObject(getClass("Point"), point);
}

void defineLine()
{
   VALUE cDrawable = getClass("Drawable");
   VALUE cLine = defClass<Line>("Line", cDrawable);
   defMethod(cLine, "setColor", Line_setColor, 1);
   defMethod(cLine, "setGradient", Line_setGradient, 1);
   defMethod(cLine, "setPoint", Line_setPoint, 2);
   defMethod(cLine, "getColors", Line_getColors, 0);
   defMethod(cLine, "getPoint", Line_getPoint, 0);
}
