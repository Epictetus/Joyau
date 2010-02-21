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

#include "Font.hpp"
#include "Manager.hpp"

void IntraText::load(const std::string &name, int options)
{
   font = Manager::getInstance().getIntraFont(name, options);
   if (font == NULL)
      throw RubyException(rb_eRuntimeError, "the font could not be loaded.");
}

int IntraText::getW() const
{
   return maxWidth == -1 ? 
      intraFontMeasureText(font->intra, txt.c_str()) : 
      maxWidth;
}

void IntraText::activate()
{
   intraFontActivate(font->intra);
}

void IntraText::setStyle(float size, int color, int shadowColor, int option)
{
   scale   = size;
   _col    = color;
   _shadow = shadowColor;
   _style  = option;
}

void IntraText::setEncoding(int options)
{
   _encoding = options;
}

void IntraText::setAltFont(IntraText &val)
{
   intraFontSetAltFont(font->intra, val.font->intra);
}

void IntraText::draw()
{
   intraFontSetEncoding(font->intra, _encoding);
   intraFontSetStyle(font->intra, scale, _col, _shadow, _style);
   
   OSL_FONT *old_font = osl_curFont;
   oslSetFont(font);

   if (maxWidth == -1)
      oslDrawString(getX(), getY(), txt.c_str());
   else
      oslIntraFontPrintColumn(font,
			      getX(), getY(),
			      maxWidth,
			      1,
			      txt.c_str());
   
   oslSetFont(old_font);
}

Font::Font(): font(NULL),
	      _col(RGBA(255, 255, 255, 255)),
	      _shadow(RGBA(0, 0, 0, 0)),
	      _style(0),
	      _encoding(INTRAFONT_STRING_UTF8),
	      scale(1.f),
	      background(RGBA(255, 255, 255, 255)),
	      foreground(RGBA(0, 0, 0, 0))
{}

Font::Font(const std::string &filename):
   _col(RGBA(255, 255, 255, 255)),
   _shadow(RGBA(0, 0, 0, 0)),
   _style(0),
   _encoding(INTRAFONT_STRING_UTF8),
   scale(1.f),
   background(RGBA(255, 255, 255, 255)),
   foreground(RGBA(0, 0, 0, 0))
{
   font = Manager::getInstance().getFont(filename.c_str());
}

Font::Font(const std::string &filename, int option):
   _col(RGBA(255, 255, 255, 255)),
   _shadow(RGBA(0, 0, 0, 0)),
   _style(0),
   _encoding(INTRAFONT_STRING_UTF8),
   scale(1.f),
   background(RGBA(255, 255, 255, 255)),
   foreground(RGBA(0, 0, 0, 0))
{
   font = Manager::getInstance().getIntraFont(filename.c_str(), option);
}

Font::Font(OSL_FONT *argFont): font(argFont),
			       _col(RGBA(255, 255, 255, 255)),
			       _shadow(RGBA(0, 0, 0, 0)),
			       _style(0),
			       _encoding(INTRAFONT_STRING_UTF8),
			       scale(1.f),
			       background(RGBA(255, 255, 255, 255)),
			       foreground(RGBA(0, 0, 0, 0))
{}

void Font::load(const std::string &filename) {
   font = Manager::getInstance().getFont(filename.c_str());
}

void Font::load(const std::string &filename, int options) {
   font = Manager::getInstance().getIntraFont(filename.c_str(), options);
}

void Font::setFont(OSL_FONT *argFont) {
   font = argFont;
}

void Font::print(int x, int y, const std::string &text) {
   OSL_FONT *old_font = osl_curFont;
   oslSetFont(font);

   if (font->fontType == OSL_FONT_OFT) {
      oslSetTextColor(foreground);
      oslSetBkColor(background);
   }
   else if (font->fontType == OSL_FONT_INTRA) {
      intraFontSetEncoding(font->intra, _encoding);
      oslIntraFontSetStyle(font, scale, _col, _shadow, _style);
   }

   oslDrawString(x, y, text.c_str());

   oslSetFont(old_font);
}

