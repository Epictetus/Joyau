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

#ifndef JOYAU_TEXT
#define JOYAU_TEXT

#include "Shape.hpp"

/** @addtogroup Drawables **/
/*@{*/

/**
 * @class DrawableText
 * A class which represents a text, which can be drawed.
 */
class DrawableText: public Shape
{
public:
   DrawableText();
   
   /** Changes the text.
    *  @param text the new text.
    */
   void setText(const std::string &text) { _text = text;  }

   /** Reverses the stirring state. **/
   void toggleStirring();

   /** Changes the stirring state.
    *  @param val new state
    */
   void setStirring(bool val) { stirring = val; }

   /** Reverses the scripted state. **/
   void toggleScripted();
   
   /** Changes the background color.
    *  @param col background color.
    */
   void setBackground(OSL_COLOR col) { bg = col; }

   /** Changes the background color.
    *  @param col background color.
    */
   void setFont(const std::string &filename) { font = filename; }

   void draw();

   int getW() const;
   int getH() const { return 10; }

   /** returns whether the text is scripted **/
   bool isScripted() const { return scripted; }
   
   /** returns whether the text is stirring **/
   bool isStirring() const { return stirring; }

   /** returns the background  **/
   OSL_COLOR getBackground() const { return bg; }

   /** returns the used font's name  **/
   std::string getFont() const { return font; }

   /** returns the drawed text **/
   std::string getText() const { return _text; }
private:
   std::string font;
   std::string _text;

   bool scripted;
   bool stirring;

   OSL_COLOR bg;
};

/*@}*/

VALUE DrawableText_setText(VALUE self, VALUE text);

VALUE DrawableText_toggleStirring(VALUE self);
VALUE DrawableText_toggleScripted(VALUE self);

VALUE DrawableText_setBackground(VALUE self, VALUE color);
VALUE DrawableText_setFont(VALUE self, VALUE font);

VALUE DrawableText_scripted(VALUE self);
VALUE DrawableText_stirring(VALUE self);
VALUE DrawableText_setStirring(VALUE self, VALUE val);

VALUE DrawableText_background(VALUE self);
VALUE DrawableText_font(VALUE self);

VALUE DrawableText_text(VALUE self);

void defineDrawableText();

#endif
