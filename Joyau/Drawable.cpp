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
#include "Buffer.hpp"

/*
  Document-class: Joyau::Rect

  Rects are used in order to represent simple rectangle. They're not here
  to be drawn, but rather to be used in a basic collision system.

  For instance, you can get one from Joyau::Drawable#boundingRect.
*/

template<>
/*
  call-seq: Joyau::Rect.new
            Joyau::Rect.new(x, y)
            Joyau::Rect.new(x, y, w, h)

  Creates a new Rect.
*/
VALUE wrap<Rect>(int argc, VALUE *argv, VALUE info)
{
   Rect *ptr = new Rect;

   VALUE x, y, w, h;
   rb_scan_args(argc, argv, "04", &x, &y, &w, &h);

   if (!NIL_P(x) && !NIL_P(y))
   {
      ptr->x = FIX2INT(x);
      ptr->y = FIX2INT(y);
      if (!NIL_P(w) && !NIL_P(h))
      {
	 ptr->w = FIX2INT(w);
	 ptr->h = FIX2INT(h);
      }
   }

   VALUE tdata = Data_Wrap_Struct(info, 0, wrapped_free<Rect>, ptr);
   return tdata;
}


/*
  Document-class: Point

  Represents a point. Can be used to get an object's position, or to set it.
*/

template<> // I don't like this syntax, yet I didn't find anything else for rdoc.
/*
  call-seq: Joyau::Point.new
            Joyau::Point.new(x, y)

  Creates a new Point.
*/
VALUE wrap<Point>(int argc, VALUE *argv, VALUE info)
{
   Point *ptr = new Point;

   VALUE x, y;
   rb_scan_args(argc, argv, "02", &x, &y);

   if (!NIL_P(x) && !NIL_P(y))
   {
      ptr->x = FIX2INT(x);
      ptr->y = FIX2INT(y);
   }

   VALUE tdata = Data_Wrap_Struct(info, 0, wrapped_free<Point>, ptr);
   return tdata;
}

/*
  Document-class: Drawable

  Classe from which almost anything that can be drawn on the screen inherits.
  Instanciating a Drawable directly makes no sens though.
*/

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

/*
  Returns a point's abscissa.
*/
VALUE Point_getX(VALUE self)
{
   Point &ref = getRef<Point>(self);
   return INT2FIX(ref.x);
}

/*
  Returns a point's ordinate.
*/
VALUE Point_getY(VALUE self)
{
   Point &ref = getRef<Point>(self);
   return INT2FIX(ref.y);
}

/*
  call-seq: x=(x)

  Sets a point's abscissa.
*/
VALUE Point_setX(VALUE self, VALUE val)
{
   Point &ref = getRef<Point>(self);
   int _val = FIX2INT(val);
   
   ref.x = _val;
   return val;
}

/*
  call-seq: y=(y)

  Sets a point's ordinate.
*/
VALUE Point_setY(VALUE self, VALUE val)
{
   Point &ref = getRef<Point>(self);
   int _val = FIX2INT(val);
   
   ref.y = _val;
   return val;
}

/*
  Returns the difference between the actual abscissa, and the one when
  clearMove was called.
*/
VALUE Drawable_movedX(VALUE self)
{
   Drawable &ref = getRef<Drawable>(self);
   return INT2FIX(ref.getMovedX());
}

/*
  Returns the difference between the actual ordinate, and the one when
  clearMove was called.
*/
VALUE Drawable_movedY(VALUE self)
{
   Drawable &ref = getRef<Drawable>(self);
   return INT2FIX(ref.getMovedY());
}

/*
  call-seq: point1 + point2

  Creates a new point, adding their abscissas and ordinates.
    Joyau::Point.new(3, 4) + Joyau::Point.new(5, 6) # => Joyau::Point(8, 10)
*/
VALUE Point_add(VALUE self, VALUE op)
{
   if (!rb_obj_is_kind_of(op, getClass("Point")))
      rb_raise(rb_eTypeError, "Can't convert %s into Joyau::Point",
	       rb_obj_classname(op));

   Point &first = getRef<Point>(self);
   Point &second = getRef<Point>(op);
   
   Point ret = first + second;
   return createObject(getClass("Point"), ret);
}

