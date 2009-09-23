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

#ifndef JOYAU_GENERIC_SELECTER2
#define JOYAU_GENERIC_SELECTER2

template<typename ContentType>
void GenericSelecter<ContentType>::draw()
{
   size_t count = 0;
   for (container_t::iterator i = content.begin(); i != content.end(); ++i)
   {
      atLoop(*i);
      if (focused && count == index)
	 forFocused(*i);
      i->draw();
      count++;
   }
}

template<typename ContentType>
void GenericSelecter<ContentType>::select(int dir)
{
   if (index + dir > 0 && index + dir < size)
      index += dir;
}

template<typename ContentType>
void GenericSelecter<ContentType>::addItem(const content_t &obj)
{
   content.push_back(obj);
   onAdd(content.back());
}

template<typename ContentType>
bool GenericSelecter<ContentType>::collide(Drawable &item)
{
   for (container_t::iterator i = content.begin(); i != content.end(); ++i)
   {
      if (i->collide(item))
	 return true;
   }
   return false;
}

template<typename ContentType>
bool GenericSelecter<ContentType>::isOn(int x, int y)
{
   for (container_t::iterator i = content.begin(); i != content.end(); ++i)
   {
      if (i->isOn(x, y))
	 return true;
   }
   return false;
}

template<typename ContentType>
int GenericSelecter<ContentType>::getW() const
{
   return content.size() * _w;
}

template<typename ContentType>
int GenericSelecter<ContentType>::getH() const
{
   return content.size() * _h;
}

template<typename ContentType>
void GenericSelecter<ContentType>::move(int x, int y)
{
   Drawable::move(x, y);
   for (container_t::iterator i = content.begin(); i != content.end(); ++i)
      i->move(x, y);
}

template<typename ContentType>
void GenericSelecter<ContentType>::resize(int w, int h)
{
   _w = w;
   _h = h;
   for (container_t::iterator i = content.begin(); i != content.end(); ++i)
      i->resize(w, h);
}

template<typename ContentType>
void HorizontalSelecter<ContentType>::onAdd(content_t &obj)
{
   obj.move(getX() + getW() - _w, getY());
}

template<typename ContentType>
void VerticalSelecter<ContentType>::onAdd(content_t &obj)
{
   obj.move(getX(), getY() + getH() - _h);
}

#endif
