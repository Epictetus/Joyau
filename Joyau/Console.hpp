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

#include "StdInclude.hpp"

VALUE rubyConsoleInit(VALUE self, VALUE mode);
VALUE rubyConsoleExit(VALUE self);

VALUE rubyConsoleSwitchSFX(VALUE self);

VALUE rubyConsolePuts(VALUE self, VALUE str);
VALUE rubyConsolePrint(VALUE self, VALUE str);
VALUE rubyConsoleReplace(VALUE self, VALUE str);

VALUE rubyConsoleGets(VALUE self);
VALUE rubyConsoleGetNum(VALUE self);

VALUE rubyConsoleClear(VALUE self);

VALUE rubyConsoleIsButton(VALUE self, VALUE button, VALUE state);
VALUE rubyConsoleWaitKey(VALUE self, VALUE button, VALUE str);

VALUE rubyConsoleSetTextColor(VALUE self, VALUE color);

VALUE rubyConsoleSwitchFixedMode(VALUE self);

VALUE rubyConsole2dInit(VALUE self);
VALUE rubyConsole2dExit(VALUE self);

VALUE rubyConsole2dPrint(VALUE self, VALUE x, VALUE y, VALUE mode, VALUE txt);

VALUE rubyConsole2dFillLine(VALUE self, VALUE mode, VALUE pos, VALUE adv, 
			    VALUE txt);
VALUE rubyConsole2dSetLineColor(VALUE self, VALUE y, VALUE col);

VALUE rubyConsole2dSetColor(VALUE self, VALUE col);

VALUE rubyConsole2dClear(VALUE self);

VALUE rubyConsole2dFlip(VALUE self);
VALUE rubyConsole2dSwitchAutoFlip(VALUE self);

void defineConsole();
