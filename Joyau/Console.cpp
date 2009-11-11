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
}
