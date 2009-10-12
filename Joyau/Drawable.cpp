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

template<> VALUE wrap<Rect>(int argc, VALUE *argv, VALUE info)
{
   Rect *ptr = new Rect;
   if (argc >= 2)
   {
      ptr->x = FIX2INT(argv[0]);
      ptr->y = FIX2INT(argv[1]);
      if (argc >= 4)
      {
	 ptr->w = FIX2INT(argv[2]);
	 ptr->h = FIX2INT(argv[3]);
      }
   }

   VALUE tdata = Data_Wrap_Struct(info, 0, wrapped_free<Rect>, ptr);
   return tdata;
}

template<> VALUE wrap<Point>(int argc, VALUE *argv, VALUE info)
{
   Point *ptr = new Point;
   if (argc >= 2)
   {
      ptr->x = FIX2INT(argv[0]);
      ptr->y = FIX2INT(argv[1]);
   }

   VALUE tdata = Data_Wrap_Struct(info, 0, wrapped_free<Point>, ptr);
   return tdata;
}

Point Point::operator+(const Point &op) const
{
   return Point(x + op.x, y + op.y);
}

Point Point::operator-(const Point &op) const
{
   return Point(x - op.x, y - op.y);
}

bool Point::operator==(const Point &op) const
{
   return x == op.x && y == op.y;
}

Drawable::Drawable()
{
   _x = 0;
   _y = 0;
   _w = 0;
   _h = 0;

   setClass("Drawable");
}

Rect Drawable::boundingRect() const
{ 
   return Rect(getX(), getY(), getW(), getH()); 
}

bool Drawable::collide(Drawable &item)
{
   Rect rect1 = item.boundingRect();
   Rect rect2 = boundingRect();

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
      /*
	If only on corner of this is on item, or if only one of the
	item corner is on this, we return true because it collides.
	Otherwisely, we return false since it doesn't.
      */

      if (isOn(points1[i]))
	 return true;
      if (item.isOn(points2[i]))
	 return true;
   }
   return false;
}

bool Drawable::isOn(int x, int y)
{
   Rect rect = boundingRect();
   int x1 = rect.x;
   int x2 = rect.x + rect.w;
   int y1 = rect.y;
   int y2 = rect.y + rect.h;

   if (x >= x1 &&
       x <= x2 &&
       y >= y1 &&
       y <= y2)
      return true;
   return false;
}

bool Drawable::isOn(const Point &p)
{
   return isOn(p.x, p.y);
}

int Drawable::getX() const
{
   return _x;
}

int Drawable::getY() const
{
   return _y;
}

void Drawable::setX(int x)
{
   move(x - _x, 0);
}

void Drawable::setY(int y)
{
   move(0, y - _y);
}

int Drawable::getW() const
{
   return _w;
}

int Drawable::getH() const
{
   return _h;
}

void Drawable::setPos(int x, int y)
{
   move(x - _x, y - _y);
}

void Drawable::setPos(const Point &p)
{
   setPos(p.x, p.y);
}

void Drawable::move(int x, int y)
{
   _x += x;
   _y += y;
   movedX += x;
   movedY += y;
}

void Drawable::clearMove()
{
   movedX = 0;
   movedY = 0;
}

void Drawable::cancelMove()
{
   move(-movedX, -movedY);
}

VALUE Point_getX(VALUE self)
{
   Point &ref = getRef<Point>(self);
   return INT2FIX(ref.x);
}

VALUE Point_getY(VALUE self)
{
   Point &ref = getRef<Point>(self);
   return INT2FIX(ref.y);
}

VALUE Point_setX(VALUE self, VALUE val)
{
   Point &ref = getRef<Point>(self);
   int _val = FIX2INT(val);
   
   ref.x = _val;
   return val;
}

VALUE Point_setY(VALUE self, VALUE val)
{
   Point &ref = getRef<Point>(self);
   int _val = FIX2INT(val);
   
   ref.y = _val;
   return val;
}

