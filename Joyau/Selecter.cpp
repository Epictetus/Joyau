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

#include "Selecter.hpp"

Selecter::Selecter()
{
   index = 0;

   bgColor = RGBA(0, 0, 255, 127);
   borderColor = RGBA(255, 255, 255, 255);

   focusBgColor = RGBA(0, 255, 255, 127);
   focusBorderColor = RGBA(0, 0, 0, 0); 

   txtColor = RGBA(255, 255, 255, 255);
   focusTxtColor = RGBA(255, 255, 255, 255);

   font = "";
   focusFont = "";
   
   pic = false;
   focusPic = false;

   bgPic = NULL;
   focusBgPic = NULL;

   _x = 0;
   _y = 0;
   _w = 0;

   has_focus = true;
}

void Selecter::moveUp()
{
   if (index - 1 > -1)
      index--;
}

void Selecter::moveDown()
{
   if (index + 1 < items.size())
      index++;
}

void Selecter::setPos(int x, int y)
{
   _x = x;
   _y = y;
}

void Selecter::addElem(Sprite *spr, string txt)
{
   illustrations.push(spr);
   items.push(txt);
}

void Selecter::draw()
{
   int count = 0;
   int increase = _h / items.size();

   int x = _x;
   int y = _y;

   Manager *manager = Manager::getInstance();

   // no background for text : ( faster, and, for me at least, more beautifull
   // than with one :)
   oslSetBkColor(RGBA(0, 0, 0, 0));

   for (vector<string>::iterator i = items.begin(); i != items.end(); ++i)
   {
      bool focus = (index == count) && has_focus; // We'll use different color and pictures.
      // First, we'll draw the border.

      Sprite *pic = NULL;
      // Actually, OSL_COLOR is nothing more than unsigned long
      OSL_COLOR col = 0;

      col = focus ? focusBorderColor : borderColor;
      oslDrawRect(x, y, x + _w, x + _increase, col);

      col = 0;
      // Then, the background 
      if (focus && focusBgPic != NULL)
	 pic = focusBgPic;
      else if (focus && focusBgPic == NULL)
	 col = focusBgColor;
      else if (!focus && bgPic != NULL)
	 pic = bgPic;
      else
	 col = bgColor;

      if (pic != NULL) // We don't want to do that on a NULL pointer...
      {
	 pic->setPos(x + 1, y + 1);
	 pic->Draw()
      }
      else
	 oslDrawRect(x + 1, y + 1, x + _w, y + increase, col);

      col = 0;
      pic = NULL; // We don't want to destroy it !

      pic = illustrations[count];
      int txtOffset = 0;
      if (pic != NULL)
      {
	 pic->setPos(x + 1, y + 1);
	 pic->Draw();
	 txtOffset = pic->getW();
      }
      
      string font = focus ? focusFont : font;
      col = focus ? focusTxtColor : txtColor;
      
      if (font != "") // We won't change the font if there is nothing to change
		      // ( I dont know what would hapen if I do so )
	 oslSetFont(manager->getFont(font.c_str()));

      oslSetTextColor(col);
      oslDrawTextBox(x + txtOffset, y + 3, x + _w + txtOffset,
		     _y + _h - marge, (*i).c_str(), 0);

      y += increase;
      count++;
   }
}


VALUE wrapSelecter(VALUE info)
{
   Selecter *item;
   VALUE tdata = Data_Wrap_Struct(info, 0, Selecter_free, item);
   return tdata;
}

void Selecter_free(void *info) { delete (Selecter*)info; }

VALUE Selecter_moveUp(VALUE self)
{
   Selecter *item;
   Data_Get_Struct(self, Selecter, item);

   item->moveUp();
   return Qnil;
}

VALUE Selecter_moveDown(VALUE self)
{
   Selecter *item;
   Data_Get_Struct(self, Selecter, item);

   item->moveDown();
   return Qnil;
}

VALUE Selecter_getIndex(VALUE self)
{
   Selecter *item;
   Data_Get_Struct(self, Selecter, item);

   int ret = item->getIndex();
   return INT2FIX(ret);
}

