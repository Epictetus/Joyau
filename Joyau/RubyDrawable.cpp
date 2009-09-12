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

#include "RubyDrawable.hpp"

template<> VALUE wrap<RubyDrawable>(int argc, VALUE *argv, VALUE info)
{
   RubyDrawable *ptr = new RubyDrawable;

   VALUE tdata = Data_Wrap_Struct(info, 0, wrapped_free<RubyDrawable>, ptr);
   ptr->setSelf(tdata);

   return tdata;
}

void RubyDrawable::draw()
{
   rb_funcall(self, getFunc("draw"), 0);
}

Rect RubyDrawable::boundingRect() const
{
   VALUE ret = rb_funcall(self, getFunc("boundingRect"), 0);
   return getRef<Rect>(ret);
}

void RubyDrawable::move(int x, int y)
{
   rb_funcall(self, getFunc("move"), 2, INT2FIX(x), INT2FIX(y));
}

bool RubyDrawable::collide(Drawable &item)
{
   VALUE drawable = item.toRuby();
   VALUE ret = rb_funcall(self, getFunc("collide"), 1, drawable);
   return ret == Qtrue;
}

bool RubyDrawable::isOn(int x, int y)
{
   VALUE xArg = INT2FIX(x);
   VALUE yArg = INT2FIX(y);
   VALUE ret = rb_funcall(self, getFunc("isOn"), 2, xArg, yArg);
   return ret == Qtrue;
}

void RubyDrawable::resize(int w, int h)
{
   _w = w;
   _h = h;
}

VALUE RubyDrawable_draw(VALUE self)
{
   rb_notimplement(); // Of course, I can't guess that...
   return Qnil;
}

VALUE RubyDrawable_boundingRect(VALUE self)
{
   RubyDrawable &ref = getRef<RubyDrawable>(self);
   Rect rect;

   rect.x = ref.getX();
   rect.y = ref.getY();
   rect.w = ref.getW();
   rect.h = ref.getH();

   return createObject(getClass("Rect"), rect);
}

VALUE RubyDrawable_move(VALUE self, VALUE x, VALUE y)
{
   RubyDrawable &ref = getRef<RubyDrawable>(self);
   
   ref.setX(ref.getX() + FIX2INT(x));
   ref.setY(ref.getY() + FIX2INT(y));

   return Qnil;
}

VALUE RubyDrawable_collide(VALUE self, VALUE item)
{
   RubyDrawable &ref = getRef<RubyDrawable>(self);
   Drawable &draw = getRef<Drawable>(item);
   
   Rect rect1(ref.getX(), ref.getY(), ref.getW(), ref.getH());
   Rect rect2 = draw.boundingRect();

   Point points1[4];
   points1[0] = rect1.top_right();
   points1[1] = rect1.top_left();
   points1[2] = rect1.bottom_left();
   points1[3] = rect1.bottom_right();

   Point points2[4];
   points2[0] = rect2.top_right();
   points2[1] = rect2.top_left();
   points2[2] = rect2.bottom_left();
   points2[3] = rect2.bottom_right();

   for (int i = 0; i < 4; ++i)
   {
      int x1 = rect1.x;
      int x2 = rect1.x + rect1.w;
      int y1 = rect1.y;
      int y2 = rect1.y + rect1.h;

      if (points1[i].x >= x1 &&
	  points1[i].x <= x2 &&
	  points1[i].y >= y1 &&
	  points1[i].y <= y2)
	 return Qtrue;
      if (draw.isOn(points2[i]))
	 return Qtrue;
   }
   return Qfalse;
}

VALUE RubyDrawable_isOn(VALUE self, VALUE x, VALUE y)
{
   RubyDrawable &ref = getRef<RubyDrawable>(self);
   
   Rect rect(ref.getX(), ref.getY(), ref.getW(), ref.getH());
   int x1 = rect.x;
   int x2 = rect.x + rect.w;
   int y1 = rect.y;
   int y2 = rect.y + rect.h;

   if (FIX2INT(x) >= x1 &&
       FIX2INT(x) <= x2 &&
       FIX2INT(y) >= y1 &&
       FIX2INT(y) <= y2)
      return Qtrue;
   return Qfalse;
}

VALUE RubyDrawable_resize(VALUE self, VALUE w, VALUE h)
{
   RubyDrawable &ref = getRef<RubyDrawable>(self);

   ref.resize(FIX2INT(w), FIX2INT(h));
   return Qnil;
}

void defineRubyDrawable()
{
   VALUE cRubyDrawable = defClass<RubyDrawable>("RubyDrawable", "Drawable");

   defMethod(cRubyDrawable, "draw", RubyDrawable_draw, 0);
   defMethod(cRubyDrawable, "collide", RubyDrawable_collide, 1);
   defMethod(cRubyDrawable, "isOn", RubyDrawable_isOn, 2);
   defMethod(cRubyDrawable, "boundingRect", RubyDrawable_boundingRect, 0);
   defMethod(cRubyDrawable, "move", RubyDrawable_move, 2);
   defMethod(cRubyDrawable, "resize", RubyDrawable_resize, 2);
}
