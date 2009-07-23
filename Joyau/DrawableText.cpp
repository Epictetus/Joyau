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

#include "DrawableText.hpp"

DrawableText::DrawableText()
{
   font = "";
   stirring = false;
   scripted = false;
}

int DrawableText::getW()
{
   // Don't forget to update the font
   oslSetFont(Manager::getInstance().getFont(font.c_str()));
   return oslGetStringWidth(_text.c_str());
}

void DrawableText::toggleStirring()
{
   stirring = !stirring;
   if (stirring)
      scripted = false;
}

void DrawableText::toggleScripted()
{
   scripted = !scripted;
   if (scripted)
      stirring = false;
}

void DrawableText::draw()
{
   if (font != "")
      oslSetFont(Manager::getInstance().getFont(font.c_str()));
   
   oslSetBkColor(bg);
   oslSetTextColor(_col);

   if (stirring)
      oslPrintStirringString(getX(), getY(), _text.c_str());
   else if (scripted)
   {
      char txt[256];
      strcpy(txt, _text.c_str());
      oslScriptText(getX(), getY(), txt);
   }
   else
   {
      char txt[256];
      strcpy(txt, _text.c_str());
      oslDrawString(getX(), getY(), txt);
   }
}

VALUE DrawableText_setText(VALUE self, VALUE text)
{
   DrawableText &ref = getRef<DrawableText>(self);
   string txt = StringValuePtr(text);

   ref.setText(txt);
   return Qnil;
}

VALUE DrawableText_toggleStirring(VALUE self)
{
   DrawableText &ref = getRef<DrawableText>(self);
   ref.toggleStirring();
   return Qnil;
}

VALUE DrawableText_setColor(VALUE self, VALUE color)
{
   DrawableText &ref = getRef<DrawableText>(self);
   OSL_COLOR col = hash2col(color);

   ref.setColor(col);
   return Qnil;
}

VALUE DrawableText_setBackground(VALUE self, VALUE color)
{
   DrawableText &ref = getRef<DrawableText>(self);
   OSL_COLOR col = hash2col(color);

   ref.setBackground(col);
   return Qnil;
}

VALUE DrawableText_setFont(VALUE self, VALUE font)
{
   DrawableText &ref = getRef<DrawableText>(self);
   string str = StringValuePtr(font);

   ref.setFont(str);
   return Qnil;
}

void defineDrawableText()
{
   VALUE cDrawable = getClass("Drawable");
   VALUE cDrawableText = defClass<DrawableText>("DrawableText", cDrawable);
   defMethod(cDrawableText, "setFont", DrawableText_setFont, 1);
   defMethod(cDrawableText, "setBackground", DrawableText_setBackground, 1);
   defMethod(cDrawableText, "setColor", DrawableText_setColor, 1);
   defMethod(cDrawableText, "toggleStirring", DrawableText_toggleStirring, 0);
   defMethod(cDrawableText, "setText", DrawableText_setText, 1);
}
