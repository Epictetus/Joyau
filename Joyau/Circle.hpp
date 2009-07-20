#ifndef JOYAU_CIRCLE
#define JOYAU_CIRCLE

#include "Drawable.hpp"

class Circle: public Drawable
{
public:
   Circle(): filled(true) {}
   void toggleFilled() { filled = !filled; }

   void setCenter(int x, int y);
   void setRadius(int r);

   void draw();

   void setColor(OSL_COLOR col);

   int getCenterX() { return centerX; }
   int getCenterY() { return centerY; }
   int getRadius() { return _r; }
private:
   int centerX, centerY;
   int _r;

   OSL_COLOR _col;
   bool filled;
};

VALUE Circle_toggleFilled(VALUE self);

VALUE Circle_setCenter(VALUE self, VALUE x, VALUE y);
VALUE Circle_setRadius(VALUE self, VALUE r);

VALUE Circle_setColor(VALUE self, VALUE col);

VALUE Circle_getCenterX(VALUE self);
VALUE Circle_getCenterY(VALUE self);
VALUE Circle_getRadius(VALUE self);

void defineCircle();

#endif
