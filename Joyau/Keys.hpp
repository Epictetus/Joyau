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

#ifndef JOYAU_KEYS
#define JOYAU_KEYS

#include "StdInclude.hpp"
#include "Sprite.hpp"

class Cursor: public Sprite
{
public:
   void updatePos();
   void setSensibility(int s) { sensibility = s; }
private:
   int sensibility;
};

VALUE Keys_repeatInit(VALUE self, VALUE time);
VALUE Keys_repeatInterval(VALUE self, VALUE time);
VALUE checkKeys(VALUE self);

VALUE Cursor_updatePos(VALUE self);
VALUE Cursor_setSensibility(VALUE self, VALUE s);

VALUE Joyau_gets(VALUE self);

void defineKeys();

#endif
