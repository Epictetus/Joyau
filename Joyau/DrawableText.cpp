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

#include "DrawableText.hpp"
#include "Manager.hpp"

DrawableText::DrawableText()
{
   font = "";
   stirring = false;
   scripted = false;
   bg = RGBA(0, 0, 0, 0);
   _col[0] = RGBA(255, 255, 255, 255);

   setClass("DrawableText");
}

int DrawableText::getW() const
{
   // Don't forget to update the font
   if (font != "")
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
   oslSetTextColor(_col[0]);

   if (stirring)
      oslPrintStirringString(getX(), getY(), _text.c_str());
   else if (scripted)
   {
      char *txt = new char[_text.size() + 1];
      strcpy(txt, _text.c_str());
      oslScriptText(getX(), getY(), txt);
   }
   else
      oslDrawString(getX(), getY(), _text.c_str());
}

/*
  Document-class: Joyau::DrawableText

  Class used when drawing text, using either a built-in or an OFT font.
*/

/*
  call-seq: setText(str)
            text=(str)

  Sets a drawable's text.
*/
VALUE DrawableText_setText(VALUE self, VALUE text)
{
   DrawableText &ref = getRef<DrawableText>(self);
   std::string txt = StringValuePtr(text);

   ref.setText(txt);
   return text;
}

/*
  Toggle the stirring mode.
*/
VALUE DrawableText_toggleStirring(VALUE self)
{
   DrawableText &ref = getRef<DrawableText>(self);
   ref.toggleStirring();
   return Qnil;
}

/*
  Toggle the scripted mode. When set to true, characters like \n can be used.
*/
VALUE DrawableText_toggleScripted(VALUE self)
{
   DrawableText &ref = getRef<DrawableText>(self);
   ref.toggleScripted();
   return Qnil;
}


/*
  call-seq: setBackground(color)
            background=(color)

  Sets the text's background.
*/
VALUE DrawableText_setBackground(VALUE self, VALUE color)
{
   DrawableText &ref = getRef<DrawableText>(self);
   OSL_COLOR col = hash2col(color);

   ref.setBackground(col);
   return color;
}

/*
  call-seq: setFont(font)
            font=(font)

  Loads an OFT. "" can be given to remove the actual font. 
*/
VALUE DrawableText_setFont(VALUE self, VALUE font)
{
   DrawableText &ref = getRef<DrawableText>(self);
   std::string str = StringValuePtr(font);

   ref.setFont(str);
   return font;
}

/*
  Returns the scripted state.
*/
VALUE DrawableText_scripted(VALUE self)
{
   DrawableText &ref = getRef<DrawableText>(self);
   return ref.isScripted() ? Qtrue : Qfalse;
}

/*
  Returns the stirring state.
*/
VALUE DrawableText_stirring(VALUE self)
{
   DrawableText &ref = getRef<DrawableText>(self);
   return ref.isStirring() ? Qtrue : Qfalse;
}

/*
  call-seq: setStirring(val)
            stirring=(val)
  
  Sets the stirring state.
*/
VALUE DrawableText_setStirring(VALUE self, VALUE val)
{
   DrawableText &ref = getRef<DrawableText>(self);
   ref.setStirring(val == Qtrue);

   return val;
}

/*
  Returns the text's background.
*/
VALUE DrawableText_background(VALUE self)
{
   DrawableText &ref = getRef<DrawableText>(self);
   return col2hash(ref.getBackground());
}

/*
  Returns the text's font name.
*/
VALUE DrawableText_font(VALUE self)
{
   DrawableText &ref = getRef<DrawableText>(self);
   return rb_str_new2(ref.getFont().c_str());
}

/*
  Returns the text's text.
*/
VALUE DrawableText_text(VALUE self)
{
   DrawableText &ref = getRef<DrawableText>(self);
   return rb_str_new2(ref.getText().c_str());
}

void defineDrawableText()
{
   VALUE cDrawableText = defClass<DrawableText>("DrawableText", "Shape");
   defMethod(cDrawableText, "setFont", DrawableText_setFont, 1);
   defMethod(cDrawableText, "setBackground", DrawableText_setBackground, 1);
   defMethod(cDrawableText, "toggleStirring", DrawableText_toggleStirring, 0);
   defMethod(cDrawableText, "toggleScripted", DrawableText_toggleScripted, 0);
   defMethod(cDrawableText, "setText", DrawableText_setText, 1);
   defMethod(cDrawableText, "scripted", DrawableText_scripted, 0);
   defMethod(cDrawableText, "stirring", DrawableText_stirring, 0);
   defMethod(cDrawableText, "stirring=", DrawableText_setStirring, 1);
   defMethod(cDrawableText, "background", DrawableText_background, 0);
   defMethod(cDrawableText, "font", DrawableText_font, 0);
   defMethod(cDrawableText, "text", DrawableText_text, 0);

   defAlias(cDrawableText, "setFont", "font=");
   defAlias(cDrawableText, "setBackground", "background=");
   defAlias(cDrawableText, "setText", "text=");
}