VALUE Drawable_movedX(VALUE self)
{
   Drawable &ref = getRef<Drawable>(self);
   return INT2FIX(ref.getMovedX());
}

VALUE Drawable_movedY(VALUE self)
{
   Drawable &ref = getRef<Drawable>(self);
   return INT2FIX(ref.getMovedY());
}

VALUE Point_add(VALUE self, VALUE op)
{
   Point &first = getRef<Point>(self);
   Point &second = getRef<Point>(op);
   
   Point ret = first + second;
   return createObject(getClass("Point"), ret);
}

VALUE Point_eq(VALUE self, VALUE op)
{
   Point &first = getRef<Point>(self);
   Point &second = getRef<Point>(op);

   return first == second ? Qtrue : Qfalse;
}

VALUE Point_sub(VALUE self, VALUE op)
{
   Point &first = getRef<Point>(self);
   Point &second = getRef<Point>(op);
   
   Point ret = first - second;
   return createObject(getClass("Point"), ret);
}

VALUE Rect_getX(VALUE self)
{
   Rect &ref = getRef<Rect>(self);
   return INT2FIX(ref.x);
}

VALUE Rect_getY(VALUE self)
{
   Rect &ref = getRef<Rect>(self);
   return INT2FIX(ref.y);
}

VALUE Rect_getW(VALUE self)
{
   Rect &ref = getRef<Rect>(self);
   return INT2FIX(ref.w);
}

VALUE Rect_getH(VALUE self)
{
   Rect &ref = getRef<Rect>(self);
   return INT2FIX(ref.h);
}

VALUE Rect_setX(VALUE self, VALUE val)
{
   Rect &ref = getRef<Rect>(self);
   int _val = FIX2INT(val);

   ref.x = _val;
   return val;
}


VALUE Rect_setY(VALUE self, VALUE val)
{
   Rect &ref = getRef<Rect>(self);
   int _val = FIX2INT(val);

   ref.y = _val;
   return val;
}

VALUE Rect_setW(VALUE self, VALUE val)
{
   Rect &ref = getRef<Rect>(self);
   int _val = FIX2INT(val);

   ref.w = _val;
   return val;
}

VALUE Rect_setH(VALUE self, VALUE val)
{
   Rect &ref = getRef<Rect>(self);
   int _val = FIX2INT(val);

   ref.h = _val;
   return val;
}

VALUE Drawable_boundingRect(VALUE self)
{
   Drawable &ref = getRef<Drawable>(self);
   Rect rect = ref.boundingRect();
  
   return createObject(getClass("Rect"), rect);
}

VALUE Drawable_collide(VALUE self, VALUE item)
{
   Drawable &ref = getRef<Drawable>(self);
   RubyDrawable val(item);
   if (ref.collide(val))
      return Qtrue;
   return Qfalse;
}

VALUE Drawable_isOn(VALUE self, VALUE x, VALUE y)
{
   Drawable &ref = getRef<Drawable>(self);
   int _x = FIX2INT(x);
   int _y = FIX2INT(y);

   if (ref.isOn(_x, _y))
      return Qtrue;
   return Qfalse;
}

VALUE Drawable_getX(VALUE self)
{
   Drawable &ref = getRef<Drawable>(self);
   return INT2FIX(ref.getX());
}

VALUE Drawable_getY(VALUE self)
{
   Drawable &ref = getRef<Drawable>(self);
   return INT2FIX(ref.getY());
}

VALUE Drawable_setX(VALUE self, VALUE x)
{
   Drawable &ref = getRef<Drawable>(self);
   ref.setX(INT2FIX(x));

   return x;
}

VALUE Drawable_setY(VALUE self, VALUE y)
{
   Drawable &ref = getRef<Drawable>(self);
   ref.setY(INT2FIX(y));

   return y;
}

VALUE Drawable_getW(VALUE self)
{
   Drawable &ref = getRef<Drawable>(self);
   return INT2FIX(ref.getW());
}

VALUE Drawable_getH(VALUE self)
{
   Drawable &ref = getRef<Drawable>(self);
   return INT2FIX(ref.getH());
}

