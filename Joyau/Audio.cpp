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

#include "Audio.hpp"

VALUE Audio_init(VALUE self)
{
   oslInitAudio();
   return Qnil;
}

VALUE Audio_kinit(VALUE self)
{
   oslInitAudioME(OSL_FMT_MP3);
   return Qnil;
}

VALUE Audio_stop(VALUE self)
{
   oslDeinitAudio();
   return Qnil;
}

VALUE Audio_setSound(VALUE self, VALUE name)
{
   Sound *item = getPtr<Sound>(self);
   item->name = StringValuePtr(name);

   return Qnil;
}

VALUE Audio_setChannel(VALUE self, VALUE id)
{
   Sound *item = getPtr<Sound>(self);
   item->channel = FIX2INT(id);

   return Qnil;
}


VALUE Audio_playStream(VALUE self)
{
   Manager *m = Manager::getInstance();

   Sound *item = getPtr<Sound>(self);
   oslPlaySound(m->getStream(item->name), item->channel);

   return Qnil;
}

VALUE Audio_playSound(VALUE self)
{
   Manager *m = Manager::getInstance();

   Sound *item = getPtr<Sound>(self);   
   oslPlaySound(m->getSound(item->name), item->channel);

   return Qnil;
}

VALUE Audio_toggleStream(VALUE self)
{
   Manager *m = Manager::getInstance();

   Sound *item = getPtr<Sound>(self);
   oslPauseSound(m->getStream(item->name), -1);
   item->status = item->status ? false : true;

   return Qnil;
}

VALUE Audio_toggleSound(VALUE self)
{
   Manager *m = Manager::getInstance();

   Sound *item = getPtr<Sound>(self);
   oslPauseSound(m->getSound(item->name), -1);
   item->status = item->status ? false : true;

   return Qnil;
}

VALUE Audio_stopStream(VALUE self)
{
   Manager *m = Manager::getInstance();

   Sound *item = getPtr<Sound>(self);
   oslStopSound(m->getStream(item->name));

   return Qnil;
}

VALUE Audio_stopSound(VALUE self)
{
   Manager *m = Manager::getInstance();

   Sound *item = getPtr<Sound>(self);
   oslStopSound(m->getSound(item->name));

   return Qnil;
}

VALUE Audio_sync(VALUE self)
{
   oslAudioVSync();
   return Qnil;
}

void defineAudio()
{
   VALUE cSound = defClass<Sound>("Sound");
   defMethod(cSound, "setSound", Audio_setSound, 1);
   defMethod(cSound, "setChannel", Audio_setChannel, 1);
   defMethod(cSound, "play", Audio_playSound, 0);
   defMethod(cSound, "toggle", Audio_toggleSound, 0);
   defMethod(cSound, "stop", Audio_stopSound, 0);

   VALUE cStream = defClass<Sound>("Stream");
   defMethod(cStream, "setSound", Audio_setSound, 1);
   defMethod(cStream, "setChannel", Audio_setChannel, 1);
   defMethod(cStream, "play", Audio_playStream, 0);
   defMethod(cStream, "toggle", Audio_toggleStream, 0);
   defMethod(cStream, "stop", Audio_stopStream, 0);

   defFunc("initAudio", Audio_init, 0);
   defFunc("kinitAudio", Audio_kinit, 0);
   defFunc("stopAudio", Audio_stop, 0);
   defFunc("audioSync", Audio_sync, 0);
}
