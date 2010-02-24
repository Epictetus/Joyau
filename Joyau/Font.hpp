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

class Font {
public:
   Font();
   Font(const std::string &filename);
   Font(const std::string &filename, int option); // Intrafont
   Font(OSL_FONT *font);

   void load(const std::string &filename);
   void load(const std::string &filename, int options);
   
   void setFont(OSL_FONT *font);

   void print(int x, int y, const std::string &text);
   void printInRect(Rect rect, const std::string &text);

   void setColors(OSL_COLOR bg, OSL_COLOR fg);
   void setStyle(float size, OSL_COLOR color, OSL_COLOR shadowColor, int option);
   void setEncoding(int encoding);

   int charHeight();
   int stringWidth(const std::string &str);

   OSL_COLOR getBackground() const { return background; }
   OSL_COLOR getForeground() const { return foreground; }

   int getColor() const { return _col; }
   int getShadowColor() const { return _shadow; }
   int getOptions() const { return _style; }
   int getEncoding() const { return _encoding; }
   float getScale() const { return scale; }

   bool isIntrafont() const {
      return font ? font->fontType == OSL_FONT_INTRA : false;
   }
   
   static Font defaultFont() {
      return Font(osl_sceFont);
   }

   static Font actualFont() {
      return Font(osl_curFont);
   }
private:
   OSL_FONT *font;

   // Configuration for intrafont :
   OSL_COLOR _col, _shadow;
   int _style, _encoding;
   float scale;

   // And for normal fonts :
   OSL_COLOR background, foreground;
};

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

VALUE Font_load(int argc, VALUE *argv, VALUE self);

VALUE Font_print(int argc, VALUE *argv, VALUE self);

VALUE Font_setColors(VALUE self, VALUE bg, VALUE fg);
VALUE Font_setStyle(VALUE self, 
		    VALUE size, VALUE color, VALUE shadowColor, VALUE option);
VALUE Font_setEncoding(VALUE self, VALUE encoding);

VALUE Font_charHeight(VALUE self);
VALUE Font_stringWidth(VALUE self, VALUE str);

VALUE Font_getBackground(VALUE self);
VALUE Font_getForeground(VALUE self);

VALUE Font_getColor(VALUE self);
VALUE Font_getShadowColor(VALUE self);
VALUE Font_getOptions(VALUE self);
VALUE Font_getEncoding(VALUE self);
VALUE Font_getScale(VALUE self);

VALUE Font_is_intra(VALUE self);

VALUE Font_default(VALUE self);
VALUE Font_actual(VALUE self);

void defineIntrafont();

#endif
