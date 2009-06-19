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

VALUE wrapSound(VALUE info)
{
   Sound *item = new Sound;
   VALUE tdata = Data_Wrap_Struct(info, 0, free, item);
   return tdata;
}

VALUE Audio_init(VALUE self)
{
   oslInitAudio();
   oslInitAudioME(OSL_FMT_ALL);
   return Qnil;
}

VALUE Audio_stop(VALUE self)
{
   oslDeinitAudio();
   return Qnil;
}

VALUE Audio_setSound(VALUE self, VALUE name)
{
   Sound *item;
   Data_Get_Struct(self, Sound, item);
   item->name = StringValuePtr(name);

   return Qnil;
}

VALUE Audio_setChannel(VALUE self, VALUE id)
{
   Sound *item;
   Data_Get_Struct(self, Sound, item);
   item->channel = FIX2INT(id);

   return Qnil;
}


VALUE Audio_playStream(VALUE self)
{
   Manager *m = Manager::getInstance();
   Sound *item;
   Data_Get_Struct(self, Sound, item);
   
   oslPlaySound(m->getStream(item->name), item->channel);

   return Qnil;
}

VALUE Audio_playSound(VALUE self)
{
   Manager *m = Manager::getInstance();
   Sound *item;
   Data_Get_Struct(self, Sound, item);
   
   oslPlaySound(m->getSound(item->name), item->channel);

   return Qnil;
}

VALUE Audio_toggleStream(VALUE self)
{
   Manager *m = Manager::getInstance();
   Sound *item;
   Data_Get_Struct(self, Sound, item);

   oslPauseSound(m->getStream(item->name), -1);
   item->status = item->status ? false : true;

   return Qnil;
}

VALUE Audio_toggleSound(VALUE self)
{
   Manager *m = Manager::getInstance();
   Sound *item;
   Data_Get_Struct(self, Sound, item);

   oslPauseSound(m->getSound(item->name), -1);
   item->status = item->status ? false : true;

   return Qnil;
}

VALUE Audio_stopStream(VALUE self)
{
   Manager *m = Manager::getInstance();
   Sound *item;
   Data_Get_Struct(self, Sound, item);

   oslStopSound(m->getStream(item->name));

   return Qnil;
}

VALUE Audio_stopSound(VALUE self)
{
   Manager *m = Manager::getInstance();
   Sound *item;
   Data_Get_Struct(self, Sound, item);

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
   VALUE cSound = rb_define_class("Sound", rb_cObject);

   rb_define_singleton_method(cSound, "new", (VALUE(*)(...))&wrapSound, 0);
   rb_define_method(cSound, "setSound", (VALUE(*)(...))&Audio_setSound, 1);
   rb_define_method(cSound, "setChannel", (VALUE(*)(...))&Audio_setChannel, 1);
   rb_define_method(cSound, "play", (VALUE(*)(...))&Audio_playSound, 0);
   rb_define_method(cSound, "toggle", (VALUE(*)(...))&Audio_toggleSound, 0);
   rb_define_method(cSound, "stop", (VALUE(*)(...))&Audio_stopSound, 0);

   VALUE cStream = rb_define_class("Stream", rb_cObject);

   rb_define_singleton_method(cStream, "new", (VALUE(*)(...))&wrapSound, 0);
   rb_define_method(cStream, "setSound", (VALUE(*)(...))&Audio_setSound, 1);
   rb_define_method(cStream, "setChannel", (VALUE(*)(...))&Audio_setChannel
		    , 1);
   rb_define_method(cStream, "play", (VALUE(*)(...))&Audio_playStream, 0);
   rb_define_method(cStream, "toggle", (VALUE(*)(...))&Audio_toggleStream, 0);
   rb_define_method(cStream, "stop", (VALUE(*)(...))&Audio_stopStream, 0);

   rb_define_global_function("initAudio", (VALUE(*)(...))&Audio_init, 0);
   rb_define_global_function("stopAudio", (VALUE(*)(...))&Audio_stop, 0);
   rb_define_global_function("audioSync", (VALUE(*)(...))&Audio_sync, 0);
}