void Font::printInRect(Rect rect, const std::string &text) {
   OSL_FONT *old_font = osl_curFont;
   oslSetFont(font);

   if (font->fontType == OSL_FONT_OFT) {
      oslSetTextColor(foreground);
      oslSetBkColor(background);
   }
   else if (font->fontType == OSL_FONT_INTRA) {
      intraFontSetEncoding(font->intra, _encoding);
      oslIntraFontSetStyle(font, scale, _col, _shadow, _style);
   }

   oslDrawTextBox(rect.x, rect.y, rect.x + rect.w, rect.y + rect.h,
		  text.c_str(), 0);

   oslSetFont(old_font);
}

void Font::setColors(OSL_COLOR bg, OSL_COLOR fg) {
   background = bg;
   foreground = fg;
}

void Font::setStyle(float size, OSL_COLOR color, OSL_COLOR shadowColor,
		    int option) {
   scale = size;
   _col = color;
   _shadow = shadowColor;
   _style = option;
}

void Font::setEncoding(int encoding) {
   _encoding = encoding;
}

int Font::charHeight() {
   return font->charHeight * scale;
}

int Font::stringWidth(const std::string &str) {
   OSL_FONT *old_font = osl_curFont;
   oslSetFont(font);
   
   if (font->fontType == OSL_FONT_OFT) {
      oslSetTextColor(foreground);
      oslSetBkColor(background);
   }
   else if (font->fontType == OSL_FONT_INTRA) {
      intraFontSetEncoding(font->intra, _encoding);
      oslIntraFontSetStyle(font, scale, _col, _shadow, _style);
   }

   int ret = oslGetStringWidth(str.c_str());
   oslSetFont(old_font);

   return ret;
}

/*
  Inits the intrafont module.
*/
VALUE Intrafont_init(VALUE self)
{
   oslIntraFontInit(0);
   return Qnil;
}

/*
  Stops the intrafont module.
*/
VALUE Intrafont_stop(VALUE self)
{
   oslIntraFontShutdown();
   return Qnil;
}

/*
  call-seq: text=(val)

  Sets an intratext's content.
*/
VALUE IntraText_setText(VALUE self, VALUE txt)
{
   txt = rb_obj_as_string(txt);
   IntraText &ref = getRef<IntraText>(self);
   ref.setText(StringValuePtr(txt));

   return txt;
}

/*
  Returns an intratext's content.
*/
VALUE IntraText_text(VALUE self)
{
   IntraText &ref = getRef<IntraText>(self);
   return rb_str_new2(ref.getText().c_str());
}

/*
  call-seq: max_width=(val)

  Sets an intratext's maximum width.
*/
VALUE IntraText_setMaxWidth(VALUE self, VALUE val) {
   IntraText &ref = getRef<IntraText>(self);
   ref.setMaxWidth(FIX2INT(val));

   return val;
}

/*
  call-seq: load(font_name, options)

  Loads a font. You can use flags as option, or give 0.
*/
VALUE IntraText_load(VALUE self, VALUE name, VALUE options)
{
   name = rb_obj_as_string(name);

   IntraText &ref = getRef<IntraText>(self);
   try {
      ref.load(StringValuePtr(name), FIX2INT(options));
   }
   catch (const RubyException &e) {
      e.rbRaise();
   }

   return Qnil;
}

/*
  Activates a font.
*/
VALUE IntraText_activate(VALUE self)
{
   IntraText &ref = getRef<IntraText>(self);
   ref.activate();

   return Qnil;
}

/*
  call-seq: setStyle(size, color, shadow, options)

  Sets a font style. Flags of options can be given too.
*/
VALUE IntraText_setStyle(VALUE self, VALUE size, VALUE color, VALUE shadow, 
			 VALUE options)
{
   IntraText &ref = getRef<IntraText>(self);
   ref.setStyle(NUM2DBL(size), hash2col(color), hash2col(shadow), 
		FIX2INT(options));

   return Qnil;
}

/*
  call-seq: encoding=(options)

  Sets the text's encoding (required for non-ASCII texts).
*/
VALUE IntraText_setEncoding(VALUE self, VALUE options)
{
   IntraText &ref = getRef<IntraText>(self);
   ref.setEncoding(FIX2INT(options));

   return Qnil;
}

