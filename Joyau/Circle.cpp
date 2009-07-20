#include "Circle.hpp"

void Circle::setCenter(int x, int y)
{
   centerX = x;
   centerY = y;

   setPos(centerX - _r, centerY - _r);
}

void Circle::setRadius(int r)
{
   _r = r;
   // We'll call setPos here to, so setCenter and setRadius can be called
   // in any order.
   setPos(centerX - _r, centerY - _r);
   
   _w = _r * 2;
   _h = _r * 2;
}

void Circle::draw()
{
   if (filled)
      oslDrawFillCircle(centerX, centerY, _r, _col);
   else
      oslDrawCircle(centerX, centerY, _r, _col);
}

void Circle::setColor(OSL_COLOR col)
{
   _col = col;
}

VALUE Circle_toggleFilled(VALUE self)
{
   Circle &ref = getRef<Circle>(self);

   ref.toggleFilled();
   return Qnil;
}

VALUE Circle_setCenter(VALUE self, VALUE x, VALUE y)
{
   Circle &ref = getRef<Circle>(self);
   int _x = FIX2INT(x);
   int _y = FIX2INT(y);

   ref.setCenter(_x, _y);
   return Qnil;
}

VALUE Circle_setRadius(VALUE self, VALUE r)
{
   Circle &ref = getRef<Circle>(self);
   int _r = FIX2INT(r);

   ref.setRadius(_r);
   return Qnil;
}

VALUE Circle_setColor(VALUE self, VALUE col)
{
   Circle &ref = getRef<Circle>(self);
   OSL_COLOR _col = hash2col(col);

   ref.setColor(_col);
   return Qnil;
}

VALUE Circle_getCenterX(VALUE self)
{
   Circle &ref = getRef<Circle>(self);
   return INT2FIX(ref.getCenterX());
}

VALUE Circle_getCenterY(VALUE self)
{
   Circle &ref = getRef<Circle>(self);
   return INT2FIX(ref.getCenterX());
}

VALUE Circle_getRadius(VALUE self)
{
   Circle &ref = getRef<Circle>(self);
   return INT2FIX(ref.getRadius());
}

void defineCircle()
{
   VALUE cDrawable = getClass("Drawable");
   VALUE cCircle = defClass<Circle>("Circle", cDrawable);
   defMethod(cCircle, "setColor", Circle_setColor, 1);
   defMethod(cCircle, "setRadius", Circle_setRadius, 1);
   defMethod(cCircle, "setCenter", Circle_setCenter, 2);
   defMethod(cCircle, "toggleFilled", Circle_toggleFilled, 0);
   defMethod(cCircle, "getCenterX", Circle_getCenterX, 0);
   defMethod(cCircle, "getCenterY", Circle_getCenterY, 0);
   defMethod(cCircle, "getRadius", Circle_getRadius, 0);
}
