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

#include "RubyDrawable.hpp"

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

int RubyDrawable::getX() const
{
   VALUE ret = rb_funcall(self, getFunc("getX"), 0);
   return FIX2INT(ret);
}

int RubyDrawable::getY() const
{
   VALUE ret = rb_funcall(self, getFunc("getY"), 0);
   return FIX2INT(ret);
}

void RubyDrawable::setX(int val)
{
   rb_funcall(self, getFunc("setX"), 1, INT2FIX(val));
}

void RubyDrawable::setY(int val)
{
   rb_funcall(self, getFunc("setY"), 1, INT2FIX(val));
}

int RubyDrawable::getW() const
{
   VALUE ret = rb_funcall(self, getFunc("getW"), 0);
   return FIX2INT(ret);
}

int RubyDrawable::getH() const
{
   VALUE ret = rb_funcall(self, getFunc("getH"), 0);
   return FIX2INT(ret);
}

void RubyDrawable::setPos(int x, int y)
{
   VALUE xArg = INT2FIX(x);
   VALUE yArg = INT2FIX(y);
   rb_funcall(self, getFunc("setPos"), 2, xArg, yArg);
}

void RubyDrawable::clearMove()
{
   rb_funcall(self, getFunc("clearMove"), 0);
}
