#include "Line.hpp"

void Line::setPoint(int x, int y)
{
   _x2 = x;
   _y2 = y;

   _w = _x2 - getX();
   _h = _y2 - getY();
}

void Line::setColor(OSL_COLOR col)
{
   for (int i = 0; i < 2; ++i)
      _col[i] = col;
}

void Line::setGradient(OSL_COLOR col[2])
{
   for (int i = 0; i < 2; ++i)
      _col[i] = col[i];
}

void Line::draw()
{
   oslDrawGradientLine (getX(), getY(), _x2, _y2, 
			_col[0], _col[1]);
}

VALUE Line_setPoint(VALUE self, VALUE x, VALUE y)
{
   Line &ref = getRef<Line>(self);
   int _x = FIX2INT(x);
   int _y = FIX2INT(y);

   ref.setPoint(_x, _y);
   return Qnil;
}

VALUE Line_setColor(VALUE self, VALUE col)
{
   Line &ref = getRef<Line>(self);
   OSL_COLOR _col = hash2col(col);

   ref.setColor(_col);
   return Qnil;
}

VALUE Line_setGradient(VALUE self, VALUE col)
{
   Line &ref = getRef<Line>(self);
   OSL_COLOR _col[2];

   for (int i = 0; i < 2; ++i)
      _col[i] = hash2col(rb_ary_entry(col, INT2FIX(i)));

   ref.setGradient(_col);
   return Qnil;
}

void defineLine()
{
   VALUE cDrawable = getClass("Drawable");
   VALUE cLine = defClass<Line>("Line", cDrawable);
   defMethod(cLine, "setColor", Line_setColor, 1);
   defMethod(cLine, "setGradient", Line_setGradient, 1);
   defMethod(cLine, "setPoint", Line_setPoint, 2);
}
