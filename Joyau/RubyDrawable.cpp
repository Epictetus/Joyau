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

bool RubyDrawable::collide(Drawable &item)
{
   VALUE drawable = createObject(getClass("Drawable"), item);
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

VALUE RubyDrawable_resize(VALUE self, VALUE w, VALUE h)
{
   RubyDrawable &ref = getRef<RubyDrawable>(self);

   ref.resize(FIX2INT(w), FIX2INT(h));
   return Qnil;
}

void defineRubyDrawable()
{
   VALUE cRubyDrawable = defClass<RubyDrawable>("RubyDrawable", "Drawable");

   defMethod(cRubyDrawable, "resize", RubyDrawable_resize, 2);
}
