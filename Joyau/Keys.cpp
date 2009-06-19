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

#include "Keys.hpp"

VALUE checkKeys(VALUE self)
{
   oslReadKeys();
   VALUE keys = rb_gv_get("$keys");

   if (osl_pad.held.select)
      rb_hash_aset(keys, rb_str_new2("select"), Qtrue);
   else
      rb_hash_aset(keys, rb_str_new2("select"), Qfalse);

   if (osl_pad.held.start)
      rb_hash_aset(keys, rb_str_new2("start"), Qtrue);
   else
      rb_hash_aset(keys, rb_str_new2("start"), Qfalse);

   if (osl_pad.held.up)
      rb_hash_aset(keys, rb_str_new2("up"), Qtrue);
   else
      rb_hash_aset(keys, rb_str_new2("up"), Qfalse);

   if (osl_pad.held.down)
      rb_hash_aset(keys, rb_str_new2("down"), Qtrue);
   else
      rb_hash_aset(keys, rb_str_new2("down"), Qfalse);

   if (osl_pad.held.left)
      rb_hash_aset(keys, rb_str_new2("left"), Qtrue);
   else
      rb_hash_aset(keys, rb_str_new2("left"), Qfalse);
   
   if (osl_pad.held.right)
      rb_hash_aset(keys, rb_str_new2("right"), Qtrue);
   else
      rb_hash_aset(keys, rb_str_new2("right"), Qfalse);

   if (osl_pad.held.L)
      rb_hash_aset(keys, rb_str_new2("L"), Qtrue);
   else
      rb_hash_aset(keys, rb_str_new2("L"), Qfalse);

   if (osl_pad.held.R)
      rb_hash_aset(keys, rb_str_new2("R"), Qtrue);
   else
      rb_hash_aset(keys, rb_str_new2("R"), Qfalse);

   if (osl_pad.held.circle)
      rb_hash_aset(keys, rb_str_new2("circle"), Qtrue);
   else
      rb_hash_aset(keys, rb_str_new2("circle"), Qfalse);

   if (osl_pad.held.triangle)
      rb_hash_aset(keys, rb_str_new2("triangle"), Qtrue);
   else
      rb_hash_aset(keys, rb_str_new2("triangle"), Qfalse);

   if (osl_pad.held.cross)
      rb_hash_aset(keys, rb_str_new2("cross"), Qtrue);
   else
      rb_hash_aset(keys, rb_str_new2("cross"), Qfalse);

   if (osl_pad.held.square)
      rb_hash_aset(keys, rb_str_new2("square"), Qtrue);
   else
      rb_hash_aset(keys, rb_str_new2("square"), Qfalse);

   if (osl_pad.held.hold)
      rb_hash_aset(keys, rb_str_new2("hold"), Qtrue);
   else
      rb_hash_aset(keys, rb_str_new2("hold"), Qfalse);
   int analogX = osl_pad.analogX;
   int analogY = osl_pad.analogY;

   rb_hash_aset(keys, rb_str_new2("analogX"), INT2FIX(analogX));
   rb_hash_aset(keys, rb_str_new2("analogY"), INT2FIX(analogY));

   rb_gv_set("$keys", keys);
   
   return Qnil;
}
void defineKeys()
{
   VALUE keys = rb_hash_new();
   rb_gv_set("$keys", keys);
   
   rb_define_global_function("readKeys", (VALUE(*)(...))&checkKeys, 0);
}
