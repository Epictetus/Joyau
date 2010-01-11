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

#include "Console.hpp"
#undef RED
#undef GREEN
#undef BLUE
#undef WHITE
#undef BLACK

#undef RGB
#undef RGBA

extern "C" {
#include "console/console2d.h"
#include "console/console.h"
}

/*
  call-seq: init(mode)

  Inits Console, either in FULL or LITE mode.
  Use FULL if you want to use the keyboard.
*/
VALUE rubyConsoleInit(VALUE self, VALUE mode) {
   consoleInit(FIX2INT(mode));
   return Qnil;
}

/*
  call-seq: exit

  Exits from the Console, with a fading effect.
*/
VALUE rubyConsoleExit(VALUE self) {
   consoleExit();
   return Qnil;
}

/*
  call-seq: switch_sfx

  Switches the scroll and fade effect on/off.
*/
VALUE rubyConsoleSwitchSFX(VALUE self) {
   return INT2FIX(consoleSwitchSFX());
}

/*
  call-seq: puts(str)

  Writes a line on the console.
*/
VALUE rubyConsolePuts(VALUE self, VALUE str) {
   consolePrintText(ADD, StringValuePtr(str));
   return Qnil;
}

/*
  call-seq: print(str) -> nil

  Adds text to the actual line.
*/
VALUE rubyConsolePrint(VALUE self, VALUE str) {
   consolePrintText(CAT, StringValuePtr(str));
   return Qnil;
}

/*
  call-seq: replace(str)

  Replaces the actual line.
*/
VALUE rubyConsoleReplace(VALUE self, VALUE str) {
   consolePrintText(REPLACE, StringValuePtr(str));
   return Qnil;
}

/*
  Returns a string typed by the user.
*/
VALUE rubyConsoleGets(VALUE self) {
   return rb_str_new2(consoleInputText(LETTERS));
}

/*
  Returns an integer typed by the user.
*/
VALUE rubyConsoleGetNum(VALUE self) {
   VALUE str = rb_str_new2(consoleInputText(NUMBERS));
   return rb_funcall(str, getFunc("to_i"), 0);
}

/*
  Clears the console.
*/
VALUE rubyConsoleClear(VALUE self) {
   consoleFreeTextArray();
   return Qnil;
}

/*
  call-seq: is_button?(button, state) -> true or false

  Returns whether the button <em>button</em> (either a constant in +Joyau::Pad+
  or ANY_KEY) is at <em>state</em>, where +state+ is a constant defined in 
  +Console+.
*/
VALUE rubyConsoleIsButton(VALUE self, VALUE button, VALUE state) {
   return consoleButton(FIX2INT(button), FIX2INT(state)) ? Qtrue : Qfalse;
}

/*
  call-seq: wait_key(button, str)

  Waits that the user presses button, and then prints something on the screen.
*/
VALUE rubyConsoleWaitKey(VALUE self, VALUE button, VALUE str) {
   consoleWaitButton(FIX2INT(button), StringValuePtr(str));
   return Qnil;
}

/*
  call-seq: text_color=(color)

  Sets the Console's text color.
*/
VALUE rubyConsoleSetTextColor(VALUE self, VALUE color) {
   consoleSetTextColor(hash2col(color));
   return Qnil;
}

/*
  Switches the fixed state on/off.
*/
VALUE rubyConsoleSwitchFixedMode(VALUE self) {
   consoleSwitchFixedMode();
   return Qnil;
}

/*
  Inits the Console2d library.
 */
VALUE rubyConsole2dInit(VALUE self) {
   console2dInit();
   return Qnil;
}

/*
  Exits the Console2d library, with a fading effect.
*/
VALUE rubyConsole2dExit(VALUE self) {
   console2dExit();
   return Qnil;
}

/*
  call-seq: print(x, y, mode, txt)

  Prints some text, either alligned on the left or on the right (this is set
  with mode).
*/
VALUE rubyConsole2dPrint(VALUE self, VALUE x, VALUE y, VALUE mode, 
			 VALUE txt) {
   console2dPrint(FIX2INT(x), FIX2INT(y), FIX2INT(mode), 
		  StringValuePtr(txt));
   return Qnil;
}

/*
  call-seq: fill_line(mode, pos, adv, txt)

  Fills a line, either horizontally or vertically (depending on the given mode),
  starting from the given pos, with the +adv+th character, drawing a text.
*/
VALUE rubyConsole2dFillLine(VALUE self, VALUE mode, VALUE pos, VALUE adv, 
			    VALUE txt) {
   console2dFillLine(FIX2INT(mode), FIX2INT(pos), FIX2INT(adv), 
		     StringValuePtr(txt));
   return Qnil;
}

/*
  call-seq: set_line_color(y, col)

  Draws a line in a given color.
*/
VALUE rubyConsole2dSetLineColor(VALUE self, VALUE y, VALUE col) {
   console2dSetLineColor(FIX2INT(y), hash2col(col));
   return Qnil;
}