/*
  call-seq: altFont=(font)

  Sets the font from which characters will be taken 
  if it is not in this one.
*/
VALUE IntraText_setAltFont(VALUE self, VALUE obj)
{
   if (!rb_obj_is_kind_of(obj, getClass("IntraText")))
       rb_raise(rb_eTypeError, "Can't convert %s into Joyau::IntraText",
		rb_obj_classname(obj));

   IntraText &ref = getRef<IntraText>(self);
   IntraText &val = getRef<IntraText>(obj);
   
   ref.setAltFont(val);
   return Qnil;
}

template<>
/*
  Creates a new font. You can load an OFT font if you give a fontname, e.g.
    Joyau::Font.new("my_font.oft")
  and an intrafont if you give also the loading options :
    Joyau::Font.new("flash0:/font/ltn10.pgf", 0)
*/
VALUE wrap<Font>(int argc, VALUE *argv, VALUE info) {
   VALUE filename, options;
   rb_scan_args(argc, argv, "02", &filename, &options);

   Font *ptr = NULL;
   if (NIL_P(filename)) {
      ptr = new Font;
   }
   else {
      filename = rb_obj_as_string(filename);
      if (NIL_P(options))
	 ptr = new Font(StringValuePtr(filename));
      else 
	 ptr = new Font(StringValuePtr(filename), 
			FIX2INT(options));
   }

   VALUE tdata = Data_Wrap_Struct(info, 0, wrapped_free<Font>, ptr);
   return tdata;
}

/*
  Loads a font. If only one argument, containing the file to load, is given,
  it loads an OFT. If a second parameter, containing loading parameters, is
  given, then an intrafont is loaded.
*/
VALUE Font_load(int argc, VALUE *argv, VALUE self) {
   Font &ref = getRef<Font>(self);
   
   VALUE filename, options;
   rb_scan_args(argc, argv, "11", &filename, &options);
   
   filename = rb_obj_as_string(filename);
   if (NIL_P(options))
      ref.load(StringValuePtr(filename));
   else 
      ref.load(StringValuePtr(filename), FIX2INT(options));

   return Qnil;
}

/*
  This function can be used either to write some text at a specific position,
  or in a rect.
    font.print(x, y, "Hello world !")
    font.print(Joyau::Rect.new(10, 10, 100, 100), "Heeeeeeeeeeelo wooooorld")
*/
VALUE Font_print(int argc, VALUE *argv, VALUE self) {
   Font &ref = getRef<Font>(self);
   
   VALUE arg1, arg2, arg3;
   rb_scan_args(argc, argv, "21", &arg1, &arg2, &arg3);
   
   VALUE text = NIL_P(arg3) ? rb_obj_as_string(arg2) : rb_obj_as_string(arg3);
   if (NIL_P(arg3))
      ref.printInRect(getRef<Rect>(arg1), StringValuePtr(text));
   else
      ref.print(FIX2INT(arg1), FIX2INT(arg2), StringValuePtr(text));

   return Qnil;
}

/*
  call-seq: setColors(bg, fg)

  Sets the colors used for OFT fonts (bg being the background's color,
  and fg the foureground's)
*/
VALUE Font_setColors(VALUE self, VALUE bg, VALUE fg) {
   Font &ref = getRef<Font>(self);
   ref.setColors(hash2col(bg), hash2col(fg));

   return Qnil;
}

/*
  call-seq: setStyle(scale, color, shadow_color, options)

  Sets the font's style (only used for intrafont).
    font.setStyle(0.7, Joyau::Color::RED, Joyau::Color::BLUE, 0)
*/
VALUE Font_setStyle(VALUE self, 
		    VALUE size, VALUE color, VALUE shadowColor, VALUE option) {
   Font &ref = getRef<Font>(self);
   ref.setStyle(NUM2DBL(size), hash2col(color), hash2col(shadowColor), 
		FIX2INT(option));

   return Qnil;
}

/*
  call-seq: encoding=(encoding)

  Sets the font's encoding (onyly for intrafont)
*/
VALUE Font_setEncoding(VALUE self, VALUE encoding) {
   Font &ref = getRef<Font>(self);
   ref.setEncoding(FIX2INT(encoding));

   return encoding;
}

