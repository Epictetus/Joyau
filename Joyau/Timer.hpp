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

#ifndef JOYAU_TIMER
#define JOYAU_TIMER

#include "StdInclude.hpp"

class Timer
{
public:
   Timer();
   void reset();

   time_t getTime();

   void pause();
   void resume();

   bool isPaused() { return paused; }
private:
   bool paused;

   time_t offset;
   time_t startTimestamp;
};

VALUE Timer_reset(VALUE self);

VALUE Timer_getTime(VALUE self);

VALUE Timer_pause(VALUE self);
VALUE Timer_resume(VALUE self);

VALUE Timer_paused(VALUE self);

void defineTimer();

#endif
