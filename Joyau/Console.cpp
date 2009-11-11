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

VALUE rubyConsoleInit(VALUE self, VALUE mode) {
   consoleInit(FIX2INT(mode));
   return Qnil;
}

VALUE rubyConsoleExit(VALUE self) {
   consoleExit();
   return Qnil;
}

VALUE rubyConsoleSwitchSFX(VALUE self) {
   return INT2FIX(consoleSwitchSFX());
}

VALUE rubyConsolePuts(VALUE self, VALUE str) {
   consolePrintText(ADD, StringValuePtr(str));
   return Qnil;
}

VALUE rubyConsolePrint(VALUE self, VALUE str) {
   consolePrintText(CAT, StringValuePtr(str));
   return Qnil;
}

VALUE rubyConsoleReplace(VALUE self, VALUE str) {
   consolePrintText(REPLACE, StringValuePtr(str));
   return Qnil;
}

VALUE rubyConsoleGets(VALUE self) {
   return rb_str_new2(consoleInputText(LETTERS));
}

VALUE rubyConsoleGetNum(VALUE self) {
   VALUE str = rb_str_new2(consoleInputText(NUMBERS));
   return rb_funcall(str, getFunc("to_i"), 0);
}

VALUE rubyConsoleClear(VALUE self) {
   consoleFreeTextArray();
   return Qnil;
}

VALUE rubyConsoleIsButton(VALUE self, VALUE button, VALUE state) {
   return consoleButton(FIX2INT(button), FIX2INT(state)) ? Qtrue : Qfalse;
}

VALUE rubyConsoleWaitKey(VALUE self, VALUE button, VALUE str) {
   consoleWaitButton(FIX2INT(button), StringValuePtr(str));
   return Qnil;
}

VALUE rubyConsoleSetTextColor(VALUE self, VALUE color) {
   consoleSetTextColor(FIX2INT(color));
   return Qnil;
}

VALUE rubyConsoleSwitchFixedMode(VALUE self) {
   consoleSwitchFixedMode();
   return Qnil;
}

VALUE rubyConsole2dInit(VALUE self) {
   console2dInit();
   return Qnil;
}

VALUE rubyConsole2dExit(VALUE self) {
   console2dExit();
   return Qnil;
}

VALUE rubyConsole2dPrint(VALUE self, VALUE x, VALUE y, VALUE mode, VALUE txt) {
   console2dPrint(FIX2INT(x), FIX2INT(y), FIX2INT(mode), StringValuePtr(txt));
   return Qnil;
}

VALUE rubyConsole2dFillLine(VALUE self, VALUE mode, VALUE pos, VALUE adv, VALUE txt) {
   console2dFillLine(FIX2INT(mode), FIX2INT(pos), FIX2INT(adv), StringValuePtr(txt));
   return Qnil;
}

VALUE rubyConsole2dSetLineColor(VALUE self, VALUE y, VALUE col) {
   console2dSetLineColor(FIX2INT(y), FIX2INT(col));
   return Qnil;
}

VALUE rubyConsole2dSetColor(VALUE self, VALUE col) {
   console2dSetColor(FIX2INT(col));
   return Qnil;
}

VALUE rubyConsole2dClear(VALUE self) {
   console2dClear();
   return Qnil;
}

VALUE rubyConsole2dFlip(VALUE self) {
   console2dFlip();
   return Qnil;
}

VALUE rubyConsole2dSwitchAutoFlip(VALUE self) {
   console2dSwitchAutoFlip();
   return Qnil;
}

void defineConsole() {
   VALUE mConsole = rb_define_module("Console"); // Not defined in Joyau
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

   defConst(mConsole, "FULL", INT2FIX(FULL));
   defConst(mConsole, "LITE", INT2FIX(LITE));

   defConst(mConsole, "PRESSED", INT2FIX(PRESSED));
   defConst(mConsole, "JUST_PRESSED", INT2FIX(JUST_PRESSED));
   defConst(mConsole, "JUST_RELEASED", INT2FIX(JUST_RELEASED));
   defConst(mConsole, "STILL_PRESSED", INT2FIX(STILL_PRESSED));

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

   defConst(mConsole2d, "HORIZONTAL", INT2FIX(H));
   defConst(mConsole2d, "VERTICAL", INT2FIX(V));

   defConst(mConsole2d, "LEFT", INT2FIX(L));
   defConst(mConsole2d, "RIGHT", INT2FIX(R));
}
