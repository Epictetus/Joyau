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

#include "Drawable.hpp"

template<typename ContentType>
void GenericSelecter<ContentType>::draw()
{
   size_t count = 0;
   for (typename container_t::iterator i = content.begin(); 
	i != content.end(); ++i)
   {
      atLoop(*i, count);
      if (focused && count == index)
      	 forFocused(*i);
      
      i->draw();
      count++;
   }
}

template<typename ContentType>
void GenericSelecter<ContentType>::select(int dir)
{
   if (index + dir >= 0 && index + dir < size)
      index += dir;
}

template<typename ContentType>
void GenericSelecter<ContentType>::addItem(const content_t &obj)
{
   content.push_back(obj);
   size += 1;
   onAdd(content.back());
}

template<typename ContentType>
bool GenericSelecter<ContentType>::collide(Drawable &item)
{
   for (typename container_t::iterator i = content.begin(); 
	i != content.end(); ++i)
   {
      if (i->collide(item))
	 return true;
   }
   return false;
}

template<typename ContentType>
bool GenericSelecter<ContentType>::isOn(int x, int y)
{
   for (typename container_t::iterator i = content.begin(); 
	i != content.end(); ++i)
   {
      if (i->isOn(x, y))
	 return true;
   }
   return false;
}

template<typename ContentType>
int GenericSelecter<ContentType>::getW() const
{
   return _w;
}

template<typename ContentType>
int GenericSelecter<ContentType>::getH() const
{
   return _h;
}

template<typename ContentType>
void GenericSelecter<ContentType>::move(int x, int y)
{
   Drawable::move(x, y);
   for (typename container_t::iterator i = content.begin(); 
	i != content.end(); ++i)
      i->move(x, y);
}

template<typename ContentType>
void GenericSelecter<ContentType>::resize(int w, int h)
{
   _w = w;
   _h = h;
}

template<typename ContentType>
void HorizontalSelecter<ContentType>::onAdd(content_t &obj)
{
   obj.setPos(this->getX() + this->getW() * this->getSize(), this->getY());
}

template<typename ContentType>
void HorizontalSelecter<ContentType>::atLoop(content_t &obj, size_t pos)
{
   // getSize is always greater than 0 here, because we know
   // there is at least one object (obj).
   obj.setPos(this->getX() + (this->getW() / this->getSize()) * pos, 
	      this->getY());
   obj.resize(this->getW() / this->getSize(), this->getH());
}

template<typename ContentType>
void VerticalSelecter<ContentType>::onAdd(content_t &obj)
{
   obj.setPos(this->getX(), this->getY() + this->getH() * this->getSize());
}

template<typename ContentType>
void VerticalSelecter<ContentType>::atLoop(content_t &obj, size_t pos)
{
   obj.setPos(this->getX(), this->getY() + (this->getH() / this->getSize()) * 
	      pos);
   obj.resize(this->getW(), this->getH() / this->getSize());
}

template<typename T> VALUE GenericSelecter_select(VALUE self, VALUE dir)
{
   T &ref = getRef<T>(self);
   ref.select(FIX2INT(dir));

   return Qnil;
}

template<typename T> VALUE GenericSelecter_addItem(VALUE self, VALUE obj)
{
   T &ref = getRef<T>(self);
   typename T::content_t &arg = getRef<typename T::content_t>(obj);
   
   ref.addItem(arg);
   return Qnil;
}

template<typename T> VALUE GenericSelecter_resize(VALUE self, VALUE w, VALUE h)
{
   T &ref = getRef<T>(self);
   ref.resize(FIX2INT(w), FIX2INT(h));

   return Qnil;
}

template<typename T> VALUE GenericSelecter_selected(VALUE self)
{
   T &ref = getRef<T>(self);
   return ref.getSelected().toRuby();
}

template<typename T> VALUE GenericSelecter_item(VALUE self, VALUE index)
{
   T &ref = getRef<T>(self);
   return ref.getItem(FIX2INT(index)).toRuby();
}

template<typename T> VALUE GenericSelecter_index(VALUE self)
{
   T &ref = getRef<T>(self);
   return INT2FIX(ref.getIndex());
}

template<typename T> VALUE GenericSelecter_size(VALUE self)
{
   T &ref = getRef<T>(self);
   return INT2FIX(ref.getSize());
}

template<typename T> VALUE GenericSelecter_setFocus(VALUE self, VALUE val)
{
   T &ref = getRef<T>(self);
   ref.setFocus(val == Qtrue);
   
   return Qnil;
}

template<typename T> VALUE GenericSelecter_focus(VALUE self)
{
   T &ref = getRef<T>(self);
   return ref.getFocus() ? Qtrue : Qfalse;
}

template<typename T> VALUE defineGenericSelecter(const char *name)
{
   VALUE klass = defClass<T>(name, "Drawable");
   defTplMethod1(klass, "select", GenericSelecter_select<T>);
   defTplMethod1(klass, "<<", GenericSelecter_addItem<T>);
   defTplMethod2(klass, "resize", GenericSelecter_resize<T>);
   defTplMethod1(klass, "select", GenericSelecter_select<T>); 
   defTplMethod(klass, "selected", GenericSelecter_selected<T>);
   defTplMethod1(klass, "item", GenericSelecter_item<T>);
   defTplMethod(klass, "index", GenericSelecter_index<T>);
   defTplMethod(klass, "size", GenericSelecter_size<T>);
   defTplMethod1(klass, "focus=", GenericSelecter_setFocus<T>);
   defTplMethod(klass, "focus", GenericSelecter_focus<T>);

   defConst(klass, "PREV", INT2FIX(T::PREV));
   defConst(klass, "NEXT", INT2FIX(T::NEXT));

   return klass;
}

#endif