/*
  call-seq: point1 == point2

  Returns whether two points represents the same one (i.e. they have the same
  coordinates).
*/
VALUE Point_eq(VALUE self, VALUE op)
{
   if (!rb_obj_is_kind_of(op, getClass("Point")))
      rb_raise(rb_eTypeError, "Can't convert %s into Joyau::Point",
	       rb_obj_classname(op));

   Point &first = getRef<Point>(self);
   Point &second = getRef<Point>(op);

   return first == second ? Qtrue : Qfalse;
}

/*
  call-seq: point1 - point2

  Creates a new point, substracting their abscissas and ordinates.
    Joyau::Point.new(3, 4) - Joyau::Point.new(5, 6) # => Joyau::Point(-2, -2)
*/
VALUE Point_sub(VALUE self, VALUE op)
{
   if (!rb_obj_is_kind_of(op, getClass("Point")))
      rb_raise(rb_eTypeError, "Can't convert %s into Joyau::Point",
	       rb_obj_classname(op));

   Point &first = getRef<Point>(self);
   Point &second = getRef<Point>(op);
   
   Point ret = first - second;
   return createObject(getClass("Point"), ret);
}

/*
  Returns a rect's abscissa.
*/
VALUE Rect_getX(VALUE self)
{
   Rect &ref = getRef<Rect>(self);
   return INT2FIX(ref.x);
}

/*
  Returns a rect's ordinate.
*/
VALUE Rect_getY(VALUE self)
{
   Rect &ref = getRef<Rect>(self);
   return INT2FIX(ref.y);
}

/*
  Returns a rect's width.
*/
VALUE Rect_getW(VALUE self)
{
   Rect &ref = getRef<Rect>(self);
   return INT2FIX(ref.w);
}

/*
  Returns a rect's height.
*/
VALUE Rect_getH(VALUE self)
{
   Rect &ref = getRef<Rect>(self);
   return INT2FIX(ref.h);
}

/*
  call-seq: x=(val)

  Sets a rect's abscissa.
*/
VALUE Rect_setX(VALUE self, VALUE val)
{
   Rect &ref = getRef<Rect>(self);
   int _val = FIX2INT(val);

   ref.x = _val;
   return val;
}

/*
  call-seq: y=(val)

  Sets a rect's ordinate.
*/
VALUE Rect_setY(VALUE self, VALUE val)
{
   Rect &ref = getRef<Rect>(self);
   int _val = FIX2INT(val);

   ref.y = _val;
   return val;
}

/*
  call-seq: w=(val)

  Sets a rect's width.
*/
VALUE Rect_setW(VALUE self, VALUE val)
{
   Rect &ref = getRef<Rect>(self);
   int _val = FIX2INT(val);

   ref.w = _val;
   return val;
}

/*
  call-seq: h=(val)

  Sets a rect's height.
*/
VALUE Rect_setH(VALUE self, VALUE val)
{
   Rect &ref = getRef<Rect>(self);
   int _val = FIX2INT(val);

   ref.h = _val;
   return val;
}

/*
  Returns a Drawable's bounding rect.
*/
VALUE Drawable_boundingRect(VALUE self)
{
   Drawable &ref = getRef<Drawable>(self);
   Rect rect = ref.boundingRect();
  
   return createObject(getClass("Rect"), rect);
}

/*
  call-seq: collide(item)
            collide?(item)

  Returns whether two items collide.
*/
VALUE Drawable_collide(VALUE self, VALUE item)
{
   if (!rb_obj_is_kind_of(item, getClass("Point")))
      rb_raise(rb_eTypeError, "Can't convert %s into Joyau::Drawable",
	       rb_obj_classname(item));

   Drawable &ref = getRef<Drawable>(self);
   RubyDrawable val(item);
   if (ref.collide(val))
      return Qtrue;
   return Qfalse;
}

/*
  call-seq: isOn(x, y)
            is_on?(x, y)

  Returns whether a point is on the drawable.
*/
VALUE Drawable_isOn(VALUE self, VALUE x, VALUE y)
{
   Drawable &ref = getRef<Drawable>(self);
   int _x = FIX2INT(x);
   int _y = FIX2INT(y);

   if (ref.isOn(_x, _y))
      return Qtrue;
   return Qfalse;
}

/*
  Returns a drawable's abscissa.
*/
VALUE Drawable_getX(VALUE self)
{
   Drawable &ref = getRef<Drawable>(self);
   return INT2FIX(ref.getX());
}

