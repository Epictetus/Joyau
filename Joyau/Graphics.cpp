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
   return osl_quit ? Qfalse : Qtrue;
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
   
   oslEndFrame();
   if (oslSyncFrame())
       return Qtrue;
   return Qfalse;
}

VALUE Graphics_frameskip(VALUE self, VALUE min, VALUE max)
{
   oslSetFrameskip(FIX2INT(min));
   oslSetMaxFrameskip(FIX2INT(max));
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

VALUE Graphics_color(int argc, VALUE *argv, VALUE self)
{
   if (argc >= 3)
   {
      VALUE hash = rb_hash_new();
      rb_hash_aset(hash, rb_str_new2("r"), argv[0]);
      rb_hash_aset(hash, rb_str_new2("g"), argv[1]);
      rb_hash_aset(hash, rb_str_new2("b"), argv[2]);
      if (argc < 4)
	 rb_hash_aset(hash, rb_str_new2("a"), INT2FIX(255));
      else
	 rb_hash_aset(hash, rb_str_new2("a"), argv[3]);
      return hash;
   }
   else
      return Qfalse; // not enough arguments.
}

VALUE Graphics_drawLine(VALUE self, VALUE x1, VALUE y1, VALUE x2, 
			VALUE y2, VALUE color)
{
   OSL_COLOR c = hash2col(color);
   oslDrawLine(FIX2INT(x1), FIX2INT(y1), FIX2INT(x2), FIX2INT(y2), c);
   return Qnil;
}

VALUE Graphics_drawRect(VALUE self, VALUE x1, VALUE y1, VALUE x2, 
			VALUE y2, VALUE color)
{
   OSL_COLOR c = hash2col(color);
   oslDrawRect(FIX2INT(x1), FIX2INT(y1), FIX2INT(x2), FIX2INT(y2), c);

   return Qnil;
}

VALUE Graphics_drawTriangle(VALUE self, VALUE x1, VALUE y1, VALUE x2, VALUE y2, 
			    VALUE x3, VALUE y3, VALUE col1, VALUE col2, 
			    VALUE col3)
{
   int _x1 = FIX2INT(x1);
   int _x2 = FIX2INT(x2);
   int _x3 = FIX2INT(x3);
   
   int _y1 = FIX2INT(y1);
   int _y2 = FIX2INT(y2);
   int _y3 = FIX2INT(y3);

   OSL_COLOR _col1 = hash2col(col1);
   OSL_COLOR _col2 = hash2col(col2);
   OSL_COLOR _col3 = hash2col(col3);

   oslDrawGradientTriangle(_x1, _y1, _x2, _y2, _x3, _y3, _col1, _col2, _col3);
   return Qnil;
}

VALUE Graphics_drawFillRect(VALUE self, VALUE x1, VALUE y1, VALUE x2, 
			    VALUE y2, VALUE color)
{
   OSL_COLOR c = hash2col(color);
   oslDrawFillRect(FIX2INT(x1), FIX2INT(y1), FIX2INT(x2), FIX2INT(y2), c);

   return Qnil;
}

VALUE Graphics_drawCircle(VALUE self, VALUE x, VALUE y, VALUE radius, 
			  VALUE col)
{
   OSL_COLOR c = hash2col(col);
   int _x = FIX2INT(x);
   int _y = FIX2INT(y);
   int _radius = FIX2INT(radius);

   oslDrawCircle(_x, _y, _radius, c);   
   return Qnil;
}

VALUE Graphics_drawFillCircle(VALUE self, VALUE x, VALUE y, VALUE radius, 
			  VALUE col)
{
   OSL_COLOR c = hash2col(col);
   int _x = FIX2INT(x);
   int _y = FIX2INT(y);
   int _radius = FIX2INT(radius);

   oslDrawFillCircle(_x, _y, _radius, c);   
   return Qnil;
}

VALUE Graphics_screenshot(VALUE self, VALUE pic)
{
   char *filename = StringValuePtr(pic);
   
   oslWriteImageFile(OSL_SECONDARY_BUFFER, filename, OSL_WRI_ALPHA);
   return Qnil;
}

VALUE Graphics_fade(VALUE self)
{
   oslFadeInEffect();
   return Qnil;
}

VALUE setTextFont(VALUE self, VALUE fontname)
{
   Manager &manager = Manager::getInstance();
   oslSetFont(manager.getFont(StringValuePtr(fontname)));
   return Qnil;
}

VALUE getTextSize(VALUE self, VALUE text)
{
   int val = oslGetStringWidth(StringValuePtr(text));
   return INT2FIX(val);
}

VALUE setTextBackground(VALUE self, VALUE color)
{
   OSL_COLOR c = hash2col(color);
   oslSetBkColor(c);

   return Qnil;
}

VALUE setTextColor(VALUE self, VALUE color)
{
   OSL_COLOR c = hash2col(color);
   oslSetTextColor(c);
   return Qnil;
}

VALUE Graphics_clear(VALUE self)
{
   oslCls();
   return Qnil;
}

VALUE drawText(VALUE self, VALUE x, VALUE y, VALUE text)
{
   oslDrawString(FIX2INT(x), FIX2INT(y), StringValuePtr(text));
   return Qnil;
}

VALUE drawScripted(VALUE self, VALUE x, VALUE y, VALUE text)
{
   oslScriptText(FIX2INT(x), FIX2INT(y), StringValuePtr(text));
   return Qnil;
}

VALUE drawStirringText(VALUE self, VALUE x, VALUE y, VALUE text)
{
   oslPrintStirringString(FIX2INT(x), FIX2INT(y), StringValuePtr(text));
   return Qnil;
}

VALUE setAutoSwizzle(VALUE self)
{
   oslSetImageAutoSwizzle(1);
   return Qnil;
}

void defineGraphics()
{
   VALUE joyau = JOYAU_MOD;
   defModFunc(joyau, "swizzle", setAutoSwizzle, 0);

   defModFunc(joyau, "mayPlay", Graphics_mayPlay, 0);

   defModFunc(joyau, "startDraw", Graphics_startDraw, 0);
   defModFunc(joyau, "endDraw", Graphics_endDraw, 0);
   defModFunc(joyau, "sync", Graphics_sync, 0);

   defModFunc(joyau, "frameskip", Graphics_frameskip, 2);

   defModFunc(joyau, "color", Graphics_color, -1);

   defModFunc(joyau, "drawLine", Graphics_drawLine, 5);
   defModFunc(joyau, "drawRect", Graphics_drawRect, 5);
   defModFunc(joyau, "drawFillRect", Graphics_drawFillRect, 5);
   defModFunc(joyau, "drawTriangle", Graphics_drawTriangle, 9);
   defModFunc(joyau, "drawCircle", Graphics_drawCircle, 4);
   defModFunc(joyau, "drawFillCircle", Graphics_drawFillCircle, 4);

   defModFunc(joyau, "screenshot", Graphics_screenshot, 1);
   defModFunc(joyau, "fade", Graphics_fade, 0);

   defModFunc(joyau, "setFont", setTextFont, 1);
   defModFunc(joyau, "getLength", getTextSize, 1);
   defModFunc(joyau, "setTextBackground", setTextBackground,1);
   defModFunc(joyau, "setTextColor", setTextColor, 1);
   defModFunc(joyau, "drawText", drawText, 3);
   defModFunc(joyau, "drawScripted", drawScripted, 3);
   defModFunc(joyau, "drawStirringText", drawStirringText, 3);

   defModFunc(joyau, "clearScreen", Graphics_clear, 0);

   defModFunc(joyau, "initGfx", Graphics_init, 0);
   defModFunc(joyau, "initLib", lib_start, 0);
   defModFunc(joyau, "stopLib", lib_stop, 0);
   defModFunc(joyau, "stopGfx", Graphics_stop, 0);

   defAlias(joyau, "mayPlay", "may_play?");
}
