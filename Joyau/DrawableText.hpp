#ifndef JOYAU_TEXT
#define JOYAU_TEXT

#include "StdInclude.hpp"
#include "Drawable.hpp"
#include "Manager.hpp"

class DrawableText: public Drawable
{
public:
   DrawableText(): font(""), stirring(false) {}
   void setText(string text) { _text = text; }

   void toggleStirring() { stirring = !stirring; }
   
   void setColor(OSL_COLOR col) { _col = col; }
   void setBackground(OSL_COLOR col) { bg = col; }

   void setFont(string filename) { font = filename; }

   void draw();

   int getW();
   int getH() { return 10; }
private:
   string font;
   string _text;
   bool stirring;

   OSL_COLOR bg, _col;
};

VALUE DrawableText_setText(VALUE self, VALUE text);

VALUE DrawableText_toggleStirring(VALUE self);

VALUE DrawableText_setColor(VALUE self, VALUE color);
VALUE DrawableText_setBackground(VALUE self, VALUE color);

VALUE DrawableText_setFont(VALUE self, VALUE font);

void defineDrawableText();

#endif