/*
  Returns the height used by a character.
*/
VALUE Font_charHeight(VALUE self) {
   return INT2FIX(getRef<Font>(self).charHeight());
}

/*
  call-seq: width_of(str)

  Returns the width of a string if it were to be drawn with self.
*/
VALUE Font_stringWidth(VALUE self, VALUE str) {
   Font &ref = getRef<Font>(self);
   str = rb_obj_as_string(str);

   return INT2FIX(ref.stringWidth(StringValuePtr(str)));
}

/*
  Returns the font's background
*/
VALUE Font_getBackground(VALUE self) {
   Font &ref = getRef<Font>(self);
   return col2hash(ref.getBackground());
}

/*
  Return's the font's foreground.
*/
VALUE Font_getForeground(VALUE self) {
   Font &ref = getRef<Font>(self);
   return col2hash(ref.getForeground());
}

/*
  Returns the font's color (for intrafont)
*/
VALUE Font_getColor(VALUE self) {
   Font &ref = getRef<Font>(self);
   return col2hash(ref.getColor());
}

/*
  Returns the font's shadow's color.
*/
VALUE Font_getShadowColor(VALUE self) {
   Font &ref = getRef<Font>(self);
   return col2hash(ref.getShadowColor());
}

/*
  Returns the font's option (for intrafont)
*/
VALUE Font_getOptions(VALUE self) {
   Font &ref = getRef<Font>(self);
   return INT2FIX(ref.getOptions());
}

/*
  Returns the font's encoding (for intrafont).
*/
VALUE Font_getEncoding(VALUE self) {
   Font &ref = getRef<Font>(self);
   return INT2FIX(ref.getEncoding());
}

/*
  Returns the font's scale (for intrafont).
*/
VALUE Font_getScale(VALUE self) {
   Font &ref = getRef<Font>(self);
   return rb_float_new(ref.getScale());
}

/*
  Returns whether or not this font uses intrafont.
*/
VALUE Font_is_intra(VALUE self) {
   Font &ref = getRef<Font>(self);
   return ref.isIntrafont() ? Qtrue : Qfalse;
}

/*
  Returns the default font.
*/
VALUE Font_default(VALUE self) {
   Font obj = Font::defaultFont();
   return createObject(getClass("Font"), obj);
}

/*
  Returns the actual font.
*/
VALUE Font_actual(VALUE self) {
   Font obj = Font::actualFont();
   return createObject(getClass("Font"), obj);
}

/*
  Document-class: Joyau::Intrafont
  
  Intrafont is used to load fonts present in the PSP's flash.
  Constant defined here can be used when loading them.
*/

/*
  Document-class: Joyau::Intratext

  Drawable used when drawing text through Intrafont.
*/

