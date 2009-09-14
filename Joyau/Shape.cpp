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

#include "Shape.hpp"

Shape::Shape(int size)
{
   _size = size;
   _col = new OSL_COLOR[size];

   setClass("Shape");
}

Shape::Shape(const Shape &obj)
{
   _col = new OSL_COLOR[obj._size];
   _size = obj._size;
   for (int i = 0; i < _size; ++i)
      _col[i] = obj._col[i];

   setClass("Shape");
}

Shape::~Shape()
{
   delete[] _col;
}

void Shape::setColor(OSL_COLOR col)
{
   for (int i = 0; i < _size; ++i)
      _col[i] = col;
}

void Shape::setGradient(OSL_COLOR *col)
{
   for (int  i = 0; i < _size; ++i)
      _col[i] = col[i];
}

VALUE Shape_setColor(int argc, VALUE *argv, VALUE self)
{
   Shape &ref = getRef<Shape>(self);
   OSL_COLOR _col;
   if (argc >= 3)
   {
      int alpha = 255;
      if (argc > 3)
	 alpha = FIX2INT(argv[3]);
      _col = RGBA(FIX2INT(argv[0]), FIX2INT(argv[1]), FIX2INT(argv[2]), alpha);
   }
   else if (argc == 1)
      _col = hash2col(argv[0]);
   else
      return Qfalse; // Not enough arguments.

   ref.setColor(_col);
   return Qnil;
}

VALUE Shape_setGradient(VALUE self, VALUE col)
{
   Shape &ref = getRef<Shape>(self);
   int size = ref.getColorsNumber();

   OSL_COLOR *args = new OSL_COLOR[size];
   for(int i = 0; i < size; ++i)
      args[i] = hash2col(rb_ary_entry(col, i));

   ref.setGradient(args);
   return Qnil;
}

VALUE Shape_getColorsNumber(VALUE self)
{
   Shape &ref = getRef<Shape>(self);
   return INT2FIX(ref.getColorsNumber());
}

VALUE Shape_getColors(VALUE self)
{
   Shape &ref = getRef<Shape>(self);
   int size = ref.getColorsNumber();

   VALUE ret = rb_ary_new();
   OSL_COLOR *col = ref.getColors();

   for (int i = 0; i < size; ++i)
      rb_ary_push(ret, col2hash(col[i]));
   return ret;
}

VALUE Shape_getColor(VALUE self)
{
   Shape &ref = getRef<Shape>(self);
   return col2hash(ref.getColor());
}

VALUE FillableShape_toggleFilled(VALUE self)
{
   FillableShape &ref = getRef<FillableShape>(self);
   
   ref.toggleFilled();
   return Qnil;
}

VALUE FillableShape_setFilled(VALUE self, VALUE val)
{
   FillableShape &ref = getRef<FillableShape>(self);
   
   ref.setFilled(val == Qtrue);
   return Qnil;
}

VALUE FillablleShape_filled(VALUE self)
{
   FillableShape &ref = getRef<FillableShape>(self);
   return ref.isFilled() ? Qtrue : Qfalse;
}

void defineShape()
{
   VALUE cShape = defClass<Shape>("Shape", "Drawable");
   defMethod(cShape, "setColor", Shape_setColor, -1);
   defMethod(cShape, "setGradient", Shape_setGradient, 1);
   defMethod(cShape, "getColors", Shape_getColors, 0);
   defMethod(cShape, "getColor", Shape_getColor, 0);
   defMethod(cShape, "getColorsNumber", Shape_getColorsNumber, 0);
   
   VALUE cFillableShape = defClass<FillableShape>("FillableShape", "Shape");
   defMethod(cFillableShape, "toggleFilled", FillableShape_toggleFilled, 0);
   defMethod(cFillableShape, "filled", FillablleShape_filled, 0);
   defMethod(cFillableShape, "filled=", FillableShape_setFilled, 1);

   defAlias(cShape, "setColor", "color=");
   defAlias(cShape, "setGradient", "gradient=");
   defAlias(cShape, "getColors", "colors");
   defAlias(cShape, "getColor", "color");
   defAlias(cShape, "getColorsNumber", "colorsNumber");
}
