#ifndef JOYAU_RECT
#define JOYAU_RECT

#include "Drawable.hpp"

class DrawableRect: public Drawable
{
public:
   DrawableRect(): filled(true) {}

   void toggleFilled() { filled = !filled; }
   void setColor(OSL_COLOR col);
   void setGradient(OSL_COLOR col[4]);
   
   void resize(int w, int h);

   void draw();

   void setCorner(int x, int y);
private:
   bool filled;
   int _x2, _y2;
   OSL_COLOR _col[4];
};

VALUE DrawableRect_toggleFilled(VALUE self);

VALUE DrawableRect_setColor(VALUE self, VALUE col);
VALUE DrawableRect_setGradient(VALUE self, VALUE col);

VALUE DrawableRect_resize(VALUE self, VALUE w, VALUE h);

VALUE DrawableRect_setCorner(VALUE self, VALUE x, VALUE y);

void defineDrawableRect();

#endif
