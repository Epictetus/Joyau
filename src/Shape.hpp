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

#ifndef JOYAU_SHAPE
#define JOYAU_SHAPE

#include "Drawable.hpp"

/** @addtogroup Drawables **/
/*@{*/

/** 
 * @class Shape
 * A Drawable which can be colorized
 */
class Shape: public Drawable
{
public:
   /** Creates a new Shape.
    *  @param size tells how many colors there are in the shape.
    */
   Shape(int size = 1);
   Shape(const Shape &obj);
   virtual ~Shape();

   /** Changes all the color.
    *  @param col new color.
    */
   void setColor(OSL_COLOR col);

   /** Changes the color with an array.
    *  @param col an array, in which we can found getColorsNumber() colors.
    */
   void setGradient(OSL_COLOR *col);

   /** Returns an array containing all the color **/
   OSL_COLOR *getColors() const { return _col; }

   /** Returns the first color. **/
   OSL_COLOR getColor() const { return _col[0]; }

   /** Tels how many colors there are in this **/
   int getColorsNumber() const { return _size; }
protected:
   OSL_COLOR *_col;
   int _size;
};

/** 
 * @class FillableShape
 * A Shape which can be (un)filled.
 */
class FillableShape: public Shape
{
public:
   FillableShape(int size = 1):
      Shape(size), filled(true)
   { setClass("FillableShape"); }

   /** Reverses the filled value. **/
   void toggleFilled() { filled = !filled; }

   /** Changes the filled value. **/
   void setFilled(bool val) { filled = val; }

   /** Returns whether the shape is filled. **/
   bool isFilled() const { return filled; }
protected:
   bool filled;
};

/*@}*/

VALUE Shape_setColor(int argc, VALUE *argv, VALUE self);
VALUE Shape_setGradient(VALUE self, VALUE col);

VALUE Shape_getColorsNumber(VALUE self);
VALUE Shape_getColors(VALUE self);
VALUE Shape_getColor(VALUE self);

VALUE FillableShape_toggleFilled(VALUE self);
VALUE FillableShape_setFilled(VALUE self, VALUE val);
VALUE FillablleShape_filled(VALUE self);

void defineShape();

#endif
