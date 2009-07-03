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

#ifndef __JOYAU_WRAPPER__
#define __JOYAU_WRAPPER__

#include "StdInclude.hpp"

#define RPROTO(proto) (VALUE(*)(...))&proto

template<typename T> void wrapped_free(void *info)
{
   delete (T*)info;
}

template<typename T> VALUE wrap(VALUE info)
{
   T *ptr = new T;
   VALUE tdata = Data_Wrap_Struct(info, 0, wrapped_free<T>, ptr);
   return tdata;
}

template<typename T> T *getPtr(VALUE val)
{
   T *ptr;
   Data_Get_Struct(val, T, ptr);
   return ptr;
}

template<typename T> VALUE defClass(const char *name)
{
   VALUE ret = rb_define_class(name, rb_cObject);
   VALUE (*func)(VALUE) = &wrap<T>;
   
   rb_define_singleton_method(ret, "new", (VALUE(*)(...))func, 0);
   return ret;
}

// That code was really boring to write
OSL_COLOR hash2col(VALUE hash);

#endif