VALUE Drawable_setPos(VALUE self, VALUE x, VALUE y)
{
   Drawable &ref = getRef<Drawable>(self);
   int _x = FIX2INT(x);
   int _y = FIX2INT(y);

   ref.setPos(_x, _y);
   return Qnil;
}

VALUE Drawable_setPoint(VALUE self, VALUE p)
{
   Drawable &ref = getRef<Drawable>(self);
   Point &pRef = getRef<Point>(p);
   ref.setPos(pRef);

   return p;
}

VALUE Drawable_move(VALUE self, VALUE x, VALUE y)
{
   Drawable &ref = getRef<Drawable>(self);
   int _x = FIX2INT(x);
   int _y = FIX2INT(y);

   ref.move(_x, _y);
   return Qnil;
}

VALUE Drawable_cancelMove(VALUE self)
{
   Drawable &ref = getRef<Drawable>(self);
   ref.cancelMove();

   return Qnil;
}

VALUE Drawable_clearMove(VALUE self)
{
   Drawable &ref = getRef<Drawable>(self);
   ref.clearMove();

   return Qnil;
}

VALUE Drawable_draw(VALUE self)
{
   Drawable &ref = getRef<Drawable>(self);
   ref.clearMove(); // So, we don't have to change each draw methods
   ref.draw();

   return Qnil;
}

void defineDrawable()
{
   VALUE cPoint = defClass<Point>("Point");
   defMethod(cPoint, "x", Point_getX, 0);
   defMethod(cPoint, "y", Point_getY, 0);

   defMethod(cPoint, "x=", Point_setX, 1);
   defMethod(cPoint, "y=", Point_setY, 1);

   defMethod(cPoint, "+", Point_add, 1);
   defMethod(cPoint, "-", Point_sub, 1);
   defMethod(cPoint, "==", Point_eq, 1);

   VALUE cRect = defClass<Rect>("Rect");
   defMethod(cRect, "x", Rect_getX, 0);
   defMethod(cRect, "y", Rect_getY, 0);
   defMethod(cRect, "w", Rect_getW, 0);
   defMethod(cRect, "h", Rect_getH, 0);
   
   defMethod(cRect, "x=", Rect_setX, 1);
   defMethod(cRect, "y=", Rect_setY, 1);
   defMethod(cRect, "w=", Rect_setW, 1);
   defMethod(cRect, "h=", Rect_setH, 1);

   VALUE cDrawable = defClass<Drawable>("Drawable");
   defMethod(cDrawable, "boundingRect", Drawable_boundingRect, 0);
   defMethod(cDrawable, "collide", Drawable_collide, 1);
   defMethod(cDrawable, "isOn", Drawable_isOn, 2);

   defMethod(cDrawable, "getX", Drawable_getX, 0);
   defMethod(cDrawable, "getY", Drawable_getY, 0);

   defMethod(cDrawable, "x=", Drawable_setX, 1);
   defMethod(cDrawable, "y=", Drawable_setY, 1);

   defMethod(cDrawable, "movedX", Drawable_movedX, 0);
   defMethod(cDrawable, "movedY", Drawable_movedY, 0);

   defMethod(cDrawable, "getW", Drawable_getW, 0);
   defMethod(cDrawable, "getH", Drawable_getH, 0);

   defMethod(cDrawable, "setPos", Drawable_setPos, 2);
   defMethod(cDrawable, "pos=", Drawable_setPoint, 1);

   defMethod(cDrawable, "move", Drawable_move, 2);
   defMethod(cDrawable, "cancelMove", Drawable_cancelMove, 0);
   defMethod(cDrawable, "clearMove", Drawable_clearMove, 0);

   defMethod(cDrawable, "draw", Drawable_draw, 0);

   defAlias(cDrawable, "getX", "x");
   defAlias(cDrawable, "getY", "y");
   defAlias(cDrawable, "getW", "w");
   defAlias(cDrawable, "getH", "h");
}
