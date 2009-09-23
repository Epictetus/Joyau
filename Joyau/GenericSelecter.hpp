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

#ifndef JOYAU_GENERIC_SELECTER
#define JOYAU_GENERIC_SELECTER

#include "Drawable.hpp"

template<typename ContentType>
class GenericSelecter:public Drawable
{
public:
   typedef std::vector<ContentType> container_t;
   typedef ContentType content_t;

   enum DIRECTION { NEXT = 1, PREV = -1 };
   
   void draw();

   void select(int dir);

   void addItem(const content_t &obj);

   bool collide(Drawable &item);
   bool isOn(int x, int y);

   int getW() const;
   int getH() const;

   void move(int x, int y);
   void resize(int w, int h);

   content_t &getSelected() { return content[index]; }
   content_t &getItem(size_t ind) { return content[ind]; }

   size_t getSize() const { return size; }
   size_t getIndex() const { return index; }

   void setFocus(bool val) { focused = val; }

   virtual void onAdd(content_t &obj) = 0;
   virtual void forFocused(content_t &obj) = 0;
   virtual void atLoop(content_t &obj) = 0;
protected:
   bool focused;

   size_t index;
   size_t size;

   container_t content;

   int _w, _h;
};

template<typename ContentType>
class HorizontalSelecter: public GenericSelecter<ContentType>
{
public:
   void onAdd(content_t &obj);
}

template<typename ContentType>
class VerticalSelecter: public GenericSelecter<ContentType>
{
public:
   void onAdd(content_t &obj);
}

#include "GenericSelecter.tpp"

#endif
