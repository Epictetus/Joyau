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

#ifndef __SELECTER__
#define __SELECTER__

#include "Manager.hpp"

class Selecter
{
public:
   Selecter();

   void moveUp();
   void moveDown();

   int getIndex() { return index; }
   
   void setPos(int x, int y);

   void setWidth(int w) { _w = w; }
   void setHeigth(int h) { _h = h; }

   void toggleFocus() { has_focus = has_focus ? false : true; }

   void setBgPic(Sprite *spr) { bgPic = spr; }
   void setFocusBgPic(Sprite *spr) { focusBgPic = spr; }

   void setBgCol(OSL_COLOR col) { bgColor = col; }
   void setFocusBgCol(OSL_COLOR col) { focusBgColor = col; }

   void setBorderCol(OSL_COLOR col) { borderColor = col; }
   void setFocusBorderCol(OSL_COLOR col) { focusBorderColor = col; }

   void setFont(string f) { font = f; }
   void setFocusFont(string f) { focusFont = f; }

   void setTxtColor(OSL_COLOR col) { txtColor = col; }
   void setFocusTxtColor(OSL_COLOR col) { focusTxtColor = col; }

   void addElem(Sprite *spr, string txt);
   void draw();

private:
   int index;

   OSL_COLOR bgColor;
   OSL_COLOR borderColor;

   OSL_COLOR focusBgColor;
   OSL_COLOR focusBorderColor; 

   OSL_COLOR txtColor;
   OSL_COLOR focusTxtColor;

   string font;
   string focusFont;
   
   bool pic;
   bool focusPic;

   bool has_focus; // If set to false, the focus isn't applied.

   Sprite *bgPic;
   Sprite *focusBgPic;

   int _x, _y, _w, _h;
   vector<string> items;
   vector<Sprite*> illustrations;
};

VALUE wrapSelecter(VALUE info);
void Selecter_free(void *info);

VALUE Selecter_moveUp(VALUE self);
VALUE Selecter_moveDown(VALUE self);

VALUE Selecter_getIndex(VALUE self);
   
VALUE Selecter_setPos(VALUE self, VALUE x, VALUE y);

VALUE Selecter_setWidth(VALUE self, VALUE w);
VALUE Selecter_setHeigth(VALUE self, VALUE h);

VALUE Selecter_toggleFocus(VALUE self);

VALUE Selecter_setBgPic(VALUE self, VALUE spr);
VALUE Selecter_setFocusBgPic(VALUE self, VALUE spr);

VALUE Selecter_setBgCol(VALUE self, VALUE col);
VALUE Selecter_setFocusBgCol(VALUE self, VALUE col);

VALUE setBorderCol(VALUE self, VALUE col);
VALUE setFocusBorderCol(VALUE self, VALUE col);

VALUE Selecter_setFont(VALUE self, VALUE f);
VALUE Selecter_setFocusFont(VALUE self, VALUE f);

VALUE Selecter_setTxtColor(VALUE self, VALUE col);
VALUE Selecter_setFocusTxtColor(VALUE self, VALUE col);

VALUE Selecter_addElem(VALUE self, VALUE spr, VALUE txt);
VALUE Selecter_draw(VALUE self);

void defineSelecter();

#endif
