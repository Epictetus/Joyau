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

#ifndef __RUBY_CONTAINER__
#define __RUBY_CONTAINER__

#include "Container.hpp"

//This class is just like Container, but it's easyer to wrap it.
class RubyContainer: public Container
{
public:
   RubyContainer();

   void setSelf(VALUE val); // used at the object creation

   void play();
   void draw();
protected:
   VALUE self;
};

// Now, let's define the functions used when we wrap it.

VALUE wrapContainer(VALUE info);
void containerFree(void *info);

// We've to set the self value ! ( It'd have been boring for the Ruby 
// developer if we had used another method, no ? )
VALUE Container_initialize(VALUE self);

VALUE Container_addContent(VALUE self, VALUE item);
VALUE Container_setParent(VALUE self, VALUE parent);

VALUE Container_getX(VALUE self);
VALUE Container_getY(VALUE self);
VALUE Container_getW(VALUE self);
VALUE Container_getH(VALUE self);

VALUE Container_move(VALUE self, VALUE x, VALUE y);
VALUE Container_setPos(VALUE self, VALUE x, VALUE y);

VALUE Container_playChild(VALUE self);
VALUE Container_drawChild(VALUE self);

void defineContainer();

#endif
