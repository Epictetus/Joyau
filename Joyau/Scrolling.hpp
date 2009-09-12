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

#ifndef JOYAU_SCROLLING
#define JOYAU_SCROLLING

#include "Sprite.hpp"

// Just a background with scrolling.
class Scrolling: public Drawable
{
public:
   Scrolling() { setClass("Scrolling"); }

   void setSprite(char *spr);
   
   void setDir(int dir);
   void setSpeed(int s) { speed = s; }

   int getSpeed() const { return speed; }
   int getDir() const { return _dir; }

   void play();
   void draw();

   void move(int x, int y);
   void setPos(int x, int y);

   void setX(int x);
   void setY(int y);

   void clearMove();

   int getW() const;
   int getH() const;
private:
   int _dir;
   Sprite bg[2];
   int speed;
};

VALUE Scrolling_setSprite(VALUE self, VALUE spr);
VALUE Scrolling_setDir(VALUE self, VALUE dir);
VALUE Scrolling_setSpeed(VALUE self, VALUE s);

VALUE Scrolling_dir(VALUE self);
VALUE Scrolling_speed(VALUE self);

VALUE Scrolling_play(VALUE self);

void defineScrolling();

#endif
