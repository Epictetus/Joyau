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
#include "Buffer.hpp"

/*
  Returns true untill the user has exited through the home button.
*/
VALUE Graphics_mayPlay(VALUE self)
{
   return osl_quit ? Qfalse : Qtrue;
}

/*
  Allows to draw something on the screen.
*/
VALUE Graphics_startDraw(VALUE self)
{
   oslStartDrawing();
   return Qnil;
}

/*
  Call this when you've finished to draw on the screen.
*/
VALUE Graphics_endDraw(VALUE self)
{
   oslEndDrawing();
   return Qnil;
}

/*
  Updates the screen. If false is returned, you shouldn't draw anything during
  the next loop.
*/
VALUE Graphics_sync(VALUE self)
{
   oslEndFrame();
   if (oslSyncFrame())
       return Qtrue;
   return Qfalse;
}

/*
  call-seq: frameskip(min, max)

  Sets how many frames can be skipped. By default, your program is running
  at 60 FPS.
*/
VALUE Graphics_frameskip(VALUE self, VALUE min, VALUE max)
{
   oslSetFrameskip(FIX2INT(min));
   oslSetMaxFrameskip(FIX2INT(max));
   return Qnil;
}

/*
  Inits the graphics module.
*/
VALUE Graphics_init(VALUE self)
{
   oslInitGfx(OSL_PF_8888, 1);
   Buffer::updateScreen();
   return Qnil;
}

/*
  Stops the Graphics module.
 */
VALUE Graphics_stop(VALUE self)
{
   oslEndGfx();
   return Qnil;
}

/* 
   Inits Joyau's library. Use this before graphics module.
*/
VALUE lib_start(VALUE self)
{
   oslInit(OSL_IF_USEOWNCALLBACKS);
   return Qnil;
}

/*
  Stops Joyau's library.
*/
VALUE lib_stop(VALUE self)
{
   oslQuit();
   return Qnil;
}

/*
  call-seq: color(r, g, b, a = 255)

  Creates a new color hash.
*/
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

/*
  call-seq: drawLine(x1, y1, x2, y2, color)

  Draws a line on the screen.
*/
VALUE Graphics_drawLine(VALUE self, VALUE x1, VALUE y1, VALUE x2, 
			VALUE y2, VALUE color)
{
   OSL_COLOR c = hash2col(color);
   oslDrawLine(FIX2INT(x1), FIX2INT(y1), FIX2INT(x2), FIX2INT(y2), c);
   return Qnil;
}

/*
  call-seq: drawRect(x1, y1, x2, y2, color)

  Draws a rect on the screen.
*/
VALUE Graphics_drawRect(VALUE self, VALUE x1, VALUE y1, VALUE x2, 
			VALUE y2, VALUE color)
{
   OSL_COLOR c = hash2col(color);
   oslDrawRect(FIX2INT(x1), FIX2INT(y1), FIX2INT(x2), FIX2INT(y2), c);

   return Qnil;
}

/*
  call-seq: drawTriangle(x1, y1, x2, y2, x3, y3, col1, col2, col3)

  Draws a triangle on the screen.
*/
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

/*
  call-seq: drawFillRect(x1, y1, x2, y2, color)

  Draws a filled rect on the screen.
*/
VALUE Graphics_drawFillRect(VALUE self, VALUE x1, VALUE y1, VALUE x2, 
			    VALUE y2, VALUE color)
{
   OSL_COLOR c = hash2col(color);
   oslDrawFillRect(FIX2INT(x1), FIX2INT(y1), FIX2INT(x2), FIX2INT(y2), c);

   return Qnil;
}

/*
  call-seq: drawCircle(x, y, radius, color)
  
  Draws a circle on the screen.
*/
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

/*
  call-seq: drawFillCircle(x, y, radius, color)
  
  Draws a filled circle on the screen.
*/
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

/*
  call-seq: screenshot(pic)

  Takes a screenshot.
*/
VALUE Graphics_screenshot(VALUE self, VALUE pic)
{
   char *filename = StringValuePtr(pic);
   
   oslWriteImageFile(OSL_SECONDARY_BUFFER, filename, 0);
   return Qnil;
}

/*
  Fades the screen.
*/
VALUE Graphics_fade(VALUE self)
{
   oslFadeInEffect();
   return Qnil;
}

/*
  call-seq: setFont(fontname)

  Changes the actual font.
 */
VALUE setTextFont(VALUE self, VALUE fontname)
{
   Manager &manager = Manager::getInstance();
   oslSetFont(manager.getFont(StringValuePtr(fontname)));
   return Qnil;
}

/*
  call-seq: getLength
  Returns the width of a text if it were drawn on the screen with the actual
  fonts.
 */
VALUE getTextSize(VALUE self, VALUE text)
{
   int val = oslGetStringWidth(StringValuePtr(text));
   return INT2FIX(val);
}

/*
  call-seq: setTextBackground(color)

  Sets the color in which the text's background is drawn.
 */
VALUE setTextBackground(VALUE self, VALUE color)
{
   OSL_COLOR c = hash2col(color);
   oslSetBkColor(c);

   return Qnil;
}

/*
  call-seq: setTextColor(color)

  Sets the color in which the text's color.
*/
VALUE setTextColor(VALUE self, VALUE color)
{
   OSL_COLOR c = hash2col(color);
   oslSetTextColor(c);
   return Qnil;
}

/*
  Clears the screen.
*/
VALUE Graphics_clear(VALUE self)
{
   oslCls();
   return Qnil;
}

/*
  call-seq: drawText(x, y, text)

  Draws a text on the screen.
*/
VALUE drawText(VALUE self, VALUE x, VALUE y, VALUE text)
{
   oslDrawString(FIX2INT(x), FIX2INT(y), StringValuePtr(text));
   return Qnil;
}

/*
  call-seq: drawScripted(x, y, txt)

  Draws a scripted (i.e. which may use \n, ...) text on the screen.
*/
VALUE drawScripted(VALUE self, VALUE x, VALUE y, VALUE text)
{
   oslScriptText(FIX2INT(x), FIX2INT(y), StringValuePtr(text));
   return Qnil;
}

/*
  call-seq: drawStirringText(x, y, text)

  Draws a stirring text on the screen.
*/
VALUE drawStirringText(VALUE self, VALUE x, VALUE y, VALUE text)
{
   oslPrintStirringString(FIX2INT(x), FIX2INT(y), StringValuePtr(text));
   return Qnil;
}

/*
  Enables auto swizzling for pictures.
*/
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
}
