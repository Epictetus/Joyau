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
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA. */

#ifndef Joyau_OSLMUSIC
#define Joyau_OSLMUSIC

#include "StdInclude.hpp"
#include "Manager.hpp"

class OslMusic {
public:
   OslMusic();
   OslMusic(const std::string &title, int stream);

   void load(const std::string &title, int stream);

   void play(int voice);
   void resume();
   void pause();
   void stop();

   int playingOn();
private:
   OSL_SOUND *sound;
};

VALUE OslMusic_init(VALUE self);
VALUE OslMusic_stop_mod(VALUE self);

VALUE OslMusic_load(int argc, VALUE *argv, VALUE self);

VALUE OslMusic_play(VALUE self, VALUE voice);
VALUE OslMusic_resume(VALUE self);
VALUE OslMusic_pause(VALUE self);
VALUE OslMusic_stop(VALUE self);

VALUE OslMusic_playing_on(VALUE self);

void defineOslMusic();

#endif
