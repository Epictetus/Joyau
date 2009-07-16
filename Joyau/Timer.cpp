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

#include "Timer.hpp"

Timer::Timer()
{
   reset();
   paused = false;
}

void Timer::reset()
{
   offset = 0;
   sceKernelLibcTime(&startTimestamp);
}

time_t Timer::getTime()
{
   if (!paused)
   {
      time_t actual;
      sceKernelLibcTime(&actual);
      return actual - startTimestamp + offset;
   }
   else
   {
      return offset;
   }
}

void Timer::pause()
{
   time_t actual;
   sceKernelLibcTime(&actual);

   offset = actual - startTimestamp + offset;
   paused = true;
}

void Timer::resume()
{
   sceKernelLibcTime(&startTimestamp);
   paused = false;
}

VALUE Timer_reset(VALUE self)
{
   Timer *ptr = getPtr<Timer>(self);
   ptr->reset();

   return Qnil;
}

VALUE Timer_getTime(VALUE self)
{
   Timer *ptr = getPtr<Timer>(self);
   int ret = static_cast<int>(ptr->getTime());

   return INT2FIX(ret);
}

VALUE Timer_pause(VALUE self)
{
   Timer *ptr = getPtr<Timer>(self);
   ptr->pause();

   return Qnil;
}

VALUE Timer_resume(VALUE self)
{
   Timer *ptr = getPtr<Timer>(self);
   ptr->resume();

   return Qnil;
}

VALUE Timer_paused(VALUE self)
{
   Timer *ptr = getPtr<Timer>(self);
   return ptr->isPaused() ? Qtrue : Qfalse;
}

void defineTimer()
{
   VALUE cTimer = defClass<Timer>("Timer");
   defMethod(cTimer, "reset", Timer_reset, 0);
   defMethod(cTimer, "getTime", Timer_getTime, 0);
   defMethod(cTimer, "pause", Timer_pause, 0);
   defMethod(cTimer, "resume", Timer_resume, 0);
   defMethod(cTimer, "paused", Timer_paused, 0);
}
