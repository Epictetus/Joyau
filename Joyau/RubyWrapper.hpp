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

#define RPROTO(proto) (VALUE(*)(...))&proto // A boring cast...

// Easier to understand
#define defFunc(func, proto, argc) \
    rb_define_global_function(func, RPROTO(proto), argc)
#define defMethod(klass, func, proto, argc) \
    rb_define_method(klass, func, RPROTO(proto), argc)

// generic function for class wrapping
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

template<typename T> VALUE defClass(const char *name, 
				    VALUE father = rb_cObject)
{
   VALUE ret = rb_define_class(name, father);
   VALUE (*func)(VALUE) = &wrap<T>;
   
   rb_define_singleton_method(ret, "new", (VALUE(*)(...))func, 0);
   return ret;
}

inline VALUE getClass(const char *name)
{
   return rb_const_get(rb_cObject, rb_intern(name));
}

// That code was really boring to write each time
inline OSL_COLOR hash2col(VALUE hash)
{
   int r = FIX2INT(rb_hash_aref(hash, rb_str_new2("r")));
   int g = FIX2INT(rb_hash_aref(hash, rb_str_new2("g")));
   int b = FIX2INT(rb_hash_aref(hash, rb_str_new2("b")));
   int a = FIX2INT(rb_hash_aref(hash, rb_str_new2("a")));

   return RGBA(r, g, b, a);
}

#endif