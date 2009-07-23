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
#include "Drawable.hpp"
#include "Manager.hpp"

class DrawableText: public Drawable
{
public:
   DrawableText();
   void setText(string text) { _text = text; }

   void toggleStirring();
   void toggleScripted();
   
   void setColor(OSL_COLOR col) { _col = col; }
   void setBackground(OSL_COLOR col) { bg = col; }

   void setFont(string filename) { font = filename; }

   void draw();

   int getW();
   int getH() { return 10; }
private:
   string font;
   string _text;

   bool scripted;
   bool stirring;

   OSL_COLOR bg, _col;
};

VALUE DrawableText_setText(VALUE self, VALUE text);

VALUE DrawableText_toggleStirring(VALUE self);
VALUE DrawableText_toggleScripted(VALUE self);

VALUE DrawableText_setColor(VALUE self, VALUE color);
VALUE DrawableText_setBackground(VALUE self, VALUE color);

VALUE DrawableText_setFont(VALUE self, VALUE font);

void defineDrawableText();

#endif
