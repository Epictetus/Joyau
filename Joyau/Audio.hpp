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
#include "RubyWrapper.hpp"

#define SAMPLES_RATE 44100
#define SAMPLES_FORMAT AL_FORMAT_MONO16
#define BUFFER_SIZE 4096 // A really little buffer, it's for the PSP...
#define NUM_BUFFERS 8

/*struct Sound
{
   char *name;
   int channel;
   bool status;
};*/

class Sound // Short sound, wav. They're supported by alut.
{
public:
   virtual ~Sound();

   bool loadWav(const char *filename);

   void play();
   void pause();
   void stop();

   void setPos(float x = 0.f, float y = 0.f, float z = 0.f);
   void setVelocity(float x = 0.f, float y = 0.f, float z = 0.f);
   void setDirection(float x = 0.f, float y = 0.f, float z = 0.f);
private:
   ALuint source;
   ALuint buffer;
};

class Stream // We'll use ogg for stream.
{
public:
   virtual ~Stream();

   bool loadOgg(const char *filename);

   bool play();
   void pause();
   void stop();
   bool playing();
   bool update();

   void setPos(float x = 0.f, float y = 0.f, float z = 0.f);
   void setVelocity(float x = 0.f, float y = 0.f, float z = 0.f);
   void setDirection(float x = 0.f, float y = 0.f, float z = 0.f);
private:
   bool streamBuf(ALuint buffer);
   void clear();

   FILE *file;

   OggVorbis_File stream;
   vorbis_info *info;
   vorbis_comment *comment;
   
   ALuint buffers[2];
   ALuint source;

   ALenum format;
};

void initOpenAl();
void stopOpenAL();

VALUE Audio_init(VALUE self);
VALUE Audio_stop(VALUE self);

VALUE Sound_loadWav(VALUE self, VALUE filename);

VALUE Sound_play(VALUE self);
VALUE Sound_pause(VALUE self);
VALUE Sound_stop(VALUE self);

VALUE Sound_setPos(VALUE self, VALUE x, VALUE y, VALUE z);
VALUE Sound_setVelocity(VALUE self, VALUE x, VALUE y, VALUE z);
VALUE Sound_setDirection(VALUE self, VALUE x, VALUE y, VALUE z);

VALUE Stream_loadOgg(VALUE self, VALUE filename);

VALUE Stream_play(VALUE self);
VALUE Stream_pause(VALUE self);
VALUE Stream_stop(VALUE self);
VALUE Stream_playing(VALUE self);
VALUE Stream_update(VALUE self);

VALUE Stream_setPos(VALUE self, VALUE x, VALUE y, VALUE z);
VALUE Stream_setVelocity(VALUE self, VALUE x, VALUE y, VALUE z);
VALUE Stream_setDirection(VALUE self, VALUE x, VALUE y, VALUE z);

void defineAudio();

#endif
