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

#ifndef JOYAU_DRAWABLE
#define JOYAU_DRAWABLE

#include "StdInclude.hpp"

struct Point
{
   Point() {}

   Point(int X, int Y):
      x(X), y(Y)
   {}

   Point operator+(const Point &op) const;
   Point operator-(const Point &op) const;

   bool operator==(const Point &op) const;

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

   Point top_right() const { return Point(x + w, y); }
   Point top_left() const { return Point(x, y); }
   Point bottom_right() const { return Point(x + w, y + h); }
   Point bottom_left() const { return Point(x, y + h); }
};

class Drawable
{
public:
   Drawable();

   virtual void draw() {}

   virtual Rect boundingRect() const;

   virtual bool collide(Drawable &item);
   virtual bool isOn(int x, int y);
   virtual bool isOn(const Point &p);

   virtual int getX() const;
   virtual int getY() const;

   virtual void setX(int x);
   virtual void setY(int y);

   virtual int getW() const;
   virtual int getH() const;

   virtual void setPos(int x, int y);
   void setPos(const Point &p);

   virtual void move(int x, int y);
   void cancelMove();
   virtual void clearMove(); // maybe the user would like to clear this.
protected:
   int _w, _h;
   int _x, _y;
   int movedX, movedY;
};

VALUE Point_getX(VALUE self);
VALUE Point_getY(VALUE self);

VALUE Point_setX(VALUE self, VALUE val);
VALUE Point_setY(VALUE self, VALUE val);

VALUE Point_add(VALUE self, VALUE op);
VALUE Point_sub(VALUE self, VALUE op);
VALUE Point_eq(VALUE self, VALUE op);

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

VALUE Drawable_setX(VALUE self, VALUE x);
VALUE Drawable_setY(VALUE self, VALUE y);

VALUE Drawable_getW(VALUE self);
VALUE Drawable_getH(VALUE self);

VALUE Drawable_setPos(VALUE self, VALUE x, VALUE y);
VALUE Drawable_setPoint(VALUE self, VALUE p);

VALUE Drawable_move(VALUE self, VALUE x, VALUE y);
VALUE Drawable_cancelMove(VALUE self);
VALUE Drawable_clearMove(VALUE self);

VALUE Drawable_draw(VALUE self);

void defineDrawable();

#endif