VALUE Selecter_setPos(VALUE self, VALUE x, VALUE y)
{
   Selecter *item;
   Data_Get_Struct(self, Selecter, item);

   item->setPos(FIX2INT(x), FIX2INT(y));
   return Qnil;
}

VALUE Selecter_setWidth(VALUE self, VALUE w)
{
   Selecter *item;
   Data_Get_Struct(self, Selecter, item);

   item->setWidth(FIX2INT(w));
   return Qnil;
}

VALUE Selecter_setHeigth(VALUE self, VALUE h)
{
   Selecter *item;
   Data_Get_Struct(self, Selecter, item);

   item->setHeigth(FIX2INT(h));
   return Qnil;
}

VALUE Selecter_toggleFocus(VALUE self)
{
   Selecter *item;
   Data_Get_Struct(self, Selecter, item);

   item->setWidth(FIX2INT(w));
   return Qnil;
}

VALUE Selecter_setBgPic(VALUE self, VALUE spr)
{
   Selecter *item;
   Data_Get_Struct(self, Selecter, item);

   Sprite *val;
   Data_Get_Struct(spr, Sprite, val);

   item->setBgPic(val);
   return Qnil;
}

VALUE Selecter_setFocusBgPic(VALUE self, VALUE spr)
{
   Selecter *item;
   Data_Get_Struct(self, Selecter, item);

   Sprite *val;
   Data_Get_Struct(spr, Sprite, val);

   item->setFocusBgPic(val);
   return Qnil;
}

VALUE Selecter_setBgCol(VALUE self, VALUE col)
{
   Selecter *item;
   Data_Get_Struct(self, Selecter, item);

   int r = rb_hash_aref(col, rb_str_new2("r"));
   int g = rb_hash_aref(col, rb_str_new2("g"));
   int b = rb_hash_aref(col, rb_str_new2("b"));
   int a = rb_hash_aref(col, rb_str_new2("a"));
   OSL_COLOR color = RGBA(r, g, b, a);

   item->setBgCol(color);
   return Qnil;
}

VALUE Selecter_setFocusBgCol(VALUE self, VALUE col)
{
   Selecter *item;
   Data_Get_Struct(self, Selecter, item);

   int r = rb_hash_aref(col, rb_str_new2("r"));
   int g = rb_hash_aref(col, rb_str_new2("g"));
   int b = rb_hash_aref(col, rb_str_new2("b"));
   int a = rb_hash_aref(col, rb_str_new2("a"));
   OSL_COLOR color = RGBA(r, g, b, a);

   item->setFocusBgCol(color);
   return Qnil;
}

VALUE setBorderCol(VALUE self, VALUE col)
{
   Selecter *item;
   Data_Get_Struct(self, Selecter, item);

   int r = rb_hash_aref(col, rb_str_new2("r"));
   int g = rb_hash_aref(col, rb_str_new2("g"));
   int b = rb_hash_aref(col, rb_str_new2("b"));
   int a = rb_hash_aref(col, rb_str_new2("a"));
   OSL_COLOR color = RGBA(r, g, b, a);

   item->setBorderCol(color);
   return Qnil;
}

VALUE setFocusBorderCol(VALUE self, VALUE col)
{
   Selecter *item;
   Data_Get_Struct(self, Selecter, item);

   int r = rb_hash_aref(col, rb_str_new2("r"));
   int g = rb_hash_aref(col, rb_str_new2("g"));
   int b = rb_hash_aref(col, rb_str_new2("b"));
   int a = rb_hash_aref(col, rb_str_new2("a"));
   OSL_COLOR color = RGBA(r, g, b, a);

   item->setFocusBorderCol(color);
   return Qnil;
}

VALUE Selecter_setFont(VALUE self, VALUE f)
{
   Selecter *item;
   Data_Get_Struct(self, Selecter, item);

   string str = StringValuePtr(f);
   
   item->setFont(str);
   return Qnil;
}