/*
  Returns a drawable's ordinate.
*/
VALUE Drawable_getY(VALUE self)
{
   Drawable &ref = getRef<Drawable>(self);
   return INT2FIX(ref.getY());
}

/*
  call-seq: x=(x)

  Sets the drawable's abscissa.
*/
VALUE Drawable_setX(VALUE self, VALUE x)
{
   Drawable &ref = getRef<Drawable>(self);
   ref.setX(INT2FIX(x));

   return x;
}

/*
  call-seq: y=(y)

  Sets the drawable's ordinate.
*/
VALUE Drawable_setY(VALUE self, VALUE y)
{
   Drawable &ref = getRef<Drawable>(self);
   ref.setY(INT2FIX(y));

   return y;
}

/*
  Returns a drawable's width.
*/
VALUE Drawable_getW(VALUE self)
{
   Drawable &ref = getRef<Drawable>(self);
   return INT2FIX(ref.getW());
}

/*
  Returns a drawable's height.
*/
VALUE Drawable_getH(VALUE self)
{
   Drawable &ref = getRef<Drawable>(self);
   return INT2FIX(ref.getH());
}

/*
  call-seq: setPos(x, y)

  Sets a drawable's position.
*/
VALUE Drawable_setPos(VALUE self, VALUE x, VALUE y)
{
   Drawable &ref = getRef<Drawable>(self);
   int _x = FIX2INT(x);
   int _y = FIX2INT(y);

   ref.setPos(_x, _y);
   return Qnil;
}

/*
  call-seq: pos=(p)

  Sets a drawable's position.
*/
VALUE Drawable_setPoint(VALUE self, VALUE p)
{
   if (!rb_obj_is_kind_of(p, getClass("Point")))
      rb_raise(rb_eTypeError, "Can't convert %s into Joyau::Point",
	       rb_obj_classname(p));

   Drawable &ref = getRef<Drawable>(self);
   Point &pRef = getRef<Point>(p);
   ref.setPos(pRef);

   return p;
}

/*
  call-seq: move(x, y)

  Moves a drawable.
*/
VALUE Drawable_move(VALUE self, VALUE x, VALUE y)
{
   Drawable &ref = getRef<Drawable>(self);
   int _x = FIX2INT(x);
   int _y = FIX2INT(y);

   ref.move(_x, _y);
   return Qnil;
}

/*
  Cancels all the moves done since the last call to clear move.
  May be called once a collision occur.
*/
VALUE Drawable_cancelMove(VALUE self)
{
   Drawable &ref = getRef<Drawable>(self);
   ref.cancelMove();

   return Qnil;
}

/*
  Clears all the moves done since the last call to this function. They can no
  longer be cancelled.
*/
VALUE Drawable_clearMove(VALUE self)
{
   Drawable &ref = getRef<Drawable>(self);
   ref.clearMove();

   return Qnil;
}

/*
  Draws the drawable, and clear its moves.
*/
VALUE Drawable_draw(VALUE self)
{
   Drawable &ref = getRef<Drawable>(self);
   ref.clearMove(); // So, we don't have to change the draw methods
   ref.draw();
   
   return Qnil;
}

/*
  Converts the drawable in a Buffer.
*/
VALUE Drawable_to_buf(VALUE self) {
   RubyDrawable drawable(self);
   try {
      return Data_Wrap_Struct(getClass("Buffer"), 0, wrapped_free<Buffer>,
			      new Buffer(drawable));
   }
   catch (const RubyException &e) {
      e.rbRaise();
      return Qnil;
   }
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

   defMethod(cDrawable, "to_buf", Drawable_to_buf, 0);
   
   defAlias(cDrawable, "getX", "x");
   defAlias(cDrawable, "getY", "y");
   defAlias(cDrawable, "getW", "w");
   defAlias(cDrawable, "getH", "h");

   defAlias(cDrawable, "collide", "collide?");
   defAlias(cDrawable, "isOn", "is_on?");
   defAlias(cDrawable, "boundingRect", "bounding_rect");
   defAlias(cDrawable, "movedX", "moved_x");
   defAlias(cDrawable, "movedY", "moved_y");
   defAlias(cDrawable, "cancelMove", "cancel_move");
   defAlias(cDrawable, "clearMove", "clear_move");
}
