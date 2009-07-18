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

#ifndef __MESSAGE__
#define __MESSAGE__

#include "Sprite.hpp"
#include "RubyWrapper.hpp"

class Message
{
public:
   Message();

   void draw();

   void setTitle(string txt);
   void setText(string txt);

   void setImage(Sprite *pic);
   void setBackground(Sprite *pic);

   void setBgColor(OSL_COLOR col);
   void setTitleColor(OSL_COLOR col);
   void setTextColor(OSL_COLOR col);
   void setBorderColor(OSL_COLOR col);

   void setTextFont(string f);
   void setTitleFont(string f);

   void setPos(int x, int y);
   void setTitlePos(int x, int y);

   void resize(int w, int h);
private:
   Sprite *image;
   Sprite *bg;

   string text;
   string title;

   int _h, _w;
   int _x, _y;
   int marge;

   OSL_COLOR textColor;
   OSL_COLOR titleColor;
   OSL_COLOR bgColor;
   OSL_COLOR borderColor;

   bool bg_pic;

   string titleFont;
   string textFont;

   int titleX, titleY; //relatively to _x and _y.
};

VALUE Message_draw(VALUE self);

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

VALUE Message_setPos(VALUE self, VALUE x, VALUE y);
VALUE Message_setTitlePos(VALUE self, VALUE x, VALUE y);

VALUE Message_resize(VALUE self, VALUE w, VALUE h);

void defineMessageBox();

#endif
