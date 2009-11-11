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

void defineConsole();
