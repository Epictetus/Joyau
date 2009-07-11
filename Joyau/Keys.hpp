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

#ifndef __KEYS__
#define __KEYS__

#include "StdInclude.hpp"
#include "Sprite.hpp"

class Cursor
{
public:
   void draw() { picture->Draw(); }

   void updatePos();
   void setSensibility(int s) { sensibility = s; }

   void move(int x, int y);
   void setPos(int x, int y);

   int getX() { return _x; }
   int getY() { return _y; }

   void setPicture(Sprite *spr) { picture = spr; }

   bool collide(Sprite *spr) { return picture->collide(spr); }
   bool isOn(int x, int y) { return picture->isOn(x, y); }
private:
   int sensibility;
   int _x, _y;

   Sprite *picture;
};

VALUE Keys_repeatInit(VALUE self, VALUE time);
VALUE Keys_repeatInterval(VALUE self, VALUE time);
VALUE checkKeys(VALUE self);

VALUE Cursor_draw(VALUE self);

VALUE Cursor_updatePos(VALUE self);
VALUE Cursor_setSensibility(VALUE self, VALUE s);

VALUE Cursor_move(VALUE self, VALUE x, VALUE y);
VALUE Cursor_setPos(VALUE self, VALUE x, VALUE y);

VALUE Cursor_getX(VALUE self);
VALUE Cursor_getY(VALUE self);

VALUE Cursor_setPicture(VALUE self, VALUE spr);

VALUE Cursor_collide(VALUE self, VALUE spr);
VALUE Cursor_isOn(VALUE self, VALUE x, VALUE y);

void defineKeys();

#endif
