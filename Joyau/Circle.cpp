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

#include "Circle.hpp"

void Circle::setCenter(int x, int y)
{
   centerX = x;
   centerY = y;

   setPos(centerX - _r, centerY - _r);
}

void Circle::setRadius(int r)
{
   _r = r;
   // We'll call setPos here to, so setCenter and setRadius can be called
   // in any order.
   setPos(centerX - _r, centerY - _r);
   
   _w = _r * 2;
   _h = _r * 2;
}

void Circle::draw()
{
   // if the user has used setPos or move, we should fix centerX and centerY
   if (centerX != getX() + _r ||
       centerY != getY() + _r)
      setCenter(getX() + _r, getY() + _r);
   if (filled)
      oslDrawFillCircle(centerX, centerY, _r, _col);
   else
      oslDrawCircle(centerX, centerY, _r, _col);
}

void Circle::setColor(OSL_COLOR col)
{
   _col = col;
}

Rect Circle::boundingRect()
{
   if (centerX != getX() + _r ||
       centerY != getY() + _r)
      setCenter(getX() + _r, getY() + _r);
   return Rect(getX(), getY(), getW(), getH());
}

VALUE Circle_toggleFilled(VALUE self)
{
   Circle &ref = getRef<Circle>(self);

   ref.toggleFilled();
   return Qnil;
}

VALUE Circle_setCenter(VALUE self, VALUE x, VALUE y)
{
   Circle &ref = getRef<Circle>(self);
   int _x = FIX2INT(x);
   int _y = FIX2INT(y);

   ref.setCenter(_x, _y);
   return Qnil;
}

VALUE Circle_setRadius(VALUE self, VALUE r)
{
   Circle &ref = getRef<Circle>(self);
   int _r = FIX2INT(r);

   ref.setRadius(_r);
   return Qnil;
}

VALUE Circle_setColor(VALUE self, VALUE col)
{
   Circle &ref = getRef<Circle>(self);
   OSL_COLOR _col = hash2col(col);

   ref.setColor(_col);
   return Qnil;
}

VALUE Circle_getCenterX(VALUE self)
{
   Circle &ref = getRef<Circle>(self);
   return INT2FIX(ref.getCenterX());
}

VALUE Circle_getCenterY(VALUE self)
{
   Circle &ref = getRef<Circle>(self);
   return INT2FIX(ref.getCenterX());
}

VALUE Circle_getRadius(VALUE self)
{
   Circle &ref = getRef<Circle>(self);
   return INT2FIX(ref.getRadius());
}

void defineCircle()
{
   VALUE cDrawable = getClass("Drawable");
   VALUE cCircle = defClass<Circle>("Circle", cDrawable);
   defMethod(cCircle, "setColor", Circle_setColor, 1);
   defMethod(cCircle, "setRadius", Circle_setRadius, 1);
   defMethod(cCircle, "setCenter", Circle_setCenter, 2);
   defMethod(cCircle, "toggleFilled", Circle_toggleFilled, 0);
   defMethod(cCircle, "getCenterX", Circle_getCenterX, 0);
   defMethod(cCircle, "getCenterY", Circle_getCenterY, 0);
   defMethod(cCircle, "getRadius", Circle_getRadius, 0);
}
