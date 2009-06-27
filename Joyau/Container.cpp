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

#include "Container.hpp"

Container::Container(Container *parent)
{
   _parent = parent;
}

void Container::playChild()
{
   for (list<Container*>::iterator i = contents.begin(); i != contents.end(); 
	++i)
   {
      if (*i != NULL) // Ruby's GC could delete them.
	 (*i)->play();
   }
}

void Container::drawChild()
{
   for (list<Container*>::iterator i = contents.begin(); i != contents.end(); 
	++i)
   {
      if (*i != NULL)
    	 (*i)->draw();
   }
}

void Container::move(int x, int y)
{
   _x += x;
   _y += y;
}

void Container::setPos(int x, int y)
{
   _x = x;
   _y = y;
}

void Container::addContent(Container *item)
{
   item->changeParent(this);
   contents.push_back(item);
}
