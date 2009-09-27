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

#ifndef JOYAU_FONT
#define JOYAU_FONT

#include "Drawable.hpp"

class IntraText: public Drawable
{
public:
   IntraText():
      font(NULL)
   { setClass("IntraText"); }

   ~IntraText();

   void setText(const std::string &val) { txt = val; }
   std::string getText() const { return txt; }

   void load(const std::string &name, int options);
   void setEncoding(int options);

   int getW() const;
   
   void activate();

   void setStyle(int size, int color, int shadowColor, int option);

   void setAltFont(IntraText &val);

   void draw();
private:
   intraFont *font;
   std::string txt;
};

VALUE Intrafont_init(VALUE self);
VALUE Intrafont_stop(VALUE self);

VALUE IntraText_setText(VALUE self, VALUE txt);
VALUE IntraText_text(VALUE self);

VALUE IntraText_load(VALUE self, VALUE name, VALUE options);

VALUE IntraText_activate(VALUE self);

VALUE IntraText_setStyle(VALUE self, VALUE size, VALUE color, VALUE shadow, 
			 VALUE options);
VALUE IntraText_setEncoding(VALUE self, VALUE options);

VALUE IntraText_setAltFont(VALUE self, VALUE obj);

void defineIntrafont();

#endif
