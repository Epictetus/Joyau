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

#include "OslSound.hpp"

OslMusic::OslMusic():
   sound(NULL)
{}

OslMusic::OslMusic(const std::string &title, int stream) {
   load(title, stream);
}

void OslMusic::load(const std::string &title, int stream) {
   sound = Manager::getInstance().getOslMusic(title, stream);
   if (!sound)
      throw RubyException(rb_eRuntimeError, "Couldn't load audio file");
}

void OslMusic::play(int voice) {
   if (!sound)
      return;
   oslPlaySound(sound, voice);
}

void OslMusic::resume() {
   if (!sound)
      return;
   oslPlaySound(sound, 0);
}

void OslMusic::pause() {
   if (!sound)
      return;
   oslPauseSound(sound, 1);
}

void OslMusic::stop() {
   if (!sound)
      return;
   oslStopSound(sound);
}

int OslMusic::playingOn() {
   if (!sound)
      return -1;
   return oslGetSoundChannel(sound);
}

/*
  Inits the audio module used for OslMusic.
*/
VALUE OslMusic_init(VALUE self) {
   oslInitAudio();
   oslInitAudioME(OSL_FMT_ALL);
   return Qnil;
}

/*
  Stops the audio module used for OslMusic.
*/
VALUE OslMusic_stop_mod(VALUE self) {
   oslDeinitAudio();
   return Qnil;
}

/*
  call-seq: load(filename, stream = Joyau::OslMusic::FMT_NONE)

  Loads the given sound.
*/
VALUE OslMusic_load(int argc, VALUE *argv, VALUE self) {
   OslMusic &ref = getRef<OslMusic>(self);
   VALUE filename, stream;
   rb_scan_args(argc, argv, "11", &filename, &stream);

   if (NIL_P(stream))
      stream = INT2FIX(OSL_FMT_NONE);
   filename = rb_obj_as_string(filename);

   try { 
      ref.load(StringValuePtr(filename), FIX2INT(stream));
   }
   catch (const RubyException &e) {
      e.rbRaise();
   }

   return Qnil;
}

/*
  call-seq: play(channel)

  Starts playing a sound on the specified channel.
*/
VALUE OslMusic_play(VALUE self, VALUE voice) {
   OslMusic &ref = getRef<OslMusic>(self);
   ref.play(FIX2INT(voice));

   return Qnil;
}

/*
  Resumes the sound if it was paused.
*/
VALUE OslMusic_resume(VALUE self) {
   OslMusic &ref = getRef<OslMusic>(self);
   ref.resume();

   return Qnil;
}

/*
  Pauses the sound.
*/
VALUE OslMusic_pause(VALUE self) {
   OslMusic &ref = getRef<OslMusic>(self);
   ref.pause();

   return Qnil;
}

/*
  Stops the sound
*/
VALUE OslMusic_stop(VALUE self) {
   OslMusic &ref = getRef<OslMusic>(self);
   ref.stop();

   return Qnil;
}

/*
  Returns the id of the channel on which this sound is being played, or
  -1 if it isn't being played.
*/
VALUE OslMusic_playing_on(VALUE self) {
   OslMusic &ref = getRef<OslMusic>(self);
   return INT2FIX(ref.playingOn());
}

template <>
/*
  call-seq: new
            new(filename, type = Joyau::OslMusic::FMT_NONE)

  Creates a new sound. You can directlyload it, and specify
  whether or not it should be played as a stream.
*/
VALUE wrap<OslMusic>(int argc, VALUE *argv, VALUE info) {
   VALUE filename, stream;
   rb_scan_args(argc, argv, "02", &filename, &stream);
   OslMusic *ptr = new OslMusic;

   if (!NIL_P(filename)) {
      if (NIL_P(stream))
	 stream = INT2FIX(OSL_FMT_NONE);
      filename = rb_obj_as_string(filename);

      try {
	 ptr->load(StringValuePtr(filename), FIX2INT(stream));
      }
      catch (const RubyException &e) {
	 e.rbRaise();
      }
   }

   VALUE tdata = Data_Wrap_Struct(info, 0, wrapped_free<OslMusic>, ptr);
   return tdata;
}

void defineOslMusic() {
   VALUE cOslMusic = defClass<OslMusic>("OslMusic");
   defMethod(cOslMusic, "play", OslMusic_play, 1);
   defMethod(cOslMusic, "pause", OslMusic_pause, 0);
   defMethod(cOslMusic, "resume", OslMusic_resume, 0);
   defMethod(cOslMusic, "stop", OslMusic_stop, 0);
   defMethod(cOslMusic, "playing_on", OslMusic_playing_on, 0);
   
   defClassMethod(cOslMusic, "init", OslMusic_init, 0);
   defClassMethod(cOslMusic, "stop", OslMusic_stop_mod, 0);

   defConst(cOslMusic, "FMT_NONE", INT2FIX(OSL_FMT_NONE));
   defConst(cOslMusic, "FMT_STREAM", INT2FIX(OSL_FMT_STREAM));
}
