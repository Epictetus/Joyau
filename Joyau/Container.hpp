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

#ifndef __CONTAINER__
#define __CONTAINER__

#include "StdInclude.hpp"

class Container
{
public:
   Container(Container *parent);
   
   void changeParent(Container *parent) { _parent = parent; }
   
   virtual void play() { playChild(); }
   virtual void draw() { drawChild(); }

   void playChild();
   void drawChild();

   void move(int x, int y);
   void setPos(int x, int y);

   int getX() { return _x; }
   int getY() { return _y; }
   int getH() { return _h; }
   int getW() { return _w; }

   void addContent(Container *item);

protected:
   Container *_parent;
   list<Container*> contents; // Each item in the Container is... a container

   int _x, _y, _w, _h;

};

#endif
