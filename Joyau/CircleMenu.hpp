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

#ifndef __JOYAU_CIRCLE_MENU__
#define __JOYAU_CIRCLE_MENU__

#include "StdInclude.hpp"

class CircleMenu
{
public:
   CircleMenu();
   virtual ~CircleMenu();
   
   void load(char *str, int nbIcons, int dist, int w, int h, int x, int y,
	     int iconW, int iconH);
   
   void changeSelection(int dir);
   int getIndex();

   void draw();

   void setAlpha(int alpha);

   void move(int x, int y);
   void setPos(int x, int y);

   void setDist(int dist);
   void setAngle(int angle);
private:
   OSL_CIRCLE_MENU *_menu;
};

VALUE CircleMenu_load(VALUE self, VALUE str, VALUE nbIcons, VALUE dist, VALUE w, 
		      VALUE h, VALUE x, VALUE y, VALUE iconW, VALUE iconH);

VALUE CircleMenu_changeSelection(VALUE self, VALUE dir);
VALUE CircleMenu_getIndex(VALUE self);

VALUE CircleMenu_draw(VALUE self);

VALUE CircleMenu_setAlpha(VALUE self, VALUE alpha);

VALUE CircleMenu_move(VALUE self, VALUE x, VALUE y);
VALUE CircleMenu_setPos(VALUE self, VALUE x, VALUE y);

VALUE CircleMenu_setDist(VALUE self, VALUE dist);
VALUE CircleMenu_setAngle(VALUE self, VALUE angle);

void defineCircleMenu();

#endif