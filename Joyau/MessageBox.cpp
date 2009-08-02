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

#include "MessageBox.hpp"

Message::Message()
{
   image = NULL;
   bg = NULL;

   text = "";
   title = "";

   setPos(0, 0);
   marge = 3;

   textColor = RGBA(0, 0, 0, 255);
   titleColor = RGBA(0, 0, 0, 255);
   bgColor = RGBA(0, 0, 255, 127);
   borderColor = RGBA(255, 255, 255, 255);

   bg_pic = false;

   titleFont = "";
   textFont = "";

   titleX = 0;
   titleY = 0;
}

void Message::draw()
{
   clearMove();
   Manager &manager = Manager::getInstance(); // Needed for getFont
   
   // Firstly, let's draw the border.
   oslDrawRect(getX(), getY(), getX() + _w, getY() + _h, borderColor);

   // Then, the background.
   // If it's a picture :
   if (bg_pic)
   {
      bg->setTile(0, 0, _w - 1, _h - 1);
      bg->setPos(getX() + 1, getY() + 1);
      bg->draw();
   }
   else
   {
      // We'll draw it in the border. We don't want to overwrit it.
      oslDrawFillRect(getX() + 1, getY() +1, getX() + _w - 1, getY() 
		      + _h - 1, bgColor);
   }

   int offset = 0;
   if (image != NULL)
   {
      image->setPos(getX() + marge, getY() + marge);
      image->draw();
      offset = image->getW();
   }

   // Now, the title should be drawn.
   // Since we'll draw text, let's stop drawing background for it :
   oslSetBkColor(RGBA(0, 0, 0, 0));
   if (title != "") // Don't lose time if there is nothing to draw...
   {
      if (titleFont != "")
	 oslSetFont(manager.getFont(titleFont.c_str()));
      oslSetTextColor(titleColor);
      oslDrawString(getX() + titleX, getY() + titleY, title.c_str());
   }

   // Let's finish with the text :
   if (textFont != "")
      oslSetFont(manager.getFont(textFont.c_str()));
   oslSetTextColor(textColor);
   oslDrawTextBox(getX() + marge + offset, getY() + marge, getX() + _w - marge + offset,
		  getY() + _h - marge, text.c_str(), 0);
}

void Message::setTitle(string txt) { title = txt; }
void Message::setText(string txt) { text = txt; }

void Message::setImage(Sprite *pic) { image = pic; }

void Message::setBackground(Sprite *pic)
{
   bg_pic = true;
   bg = pic;
}

void Message::setBgColor(OSL_COLOR col) { bgColor = col; }
void Message::setTitleColor(OSL_COLOR col) { titleColor = col; }
void Message::setTextColor(OSL_COLOR col) { textColor = col; }
void Message::setBorderColor(OSL_COLOR col) { borderColor = col; }

void Message::setTextFont(string f) { textFont = f; }
void Message::setTitleFont(string f) { titleFont = f; }

void Message::setTitlePos(int x, int y)
{
   titleX = x;
   titleY = y;
}

void Message::resize(int w, int h)
{
   _w = w;
   _h = h;
}

/*VALUE Message_draw(VALUE self)
{
   Message &ref = getRef<Message>(self);
   ref.draw();

   return Qnil;
}*/

VALUE Message_setTitle(VALUE self, VALUE txt)
{
   Message &ref = getRef<Message>(self);
   string str = StringValuePtr(txt);

   ref.setTitle(str);
   return Qnil;
}


VALUE Message_setText(VALUE self, VALUE txt)
{
   Message &ref = getRef<Message>(self);
   string str = StringValuePtr(txt);

   ref.setText(str);
   return Qnil;
}

VALUE Message_setImage(VALUE self, VALUE pic)
{
   Message &ref = getRef<Message>(self);
   Sprite *spr = getPtr<Sprite>(pic);
   
   ref.setImage(spr);
   return Qnil;
}

VALUE Message_setBackground(VALUE self, VALUE pic)
{
   Message &ref = getRef<Message>(self);
   Sprite *spr = getPtr<Sprite>(pic);
 
   ref.setBackground(spr);
   return Qnil;
}

VALUE Message_setBgColor(VALUE self, VALUE col)
{
   Message &ref = getRef<Message>(self);
   OSL_COLOR color = hash2col(col);

   ref.setBgColor(color);
   return Qnil;
}

VALUE Message_setTitleColor(VALUE self, VALUE col)
{
   Message &ref = getRef<Message>(self);
   OSL_COLOR color = hash2col(col);

   ref.setTitleColor(color);
   return Qnil;
}

VALUE Message_setTextColor(VALUE self, VALUE col)
{
   Message &ref = getRef<Message>(self);
   OSL_COLOR color = hash2col(col);

   ref.setTextColor(color);
   return Qnil;
}

VALUE Message_setBorderColor(VALUE self, VALUE col)
{
   Message &ref = getRef<Message>(self);
   OSL_COLOR color = hash2col(col);

   ref.setBorderColor(color);
   return Qnil;
}

VALUE Message_setTextFont(VALUE self, VALUE f)
{
   Message &ref = getRef<Message>(self);
   string str = StringValuePtr(f);

   ref.setTextFont(str);
   return Qnil;
}

VALUE Message_setTitleFont(VALUE self, VALUE f)
{
   Message &ref = getRef<Message>(self);
   string str = StringValuePtr(f);

   ref.setTitleFont(str);
   return Qnil;
}

/*VALUE Message_setPos(VALUE self, VALUE x, VALUE y)
{
   Message &ref = getRef<Message>(self);
   int _x = FIX2INT(x);
   int _y = FIX2INT(y);

   ref.setPos(_x, _y);
   return Qnil;
}*/

VALUE Message_setTitlePos(VALUE self, VALUE x, VALUE y)
{
   Message &ref = getRef<Message>(self);
   int _x = FIX2INT(x);
   int _y = FIX2INT(y);

   ref.setTitlePos(_x, _y);
   return Qnil;
}

VALUE Message_resize(VALUE self, VALUE w, VALUE h)
{
   Message &ref = getRef<Message>(self);
   int _w = FIX2INT(w);
   int _h = FIX2INT(h);

   ref.resize(_w, _h);
   return Qnil;
}

void defineMessageBox()
{
   VALUE cMessage = defClass<Message>("Message", "Drawable");
   defMethod(cMessage, "setTitle", Message_setTitle, 1);
   defMethod(cMessage, "setText", Message_setText, 1);
   defMethod(cMessage, "setImage", Message_setImage, 1);
   defMethod(cMessage, "setBackground", Message_setBackground, 1);
   defMethod(cMessage, "setBgColor", Message_setBgColor, 1);
   defMethod(cMessage, "setTitleColor", Message_setTitleColor, 1);
   defMethod(cMessage, "setTextColor", Message_setTextColor, 1);
   defMethod(cMessage, "setBorderColor", Message_setBorderColor, 1);
   defMethod(cMessage, "setTextFont", Message_setTextFont, 1);
   defMethod(cMessage, "setTitleFont", Message_setTitleFont, 1);
   defMethod(cMessage, "setTitlePos", Message_setTitlePos, 2);
   defMethod(cMessage, "resize", Message_resize, 2);
}
