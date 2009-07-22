#include "DrawableText.hpp"

int DrawableText::getW()
{
   // Don't forget to update the font
   oslSetFont(Manager::getInstance().getFont(font.c_str()));
   return oslGetStringWidth(_text.c_str());
}

void DrawableText::draw()
{
   if (font != "")
      oslSetFont(Manager::getInstance().getFont(font.c_str()));
   
   oslSetBkColor(bg);
   oslSetTextColor(_col);

   if (stirring)
      oslPrintStirringString(getX(), getY(), _text.c_str());
   else
   {
      char txt[256];
      strcpy(txt, _text.c_str());
      oslScriptText(getX(), getY(), txt);
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