/*
  call-seq: color=(col)

  Sets Console2d's color.
*/
VALUE rubyConsole2dSetColor(VALUE self, VALUE col) {
   console2dSetColor(hash2col(col));
   return Qnil;
}

/*
  Clears the screen.
*/
VALUE rubyConsole2dClear(VALUE self) {
   console2dClear();
   return Qnil;
}

/*
  Flips the screen.
*/
VALUE rubyConsole2dFlip(VALUE self) {
   console2dFlip();
   return Qnil;
}

/*
  Switches auto_flip on/off.
  When true,
 */
VALUE rubyConsole2dSwitchAutoFlip(VALUE self) {
   console2dSwitchAutoFlip();
   return Qnil;
}

/* Document-class: Console

   This module contains functions used when drawing text and getting text
   from a keyboard.

   Some files are required to use it. You can see them in the samples using
   that module.
*/

/* Document-class: Console2d

   This module contains functions used when creating text-based games.
*/
void defineConsole() {
   VALUE mConsole = rb_define_module("Console");
   defModFunc(mConsole, "init", rubyConsoleInit, 1);
   defModFunc(mConsole, "exit", rubyConsoleExit, 0);
   
   defModFunc(mConsole, "switch_sfx", rubyConsoleSwitchSFX, 0);
   
   defModFunc(mConsole, "puts", rubyConsolePuts, 1);
   defModFunc(mConsole, "print", rubyConsolePrint, 1);
   defModFunc(mConsole, "replace", rubyConsoleReplace, 1);

   defModFunc(mConsole, "gets", rubyConsoleGets, 0);
   defModFunc(mConsole, "get_num", rubyConsoleGetNum, 0);

   defModFunc(mConsole, "is_button?", rubyConsoleIsButton, 2);
   defModFunc(mConsole, "wait_key", rubyConsoleWaitKey, 2);

   defModFunc(mConsole, "text_color=", rubyConsoleSetTextColor, 1);

   defModFunc(mConsole, "switch_fixed", rubyConsoleSwitchFixedMode, 0);

   /*
     Document-const: FULL
     FULL: Loads the library with its keyboard.
   */

   /*
     Document-const: LITE
     LITE: Loads the library without its keyboard.
   */
   defConst(mConsole, "FULL", INT2FIX(FULL));
   defConst(mConsole, "LITE", INT2FIX(LITE));

   
   /*
     Document-const: PRESSED
     PRESSED: State for a key which is held.
   */

   /*
     Document-const: JUST_PRESSED
     JUST_PRESSED: State for a key which has been pressed.
   */

   /*
     Document-const: JUST_RELEASED
     JUST_RELEASED: State for a key which has been released.
   */

   /*
     Document-const: STILL_PRESSED
     STILL_PRESSED: State for a key which is still pressed.
   */

   defConst(mConsole, "PRESSED", INT2FIX(PRESSED));
   defConst(mConsole, "JUST_PRESSED", INT2FIX(JUST_PRESSED));
   defConst(mConsole, "JUST_RELEASED", INT2FIX(JUST_RELEASED));
   defConst(mConsole, "STILL_PRESSED", INT2FIX(STILL_PRESSED));

   /*
     Document-const: ANY_KEY
     0: This code can be given to wait untill any key is pressed.
   */

   defConst(mConsole, "ANY_KEY", INT2FIX(0));

   VALUE mConsole2d = rb_define_module("Console2d");

   defModFunc(mConsole2d, "init", rubyConsole2dInit, 0);
   defModFunc(mConsole2d, "exit", rubyConsole2dExit, 0);

   defModFunc(mConsole2d, "print", rubyConsole2dPrint, 4);
   
   defModFunc(mConsole2d, "fill_line", rubyConsole2dFillLine, 4);
   defModFunc(mConsole2d, "set_line_color", rubyConsole2dSetLineColor, 2);

   defModFunc(mConsole2d, "color=", rubyConsole2dSetColor, 1);

   defModFunc(mConsole2d, "clear", rubyConsole2dClear, 0);

   defModFunc(mConsole2d, "flip", rubyConsole2dFlip, 0);
   defModFunc(mConsole2d, "switch_auto_flip", rubyConsole2dSwitchAutoFlip, 0);

   /*
     Document-const: HORIZONTAL
     H: Use this when attempting to select an horizontal line.
   */

   /*
     Document-const: VERTICAL
     V: Use this when attempting to select a vertical line.
   */
   defConst(mConsole2d, "HORIZONTAL", INT2FIX(H));
   defConst(mConsole2d, "VERTICAL", INT2FIX(V));

   /*
     Document-const: LEFT
     L: Use this when you want to draw something alligned on the left.
   */

   /*
     Document-const: RIGHT
     R: Use this when you want to draw something alligned on the right.
   */
   defConst(mConsole2d, "LEFT", INT2FIX(L));
   defConst(mConsole2d, "RIGHT", INT2FIX(R));
}
