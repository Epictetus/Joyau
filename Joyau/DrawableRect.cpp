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

#include "DrawableRect.hpp"

void DrawableRect::setColor(OSL_COLOR col)
{
   for (int i = 0; i < 4; ++i)
      _col[i] = col;
}

void DrawableRect::setGradient(OSL_COLOR col[4])
{
   for (int i = 0; i < 4; ++i)
      _col[i] = col[i];
}
   
void DrawableRect::resize(int w, int h)
{
   _w = w;
   _h = h;

   _x2 = getX() + _w;
   _y2 = getY() + _h;
}

void DrawableRect::setCorner(int x, int y)
{
   _x2 = x;
   _y2 = 2;

   _w = _x2 - getX();
   _h = _y2 - getY();
}

void DrawableRect::draw()
{
   if (filled)
      oslDrawGradientRect(getX(), getY(), _x2, _y2, 
			  _col[0], _col[1], _col[2], _col[3]);
   else
      oslDrawRect(getX(), getY(), _x2, _y2, _col[0]);
}

VALUE DrawableRect_toggleFilled(VALUE self)
{
   DrawableRect &ref = getRef<DrawableRect>(self);
   ref.toggleFilled();
   return Qnil;
}

VALUE DrawableRect_setColor(VALUE self, VALUE col)
{
   DrawableRect &ref = getRef<DrawableRect>(self);
   OSL_COLOR _col = hash2col(col);

   ref.setColor(_col);
   return Qnil;
}

VALUE DrawableRect_setGradient(VALUE self, VALUE col)
{
   DrawableRect &ref = getRef<DrawableRect>(self);
   OSL_COLOR _col[4];
   for (int i = 0; i < 4; ++i)
      _col[i] = hash2col(rb_ary_entry(col, INT2FIX(i)));

   ref.setGradient(_col);
   return Qnil;
}

VALUE DrawableRect_resize(VALUE self, VALUE w, VALUE h)
{
   DrawableRect &ref = getRef<DrawableRect>(self);
   int _w = FIX2INT(w);
   int _h = FIX2INT(h);

   ref.resize(_w, _h);
   return Qnil;
}

VALUE DrawableRect_setCorner(VALUE self, VALUE x, VALUE y)
{
   DrawableRect &ref = getRef<DrawableRect>(self);
   int _x = FIX2INT(x);
   int _y = FIX2INT(y);

   ref.setCorner(_x, _y);
   return Qnil;
}

void defineDrawableRect()
{
   VALUE cDrawable = getClass("Drawable");
   VALUE cDrawableRect = defClass<DrawableRect>("DrawableRect", cDrawable);
   defMethod(cDrawableRect, "setCorner", DrawableRect_setCorner, 2);
   defMethod(cDrawableRect, "resize", DrawableRect_resize, 2);
   defMethod(cDrawableRect, "setColor", DrawableRect_setColor, 1);
   defMethod(cDrawableRect, "setGradient", DrawableRect_setGradient, 1);
   defMethod(cDrawableRect, "toggleFilled", DrawableRect_toggleFilled, 0);
}
