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

#ifndef JOYAU_TEXT
#define JOYAU_TEXT

#include "StdInclude.hpp"
#include "Shape.hpp"
#include "Manager.hpp"

class DrawableText: public Shape
{
public:
   DrawableText();
   void setText(string text) { _text = text; }

   void toggleStirring();
   void toggleScripted();
   
   void setBackground(OSL_COLOR col) { bg = col; }
   void setFont(string filename) { font = filename; }

   void draw();

   int getW();
   int getH() { return 10; }

   bool isScripted() const { return scripted; }
   bool isStirring() const { return stirring; }

   OSL_COLOR getBackground() const { return bg; }
   string getFont() const { return font; }

   string getText() const { return _text; }
private:
   string font;
   string _text;

   bool scripted;
   bool stirring;

   OSL_COLOR bg;
};

VALUE DrawableText_setText(VALUE self, VALUE text);

VALUE DrawableText_toggleStirring(VALUE self);
VALUE DrawableText_toggleScripted(VALUE self);

VALUE DrawableText_setBackground(VALUE self, VALUE color);
VALUE DrawableText_setFont(VALUE self, VALUE font);

VALUE DrawableText_scripted(VALUE self);
VALUE DrawableText_stirring(VALUE self);

VALUE DrawableText_background(VALUE self);
VALUE DrawableText_font(VALUE self);

VALUE DrawableText_text(VALUE self);

void defineDrawableText();

#endif
