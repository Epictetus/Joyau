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

#include "Graphics.hpp"

VALUE Graphics_mayPlay(VALUE self)
{
   if (!osl_quit)
      return Qtrue;
   return Qfalse;
}

VALUE Graphics_startDraw(VALUE self)
{
   oslStartDrawing();
   return Qnil;
}

VALUE Graphics_endDraw(VALUE self)
{
   oslEndDrawing();
   return Qnil;
}

VALUE Graphics_sync(VALUE self)
{
   oslSyncFrame();
   return Qnil;
}

VALUE Graphics_init(VALUE self)
{
   oslInitGfx(OSL_PF_8888, 1);
   
   
   return Qnil;
}

VALUE Graphics_stop(VALUE self)
{
   oslEndGfx();

   return Qnil;
}

VALUE lib_start(VALUE self)
{
   oslInit(OSL_IF_USEOWNCALLBACKS);
   return Qnil;
}

VALUE lib_stop(VALUE self)
{
   oslQuit();

   return Qnil;
}

VALUE Graphics_drawLine(VALUE self, VALUE x1, VALUE y1, VALUE x2, 
			VALUE y2, VALUE color)
{
   int r = FIX2INT(rb_hash_aref(color, rb_str_new2("r")));
   int g = FIX2INT(rb_hash_aref(color, rb_str_new2("g")));
   int b = FIX2INT(rb_hash_aref(color, rb_str_new2("b")));
   int a = FIX2INT(rb_hash_aref(color, rb_str_new2("a")));

   OSL_COLOR c = RGBA(r, g, b, a);
   oslDrawLine(x1, y1, x2, y2, c);
   return Qnil;
}

VALUE Graphics_drawRect(VALUE self, VALUE x1, VALUE y1, VALUE x2, 
			VALUE y2, VALUE color)
{
   int r = FIX2INT(rb_hash_aref(color, rb_str_new2("r")));
   int g = FIX2INT(rb_hash_aref(color, rb_str_new2("g")));
   int b = FIX2INT(rb_hash_aref(color, rb_str_new2("b")));
   int a = FIX2INT(rb_hash_aref(color, rb_str_new2("a")));

   OSL_COLOR c = RGBA(r, g, b, a);
   oslDrawRect(x1, y1, x2, y2, c);

   return Qnil;
}

VALUE Graphics_drawFillRect(VALUE self, VALUE x1, VALUE y1, VALUE x2, 
			    VALUE y2, VALUE color)
{
   int r = FIX2INT(rb_hash_aref(color, rb_str_new2("r")));
   int g = FIX2INT(rb_hash_aref(color, rb_str_new2("g")));
   int b = FIX2INT(rb_hash_aref(color, rb_str_new2("b")));
   int a = FIX2INT(rb_hash_aref(color, rb_str_new2("a")));

   OSL_COLOR c = RGBA(r, g, b, a);
   oslDrawFillRect(x1, y1, x2, y2, c);

   return Qnil;
}

VALUE setTextFont(VALUE self, VALUE fontname)
{
   Manager *manager = Manager::getInstance();
   oslSetFont(manager->getFont(StringValuePtr(fontname)));
   return Qnil;
}

VALUE getTextSize(VALUE self, VALUE text)
{
   int val = oslGetStringWidth(StringValuePtr(text));
   return INT2FIX(val);
}

VALUE setTextBackground(VALUE self, VALUE color)
{
   int r = FIX2INT(rb_hash_aref(color, rb_str_new2("r")));
   int g = FIX2INT(rb_hash_aref(color, rb_str_new2("g")));
   int b = FIX2INT(rb_hash_aref(color, rb_str_new2("b")));
   int a = FIX2INT(rb_hash_aref(color, rb_str_new2("a")));

   OSL_COLOR c = RGBA(r, g, b, a);
   oslSetBkColor(c);

   return Qnil;
}

VALUE setTextColor(VALUE self, VALUE color)
{
   int r = FIX2INT(rb_hash_aref(color, rb_str_new2("r")));
   int g = FIX2INT(rb_hash_aref(color, rb_str_new2("g")));
   int b = FIX2INT(rb_hash_aref(color, rb_str_new2("b")));
   int a = FIX2INT(rb_hash_aref(color, rb_str_new2("a")));

   OSL_COLOR c = RGBA(r, g, b, a);
   oslSetTextColor(c);
   return Qnil;
}

VALUE Graphics_clear(VALUE self)
{
   oslClearScreen(RGB(0, 0, 0));
   return Qnil;
}

VALUE drawText(VALUE self, VALUE x, VALUE y, VALUE text)
{
   oslDrawString(FIX2INT(x), FIX2INT(y), StringValuePtr(text));
   return Qnil;
}

void defineGraphics()
{
   rb_define_global_function("mayPlay", (VALUE(*)(...))&Graphics_mayPlay, 0);
   rb_define_global_function("startDraw", 
			     (VALUE(*)(...))&Graphics_startDraw, 0);
   rb_define_global_function("endDraw", (VALUE(*)(...))&Graphics_endDraw, 0);
   rb_define_global_function("sync", (VALUE(*)(...))&Graphics_sync, 0);

   rb_define_global_function("drawLine", 
			     (VALUE(*)(...))&Graphics_drawLine, 5);
   rb_define_global_function("drawRect", 
			     (VALUE(*)(...))&Graphics_drawRect, 5);
   rb_define_global_function("drawFillRect", 
			     (VALUE(*)(...))&Graphics_drawFillRect, 5);
   
   rb_define_global_function("setFont", (VALUE(*)(...))&setTextFont, 1);
   rb_define_global_function("getLength", (VALUE(*)(...))getTextSize, 1);
   rb_define_global_function("setTextBackground", 
			     (VALUE(*)(...))&setTextBackground, 1);
   rb_define_global_function("setTextColor", 
			     (VALUE(*)(...))&setTextColor, 1);
   rb_define_global_function("drawText", (VALUE(*)(...))&drawText, 3);
   rb_define_global_function("clearScreen", (VALUE(*)(...))&Graphics_clear, 0);
   rb_define_global_function("initGfx", (VALUE(*)(...))&Graphics_init, 0);
   rb_define_global_function("initLib", (VALUE(*)(...))&lib_start, 0);
   rb_define_global_function("stopLib", (VALUE(*)(...))&lib_stop, 0);
   rb_define_global_function("stopGfx", (VALUE(*)(...))&Graphics_stop, 0);
}
