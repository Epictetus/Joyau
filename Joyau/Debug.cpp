/*Copyright (C) 2009-2010 Verhetsel Kilian

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

#include "Debug.hpp"
#include "Drawable.hpp"

/*
  Inits the debugging screen.
*/
VALUE Debug_init(VALUE self) {
   pspDebugScreenInit();
   return Qnil;
}

/*
  call-seq: puts(obj, ...)

  Prints some string on the debugging screen. This function behavaes like
  Kernel#puts.
*/
VALUE Debug_puts(int argc, VALUE *argv, VALUE self) {
   if (argc == 0) {
      pspDebugScreenPrintf("\n");
      return Qnil;
   }

   for (int i = 0; i < argc; ++i) {
      VALUE line = rb_obj_as_string(argv[i]);

      const char *str = StringValuePtr(line);
      pspDebugScreenPrintf("%s", str);
      if (str[strlen(str) - 1] != '\n')
	 pspDebugScreenPrintf("\n");
   }
   
   return Qnil;
}

/*
  Prints some string on the debugging screen. This function behavaes like
  Kernel#print.
*/
VALUE Debug_print(int argc, VALUE *argv, VALUE self) {
   for (int i = 0; i < argc; ++i) {
      VALUE line = rb_obj_as_string(argv[i]);
      pspDebugScreenPrintf("%s", StringValuePtr(line));
   }
   
   return Qnil;
}

/*
  Clears the debugging screen.
*/
VALUE Debug_clear(VALUE self) {
   pspDebugScreenClear();
   return Qnil;
}

/*
  Enables background. By default, this is already done.
*/
VALUE Debug_enableBackground(VALUE self) {
   pspDebugScreenEnableBackColor(1);
   return Qtrue;
}

/*
  Disables background.
*/
VALUE Debug_disableBackground(VALUE self) {
   pspDebugScreenEnableBackColor(0);
   return Qfalse;
}

/*
  Enables or disables background.
 */
VALUE Debug_setBackground(VALUE self, VALUE val) {
   pspDebugScreenEnableBackColor(val == Qtrue);
   return val;
}

/*
  call-seq: x=(val)

  Sets the position of the place where text will be written.
*/
VALUE Debug_setX(VALUE self, VALUE x) {
   pspDebugScreenSetXY(FIX2INT(x), pspDebugScreenGetY());
   return x;
}

/*
  call-seq: y=(val)

  Sets the position of the place where text will be written.
*/
VALUE Debug_setY(VALUE self, VALUE y) {
   pspDebugScreenSetXY(pspDebugScreenGetX(), INT2FIX(y));
   return y;
}

/*
  call-seq: pos=(point)

  Sets the position of the place where text will be written, with a point.
*/
VALUE Debug_setPos(VALUE self, VALUE pos) {
   if (!rb_obj_is_kind_of(pos, getClass("Point")))
      rb_raise(rb_eTypeError, "Can't convert %s into Joyau::Point",
	       rb_class2name(pos));

   Point &ref = getRef<Point>(pos);
   pspDebugScreenSetXY(ref.x, ref.y);

   return Qnil;
}

/*
  Returns the cursor's abscissa.
 */
VALUE Debug_x(VALUE self) {
   return INT2FIX(pspDebugScreenGetX());
}

/*
  Returns the cursor's ordinate.
*/
VALUE Debug_y(VALUE self) {
   return INT2FIX(pspDebugScreenGetY());
}

/*
  Returns the cursor's position.
*/
VALUE Debug_pos(VALUE self) {
   Point p(pspDebugScreenGetX(), pspDebugScreenGetY());
   return createObject(getClass("Point"), p);
}

/*
  call-seq: bg_color=(col)

  Sets the text background's color.
*/
VALUE Debug_setBgColor(VALUE self, VALUE col) {
   pspDebugScreenSetBackColor(hash2col(col));
   return col;
}

/*
  call-seq: text_color=(col)
  
  Sets the text's color.
*/
VALUE Debug_setTextColor(VALUE self, VALUE col) {
   pspDebugScreenSetTextColor(hash2col(col));
   return col;
}

/*
  Document-class: Joyau::Debug

  The PSP's debugging sceen might be useful, if you want to trace
  the evolution of your program as you would do it with $stdout.
*/

void defineDebug() {
   VALUE mDebug = defModule("Debug");
   defModFunc(mDebug, "init", Debug_init, 0);
   
   defModFunc(mDebug, "puts", Debug_puts, -1);
   defModFunc(mDebug, "print", Debug_print, -1);
   
   defModFunc(mDebug, "clear", Debug_clear, 0);

   defModFunc(mDebug, "enable_background", Debug_enableBackground, 0);
   defModFunc(mDebug, "disable_background", Debug_disableBackground, 0);
   defModFunc(mDebug, "enable_background=", Debug_setBackground, 1);

   defModFunc(mDebug, "x=", Debug_setX, 1);
   defModFunc(mDebug, "y=", Debug_setY, 1);
   defModFunc(mDebug, "pos=", Debug_setPos, 1);

   defModFunc(mDebug, "x", Debug_x, 0);
   defModFunc(mDebug, "y", Debug_y, 0);
   defModFunc(mDebug, "pos", Debug_pos, 0);

   defModFunc(mDebug, "bg_color=", Debug_setBgColor, 1);
   defModFunc(mDebug, "text_color=", Debug_setTextColor, 1);
}
