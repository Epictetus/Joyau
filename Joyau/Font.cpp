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

#include "Font.hpp"

IntraText::~IntraText()
{
   intraFontUnload(font);
}

void IntraText::load(const std::string &name, int options)
{
   font = intraFontLoad(name.c_str(), options);
}

int IntraText::getW() const
{
   return intraFontMeasureText(font, txt.c_str());
}

void IntraText::activate()
{
   intraFontActivate(font);
}

void IntraText::setStyle(float size, int color, int shadowColor, int option)
{
   intraFontSetStyle(font, size, color, shadowColor, option);
}

void IntraText::setEncoding(int options)
{
   intraFontSetEncoding(font, options);
}

void IntraText::setAltFont(IntraText &val)
{
   intraFontSetAltFont(font, val.font);
}

void IntraText::draw()
{
   intraFontPrint(font, getX(), getY(), txt.c_str());
}

VALUE Intrafont_init(VALUE self)
{
   intraFontInit();
   return Qnil;
}

VALUE Intrafont_stop(VALUE self)
{
   intraFontShutdown();
   return Qnil;
}

VALUE IntraText_setText(VALUE self, VALUE txt)
{
   IntraText &ref = getRef<IntraText>(self);
   ref.setText(StringValuePtr(txt));

   return txt;
}

VALUE IntraText_text(VALUE self)
{
   IntraText &ref = getRef<IntraText>(self);
   return rb_str_new2(ref.getText().c_str());
}

VALUE IntraText_load(VALUE self, VALUE name, VALUE options)
{
   IntraText &ref = getRef<IntraText>(self);
   ref.load(StringValuePtr(name), FIX2INT(options));

   return Qnil;
}

VALUE IntraText_activate(VALUE self)
{
   IntraText &ref = getRef<IntraText>(self);
   ref.activate();

   return Qnil;
}

VALUE IntraText_setStyle(VALUE self, VALUE size, VALUE color, VALUE shadow, 
			 VALUE options)
{
   IntraText &ref = getRef<IntraText>(self);
   ref.setStyle(NUM2DBL(size), FIX2INT(color), FIX2INT(shadow), 
		FIX2INT(options));

   return Qnil;
}

VALUE IntraText_setEncoding(VALUE self, VALUE options)
{
   IntraText &ref = getRef<IntraText>(self);
   ref.setEncoding(FIX2INT(options));

   return Qnil;
}

VALUE IntraText_setAltFont(VALUE self, VALUE obj)
{
   IntraText &ref = getRef<IntraText>(self);
   IntraText &val = getRef<IntraText>(obj);
   
   ref.setAltFont(val);
   return Qnil;
}

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
   defMethod(cIntraText, "load", IntraText_load, 2);
   defMethod(cIntraText, "activate", IntraText_activate, 0);
   defMethod(cIntraText, "setStyle", IntraText_setStyle, 4);
   defMethod(cIntraText, "encoding=", IntraText_setEncoding, 1);
   defMethod(cIntraText, "altFont=", IntraText_setAltFont, 1);
}
