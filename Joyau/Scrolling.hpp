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

/** @addtogroup Drawables **/
/*@{*/

/** 
 * @class Scrolling
 * A moving background.
 */
class Scrolling: public Drawable
{
public:
   Scrolling() { setClass("Scrolling"); }

   /** Sets the background.
    *  @param spr filename
    */
   void setSprite(char *spr);
   
   /** Sets the direction. Only four are supported.
    *  @param dir a constant in Sprite.
    */
   void setDir(int dir);

   /** Sets speed. The scrolling move of speed pixels per play call.
    *  @param s speed value.
    */
   void setSpeed(int s) { speed = s; }

   /** returns the speed **/
   int getSpeed() const { return speed; }

   /** returns the direction **/
   int getDir() const { return _dir; }

   /** Scrolls the scrolling. **/
   void play();

   void draw();

   void move(int x, int y);   
   void clearMove();

   int getW() const;
   int getH() const;
private:
   int _dir;
   Sprite bg[2];
   int speed;
};

/*@}*/

VALUE Scrolling_setSprite(VALUE self, VALUE spr);
VALUE Scrolling_setDir(VALUE self, VALUE dir);
VALUE Scrolling_setSpeed(VALUE self, VALUE s);

VALUE Scrolling_dir(VALUE self);
VALUE Scrolling_speed(VALUE self);

VALUE Scrolling_play(VALUE self);

void defineScrolling();

#endif
