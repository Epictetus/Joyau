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

#ifndef JOYAU_FONT
#define JOYAU_FONT

#include "Drawable.hpp"

/** @addtogroup Drawables **/
/*@{*/

/**
 * @class IntraText
 * A class which draws text using Intrafont
 */
class IntraText: public Drawable
{
public:
   IntraText():
      font(NULL), txt(""), _col(0), _shadow(0), _style(0), 
      _encoding(0), maxWidth(-1)
   { setClass("IntraText"); }

   /** Changes the text.
    *  @param text the new text.
    */
   void setText(const std::string &val) { txt = val; }

   /** returns the text **/
   std::string getText() const { return txt; }

   void setMaxWidth(int val) { maxWidth = val; }

   /** Loads a font.
    *  @param name font name
    *  @param options options for the font. Use Intrafont's constants.
    */
   void load(const std::string &name, int options);

   /** Changes the text encoding.
    *  @param options options for the font. Use Intrafont's constants.
    */
   void setEncoding(int options);

   int getW() const;
   
   /** activated the text. Should be called before drawing. **/
   void activate();

   /** Applies a style to the text.
    *  @param size the text size.
    *  @param color text's color. Use its hexadecimal color.
    *  @param shadowColor shadow's color.
    *  @param option Some intrafont's constants.
    */
   void setStyle(float size, int color, int shadowColor, int option);
   
   /** Sets a font as an alternative when a character cannot be drawed
    *  with ours.
    *  @param val alternative font.
    */
   void setAltFont(IntraText &val);

   void draw();
private:
   OSL_FONT *font;

   std::string txt;

   int _col, _shadow, _style, _encoding;
   float scale;

   int maxWidth;
};

/*@}*/

VALUE Intrafont_init(VALUE self);
VALUE Intrafont_stop(VALUE self);

VALUE IntraText_setText(VALUE self, VALUE txt);
VALUE IntraText_text(VALUE self);

VALUE IntraText_setMaxWidth(VALUE self, VALUE val);

VALUE IntraText_load(VALUE self, VALUE name, VALUE options);

VALUE IntraText_activate(VALUE self);

VALUE IntraText_setStyle(VALUE self, VALUE size, VALUE color, VALUE shadow, 
			 VALUE options);
VALUE IntraText_setEncoding(VALUE self, VALUE options);

VALUE IntraText_setAltFont(VALUE self, VALUE obj);

void defineIntrafont();

#endif
