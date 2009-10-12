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

#ifndef JOYAU_RUBYDRAWABLE
#define JOYAU_RUBYDRAWABLE

#include "Drawable.hpp"

/** @addtogroup Ruby **/
/*@{*/

/** 
 * @class RubyDrawable
 * When calling its method from the C++ code, their Ruby
 * version are called.
 */
class RubyDrawable: public Drawable
{
public:
   RubyDrawable(VALUE val = Qnil): self(val) {}

   void draw();

   Rect boundingRect() const;

   void move(int x, int y);

   bool collide(Drawable &item);
   bool isOn(int x, int y);
   
   int getX() const;
   int getY() const;

   void setX(int val);
   void setY(int val);

   int getW() const;
   int getH() const;

   void setPos(int x, int y);

   void clearMove();
   
   /** Sets the ruby object, whose methods are used. **/
   void setSelf(VALUE val) { self = val; }
   VALUE toRuby() { return self; }
private:
   VALUE self;
};

/*@}*/

#endif
