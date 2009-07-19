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

#ifndef __JOYAU_DRAWABLE__
#define __JOYAU_DRAWABLE__

#include "StdInclude.hpp"

struct Point
{
   Point() {}

   Point(int X, int Y):
      x(X), y(Y)
   {}

   int x, y;
};

struct Rect
{
   Rect() {}

   Rect(int X, int Y, int W, int H):
      x(X), y(Y), w(W), h(H)
   {}

   int x, y;
   int w, h;

   Point top_right() { return Point(x + w, y); }
   Point top_left() { return Point(x, y); }
   Point bottom_right() { return Point(x + w, y + h); }
   Point bottom_left() { return Point(x, y + h); }
};

class Drawable
{
public:
   Drawable();

   virtual void draw() {}

   virtual Rect boundingRect();

   virtual bool collide(Drawable &item);
   virtual bool isOn(int x, int y);
   virtual bool isOn(Point p);

   int getX();
   int getY();

   virtual int getW();
   virtual int getH();

   void setPos(int x, int y);
   void setPos(Point p);

   void move(int x, int y);
private:
   int _x, _y;
protected:
   int _w, _h;
};

VALUE Point_getX(VALUE self);
VALUE Point_getY(VALUE self);

VALUE Point_setX(VALUE self, VALUE val);
VALUE Point_setY(VALUE self, VALUE val);

VALUE Rect_getX(VALUE self);
VALUE Rect_getY(VALUE self);
VALUE Rect_getW(VALUE self);
VALUE Rect_getH(VALUE self);

VALUE Rect_setX(VALUE self, VALUE val);
VALUE Rect_setY(VALUE self, VALUE val);
VALUE Rect_setW(VALUE self, VALUE val);
VALUE Rect_setH(VALUE self, VALUE val);

VALUE Drawable_boundingRect(VALUE self);

VALUE Drawable_collide(VALUE self, VALUE item);
VALUE Drawable_isOn(VALUE self, VALUE x, VALUE y);

VALUE Drawable_getX(VALUE self);
VALUE Drawable_getY(VALUE self);

VALUE Drawable_getW(VALUE self);
VALUE Drawable_getH(VALUE self);

VALUE Drawable_setPos(VALUE self, VALUE x, VALUE y);

VALUE Drawable_move(VALUE self, VALUE x, VALUE y);

VALUE Drawable_draw(VALUE self);

void defineDrawable();

#endif