VALUE Selecter_setFocusFont(VALUE self, VALUE f)
{
   Selecter *item;
   Data_Get_Struct(self, Selecter, item);

   string str = StringValuePtr(f);
   
   item->setFocusFont(str);
   return Qnil;
}

VALUE Selecter_setTxtColor(VALUE self, VALUE col)
{
   Selecter *item;
   Data_Get_Struct(self, Selecter, item);

   int r = rb_hash_aref(col, rb_str_new2("r"));
   int g = rb_hash_aref(col, rb_str_new2("g"));
   int b = rb_hash_aref(col, rb_str_new2("b"));
   int a = rb_hash_aref(col, rb_str_new2("a"));
   OSL_COLOR color = RGBA(r, g, b, a);

   item->setTxtColor(color);
   return Qnil;
}

VALUE Selecter_setFocusTxtColor(VALUE self, VALUE col)
{
   Selecter *item;
   Data_Get_Struct(self, Selecter, item);

   int r = rb_hash_aref(col, rb_str_new2("r"));
   int g = rb_hash_aref(col, rb_str_new2("g"));
   int b = rb_hash_aref(col, rb_str_new2("b"));
   int a = rb_hash_aref(col, rb_str_new2("a"));
   OSL_COLOR color = RGBA(r, g, b, a);

   item->setFocusTxtColor(color);
   return Qnil;
}

VALUE Selecter_addElem(VALUE self, VALUE spr, VALUE txt)
{
   Selecter *item;
   Data_Get_Struct(self, Selecter, item);

   Sprite *val;
   Data_Get_Struct(spr, Sprite, val);

   string str = StringValuePtr(txt);
   item->addElem(val, str);

   return Qnil;
}

VALUE Selecter_draw(VALUE self)
{
   Selecter *item;
   Data_Get_Struct(self, Selecter, item);

   item->draw();
   return Qnil;
}

void defineSelecter()
{
   VALUE cSelecter = rb_define_class("Selecter", rb_cObject);
   rb_define_singleton_method(cSelecter, "new", (VALUE(*)(...))&wrapSelecter, 
			      0);
   rb_define_method(cSelecter, "moveUp", (VALUE(*)(...))&Selecter_moveUp, 0);
   rb_define_method(cSelecter, "moveDown", (VALUE(*)(...))&Selecter_moveDown, 
		    0);
   rb_define_method(cSelecter, "getIndex", (VALUE(*)(...))&Selecter_getIndex, 
		    0);
   rb_define_method(cSelecter, "setPos", (VALUE(*)(...))&Selecter_setPos, 2);
   rb_define_method(cSelecter, "setWidth", (VALUE(*)(...))&Selecter_setWidth, 
		    1);
   rb_define_method(cSelecter, "toggleFocus", 
		    (VALUE(*)(...))&Selecter_toggleFocus, 0);
   rb_define_method(cSelecter, "setBgPic", (VALUE(*)(...))&Selecter_setBgPic, 
		    1);
   rb_define_method(cSelecter, "setFocusBgPic", 
		    (VALUE(*)(...))&Selecter_setFocusBgPic, 1);
   rb_define_method(cSelecter, "setBgCol", (VALUE(*)(...))&Selecter_setBgCol, 
		    1);
   rb_define_method(cSelecter, "setFocusBgCol", 
		    (VALUE(*)(...))&Selecter_setFocusBgCol, 1);
   rb_define_method(cSelecter, "setBorderCol", 
		    (VALUE(*)(...))&Selecter_setBorderCol, 1);
   rb_define_method(cSelecter, "setFocusBorderCol", 
		    (VALUE(*)(...))&Selecter_setFocusBorderCol, 1);
   rb_define_method(cSelecter, "setFont", (VALUE(*)(...))&Selecter_setFont, 1);
   rb_define_method(cSelecter, "setFocusFont", (VALUE(*)(...))&Selecter_setFocusFont, 1);
   rb_define_method(cSelecter, "setTxtColor", (VALUE(*)(...))&Selecter_setTxtColor, 1);
   
}
