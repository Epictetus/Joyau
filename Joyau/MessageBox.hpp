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

#ifndef JOYAU_MESSAGE
#define JOYAU_MESSAGE

#include "Drawable.hpp"

class Sprite;

class Message: public Drawable
{
public:
   Message();

   void draw();

   void setTitle(std::string txt);
   void setText(std::string txt);

   void setImage(Sprite *pic);
   void setBackground(Sprite *pic);

   void setBgColor(OSL_COLOR col);
   void setTitleColor(OSL_COLOR col);
   void setTextColor(OSL_COLOR col);
   void setBorderColor(OSL_COLOR col);

   void setTextFont(std::string f);
   void setTitleFont(std::string f);

   void setTitlePos(int x, int y);
   void setTitlePos(const Point &p);

   std::string getTitle() const { return title; }
   std::string getText() const { return text; }

   Sprite& getImage() const { return *image; }
   Sprite& getBackground() const { return *bg; }

   OSL_COLOR getBgColor() const { return bgColor; }
   OSL_COLOR getTitleColor() const { return titleColor; }
   OSL_COLOR getTextColor() const { return textColor; }
   OSL_COLOR getBorderColor() const { return borderColor; }

   std::string getTitleFont() const { return titleFont; }
   std::string getTextFont() const { return textFont; }

   Point getTitlePos() const { return Point(titleX, titleY); }

   void resize(int w, int h);
   void setW(int w) { _w = w; }
   void setH(int h) { _h = h; }
private:
   Sprite *image;
   Sprite *bg;

   std::string text;
   std::string title;

   int marge;

   OSL_COLOR textColor;
   OSL_COLOR titleColor;
   OSL_COLOR bgColor;
   OSL_COLOR borderColor;

   bool bg_pic;

   std::string titleFont;
   std::string textFont;

   int titleX, titleY; //relatively to _x and _y.
};

VALUE Message_setTitle(VALUE self, VALUE txt);
VALUE Message_setText(VALUE self, VALUE txt);

VALUE Message_setImage(VALUE self, VALUE pic);
VALUE Message_setBackground(VALUE self, VALUE pic);

VALUE Message_setBgColor(VALUE self, VALUE col);
VALUE Message_setTitleColor(VALUE self, VALUE col);
VALUE Message_setTextColor(VALUE self, VALUE col);
VALUE Message_setBorderColor(VALUE self, VALUE col);

VALUE Message_setTextFont(VALUE self, VALUE f);
VALUE Message_setTitleFont(VALUE self, VALUE f);

VALUE Message_setTitlePos(VALUE self, VALUE x, VALUE y);
VALUE Message_setTitlePoint(VALUE self, VALUE p);

VALUE Message_title(VALUE self);
VALUE Message_text(VALUE self);

VALUE Message_image(VALUE self);
VALUE Message_background(VALUE self);

VALUE Message_bgColor(VALUE self);
VALUE Message_titleColor(VALUE self);
VALUE Message_textColor(VALUE self);
VALUE Message_borderColor(VALUE self);

VALUE Message_textFont(VALUE self);
VALUE Message_titleFont(VALUE self);

VALUE Message_titlePos(VALUE self);

VALUE Message_resize(VALUE self, VALUE w, VALUE h);
VALUE Message_setW(VALUE self, VALUE w);
VALUE Message_setH(VALUE self, VALUE h);

void defineMessageBox();

#endif