/*
  Document-class: Joyau::Font

  Represents a font, you can draw text with. It can be used to draw
  text easier than with individual objects.
*/
void defineIntrafont()
{
   VALUE mIntraFont = defModule("Intrafont");
   defModFunc(mIntraFont, "init", Intrafont_init, 0);
   defModFunc(mIntraFont, "stop", Intrafont_stop, 0);

   defConst(mIntraFont, "ADVANCE_H", INT2FIX(INTRAFONT_ADVANCE_H));
   defConst(mIntraFont, "ADVANCE_V", INT2FIX(INTRAFONT_ADVANCE_V));

   defConst(mIntraFont, "ALIGN_LEFT", INT2FIX(INTRAFONT_ALIGN_LEFT));
   defConst(mIntraFont, "ALIGN_CENTER", INT2FIX(INTRAFONT_ALIGN_CENTER));
   defConst(mIntraFont, "ALIGN_RIGHT", INT2FIX(INTRAFONT_ALIGN_RIGHT));
   defConst(mIntraFont, "ALIGN_FULL", INT2FIX(INTRAFONT_ALIGN_FULL));

   defConst(mIntraFont, "SCROLL_LEFT", INT2FIX(INTRAFONT_SCROLL_LEFT));
   defConst(mIntraFont, "SCROLL_SEESAW", INT2FIX(INTRAFONT_SCROLL_SEESAW));
   defConst(mIntraFont, "SCROLL_RIGHT", INT2FIX(INTRAFONT_SCROLL_RIGHT));
   defConst(mIntraFont, "SCROLL_THROUGH", INT2FIX(INTRAFONT_SCROLL_THROUGH));

   defConst(mIntraFont, "WIDTH_VAR", INT2FIX(INTRAFONT_WIDTH_VAR));
   defConst(mIntraFont, "WIDTH_FIX", INT2FIX(INTRAFONT_WIDTH_FIX));

   defConst(mIntraFont, "ACTIVE", INT2FIX(INTRAFONT_ACTIVE));

   defConst(mIntraFont, "CACHE_MED", INT2FIX(INTRAFONT_CACHE_MED));
   defConst(mIntraFont, "CACHE_LARGE", INT2FIX(INTRAFONT_CACHE_LARGE));
   defConst(mIntraFont, "CACHE_ASCII", INT2FIX(INTRAFONT_CACHE_ASCII));
   defConst(mIntraFont, "CACHE_ALL", INT2FIX(INTRAFONT_CACHE_ALL));

   defConst(mIntraFont, "STRING_ASCII", INT2FIX(INTRAFONT_STRING_ASCII));
   defConst(mIntraFont, "STRING_CP437", INT2FIX(INTRAFONT_STRING_CP437));
   defConst(mIntraFont, "STRING_CP850", INT2FIX(INTRAFONT_STRING_CP850));
   defConst(mIntraFont, "STRING_CP866", INT2FIX(INTRAFONT_STRING_CP866));
   defConst(mIntraFont, "STRING_SJIS", INT2FIX(INTRAFONT_STRING_SJIS));
   defConst(mIntraFont, "STRING_GBK", INT2FIX(INTRAFONT_STRING_GBK));
   defConst(mIntraFont, "STRING_KOR", INT2FIX(INTRAFONT_STRING_KOR));
   defConst(mIntraFont, "STRING_BIG5", INT2FIX(INTRAFONT_STRING_BIG5));
   defConst(mIntraFont, "STRING_CP1251", INT2FIX(INTRAFONT_STRING_CP1251));
   defConst(mIntraFont, "STRING_CP1252", INT2FIX(INTRAFONT_STRING_CP1252));
   defConst(mIntraFont, "STRING_UTF8", INT2FIX(INTRAFONT_STRING_UTF8));
   
   VALUE cIntraText = defClass<IntraText>("IntraText", "Drawable");
   defMethod(cIntraText, "text=", IntraText_setText, 1);
   defMethod(cIntraText, "text", IntraText_text, 0);
   defMethod(cIntraText, "max_width=", IntraText_setMaxWidth, 1);
   defMethod(cIntraText, "load", IntraText_load, 2);
   defMethod(cIntraText, "activate", IntraText_activate, 0);
   defMethod(cIntraText, "setStyle", IntraText_setStyle, 4);
   defMethod(cIntraText, "encoding=", IntraText_setEncoding, 1);
   defMethod(cIntraText, "altFont=", IntraText_setAltFont, 1);

   VALUE cFont = defClass<Font>("Font");
   defMethod(cFont, "load", Font_load, -1);
   defMethod(cFont, "print", Font_print, -1);
   defMethod(cFont, "set_colors", Font_setColors, 2);
   defMethod(cFont, "set_style", Font_setStyle, 4);
   defMethod(cFont, "encoding=", Font_setEncoding, 1);
   defMethod(cFont, "char_height", Font_charHeight, 0);
   defMethod(cFont, "width_of", Font_stringWidth, 1);
   defMethod(cFont, "background", Font_getBackground, 0);
   defMethod(cFont, "foreground", Font_getForeground, 0);
   defMethod(cFont, "color", Font_getColor, 0);
   defMethod(cFont, "shadowColor", Font_getShadowColor, 0);
   defMethod(cFont, "options", Font_getOptions, 0);
   defMethod(cFont, "encoding", Font_getEncoding, 0);
   defMethod(cFont, "scale", Font_getScale, 0);
   defMethod(cFont, "is_intra?", Font_is_intra, 0);

   defClassMethod(cFont, "default", Font_default, 0);
   defClassMethod(cFont, "actual", Font_actual, 0);

   defAlias(cFont, "set_style", "setStyle");
}
