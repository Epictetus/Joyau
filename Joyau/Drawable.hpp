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

/** @defgroup Drawables
 *  Contains classes representing drawable objects, or related to them.
 */

/** @addtogroup Drawables **/
/*@{*/

/**
 * @class Point
 * A class which represents a point, with two integers.
 */
struct Point
{
   Point() {}

   /** Creates a new Point.
    *  @param X x position
    *  @param Y y position
    */
   Point(int X, int Y):
      x(X), y(Y)
   {}

   /** Returns a new point, whose x and y positions are our position and 
    *  op's position sum.
    */
   Point operator+(const Point &op) const;

   /** Returns a new point, whose x and y positions are our position and 
    *  op's position substraction.
    */
   Point operator-(const Point &op) const;

   /** Returns whether our position and op's position are equals. **/
   bool operator==(const Point &op) const;

   /** x position **/
   int x; 

   /** y position **/
   int y;
};

/**
 * @class Rect
 * A class which represents a rect.
 */
struct Rect
{
   Rect() {}

   /** Creates a new Rect.
    *  @param X x position
    *  @param Y y position
    *  @param W rect's width
    *  @param H rect's height.
    */
   Rect(int X, int Y, int W, int H):
      x(X), y(Y), w(W), h(H)
   {}

   /** x position **/
   int x;

   /** y position **/
   int y;

   /** rect's width **/
   int w;
   
   /** rect's height **/
   int h;

   /** returns our top right corner **/
   Point top_right() const { return Point(x + w, y); }

   /** returns our top left corner **/
   Point top_left() const { return Point(x, y); }

   /** returns our bottom right corner **/
   Point bottom_right() const { return Point(x + w, y + h); }

   /** returns our bottom left corner **/
   Point bottom_left() const { return Point(x, y + h); }
};

/**
 * @class Drawable
 * A class which represents a Drawable. It contains
 * some functions for positionnement, collision checking, ...
 */
class Drawable: public RubyObject
{
public:
   Drawable();

   /** Draws the object **/
   virtual void draw() {}

   /** Returns the rect the drawable stands in. **/
   virtual Rect boundingRect() const;

   /** Returns whether we're in collision with item. **/
   virtual bool collide(Drawable &item);

   /** Returns whether the point (x, y) is on this drawable. **/
   virtual bool isOn(int x, int y);

   /** Returns whether the point p is on this drawable.
    *  Provided for convenience.
    */
   virtual bool isOn(const Point &p);

   /** Returns the x position */
   virtual int getX() const;

   /** Returns the y position */
   virtual int getY() const;

   /** Changes the x position
    *  @param x new x position
    */
   virtual void setX(int x);

   /** Changes the y position
    *  @param y new y position
    */
   virtual void setY(int y);

   /** returns the drawable's width **/
   virtual int getW() const;

   /** returns the drawable's height **/
   virtual int getH() const;

   /** Changes the drawable's position
    *  @param x new x position
    *  @param y new y position
    */
   virtual void setPos(int x, int y);

   /** Changes the drawable's position, with a Point. Provided for convenience.
    *  @param p new position
    */
   void setPos(const Point &p);

   /** Returns the pixel number we've moved horizontally
    * since the last clear. 
    */
   int getMovedX() const { return movedX; }

   /** Returns the pixel number we've moved vertically
    * since the last clear. 
    */
   int getMovedY() const { return movedY; }

   /** moves the drawable of (x, y) pixels **/
   virtual void move(int x, int y);

   /** cancels all the moves since the last clear **/
   void cancelMove();

   /** clears all the moves since the last time we've called this function.  **/
   virtual void clearMove();
protected:
   int _w, _h;
   int _x, _y;
   int movedX, movedY;
};

/*@}*/

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

VALUE Drawable_movedX(VALUE self);
VALUE Drawable_movedY(VALUE self);

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
