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

#include "Timer.hpp"

/*
  Document-class: Joyau::Timer

  This class allows to count the ellapsed seconds between two moments.
  You can pause that timer, but notice that, since it is based on timestamps,
  it's still running while the PSP is not.
*/
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

time_t Timer::getTime() const
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

/*
  Restarts the timer from zero.
*/
VALUE Timer_reset(VALUE self)
{
   Timer &ref = getRef<Timer>(self);
   ref.reset();

   return Qnil;
}

/*
  Returns the ellapsed time since the timer's creation.
*/
VALUE Timer_getTime(VALUE self)
{
   Timer &ref = getRef<Timer>(self);
   int ret = ref.getTime();

   return INT2FIX(ret);
}

/*
  Pauses the timer.
*/
VALUE Timer_pause(VALUE self)
{
   Timer &ref = getRef<Timer>(self);
   ref.pause();

   return Qnil;
}

/*
  Resumes the timer.
*/
VALUE Timer_resume(VALUE self)
{
   Timer &ref = getRef<Timer>(self);
   ref.resume();

   return Qnil;
}

/*
  Returns whether the timer is paused.
*/
VALUE Timer_paused(VALUE self)
{
   Timer &ref = getRef<Timer>(self);
   return ref.isPaused() ? Qtrue : Qfalse;
}

void defineTimer()
{
   VALUE cTimer = defClass<Timer>("Timer");
   defMethod(cTimer, "reset", Timer_reset, 0);
   defMethod(cTimer, "getTime", Timer_getTime, 0);
   defMethod(cTimer, "pause", Timer_pause, 0);
   defMethod(cTimer, "resume", Timer_resume, 0);
   defMethod(cTimer, "paused", Timer_paused, 0);

   defAlias(cTimer, "getTime", "time");
   defAlias(cTimer, "paused", "paused?");
}
