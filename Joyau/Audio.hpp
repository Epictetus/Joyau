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

#ifndef JOYAU_AUDIO
#define JOYAU_AUDIO

#include "StdInclude.hpp"

#define SAMPLES_RATE 44100
#define SAMPLES_FORMAT AL_FORMAT_MONO16
#define BUFFER_SIZE 4096 // A really little buffer, it's for the PSP...
#define NUM_BUFFERS 8

struct Vector3f
{
   Vector3f(float argX = 0.f, float argY = 0.f, float argZ = 0.f):
      x(argX), y(argY), z(argZ)
   {}

   Vector3f operator+(const Vector3f &op) const;
   Vector3f operator-(const Vector3f &op) const;

   bool operator==(const Vector3f &op) const;

   float x, y, z; 
};

class AudioObject
{
public:
   void setPos(float x, float y, float z);
   void setVelocity(float x, float y, float z);
   void setDirection(float x, float y, float z);

   void setPos(const Vector3f &vector);
   void setVelocity(const Vector3f &vector);
   void setDirection(const Vector3f &vector);
protected:
   ALuint source;
};

class Sound: public AudioObject // Short sound, wav. They're supported by alut.
{
public:
   virtual ~Sound();

   bool loadWav(const char *filename);

   void play();
   void pause();
   void stop();
private:
   ALuint buffer;
};

class Stream: public AudioObject // We'll use ogg for stream.
{
public:
   virtual ~Stream();

   bool loadOgg(const char *filename);

   bool play();
   void pause();
   void stop();
   bool playing();
   bool update();
private:
   bool streamBuf(ALuint buffer);
   void clear();

   FILE *file;

   OggVorbis_File stream;
   vorbis_info *info;
   vorbis_comment *comment;
   
   ALuint buffers[2];
   ALenum format;
};

void initOpenAl();
void stopOpenAL();

VALUE Audio_init(VALUE self);
VALUE Audio_stop(VALUE self);

VALUE Vector3f_setX(VALUE self, VALUE val);
VALUE Vector3f_setY(VALUE self, VALUE val);
VALUE Vector3f_setZ(VALUE self, VALUE val);

VALUE Vector3f_x(VALUE self);
VALUE Vector3f_y(VALUE self);
VALUE Vector3f_z(VALUE self);

VALUE Vector3f_add(VALUE self, VALUE op);
VALUE Vector3f_sub(VALUE self, VALUE op);
VALUE Vector3f_eq(VALUE self, VALUE op);

VALUE AudioObject_setPos(VALUE self, VALUE x, VALUE y, VALUE z);
VALUE AudioObject_setVelocity(VALUE self, VALUE x, VALUE y, VALUE z);
VALUE AudioObject_setDirection(VALUE self, VALUE x, VALUE y, VALUE z);

VALUE AudioObject_setPosVector(VALUE self, VALUE val);
VALUE AudioObject_setVelocityVector(VALUE self, VALUE val);
VALUE AudioObject_setDirectionVector(VALUE self, VALUE val);

VALUE Sound_loadWav(VALUE self, VALUE filename);

VALUE Sound_play(VALUE self);
VALUE Sound_pause(VALUE self);
VALUE Sound_stop(VALUE self);

VALUE Stream_loadOgg(VALUE self, VALUE filename);

VALUE Stream_play(VALUE self);
VALUE Stream_pause(VALUE self);
VALUE Stream_stop(VALUE self);
VALUE Stream_playing(VALUE self);
VALUE Stream_update(VALUE self);

VALUE Listener_setPos(VALUE self, VALUE x, VALUE y, VALUE z);
VALUE Listener_setVelocity(VALUE self, VALUE x, VALUE y, VALUE z);
VALUE Listener_setDirection(VALUE self, VALUE x, VALUE y, VALUE z);
VALUE Listener_setOrientation(VALUE self, VALUE atX, VALUE atY, VALUE atZ,
			      VALUE upX, VALUE upY, VALUE upZ);

VALUE Listener_posOp(VALUE self, VALUE val);
VALUE Listener_velocityOp(VALUE self, VALUE val);
VALUE Listener_directionOp(VALUE self, VALUE val);

void defineAudio();

#endif
