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

#ifndef __AUDIO__
#define __AUDIO__

#include "StdInclude.hpp"
#include "Manager.hpp"

struct Sound
{
   char *name;
   int channel;
   bool status;
};

VALUE Audio_init(VALUE self);
VALUE Audio_stop(VALUE self);

VALUE Audio_setSound(VALUE self, VALUE name);
VALUE Audio_setChannel(VALUE self, VALUE id);

VALUE Audio_playStream(VALUE self);
VALUE Audio_playSound(VALUE self);

VALUE Audio_toggleStream(VALUE self);
VALUE Audio_toggleSound(VALUE self);

VALUE Audio_stopStream(VALUE self);
VALUE Audio_stopSound(VALUE self);

VALUE Audio_sync(VALUE self);

void defineAudio();

#endif
