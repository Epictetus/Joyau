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
#include "Manager.hpp"

Vector3f Vector3f::operator+(const Vector3f &op) const
{
   return Vector3f(x + op.x, y + op.y, z + op.z);
}

Vector3f Vector3f::operator-(const Vector3f &op) const
{
   return Vector3f(x + op.x, y + op.y, z + op.z);
}

bool Vector3f::operator==(const Vector3f &op) const
{
   return x == op.x && y == op.y && z == op.z;
}

void AudioObject::setPos(float x, float y, float z)
{
   alSource3f(source, AL_POSITION, x, y, z);
}

void AudioObject::setVelocity(float x, float y, float z)
{
   alSource3f(source, AL_VELOCITY, x, y, z);
}

void AudioObject::setDirection(float x, float y, float z)
{
   alSource3f(source, AL_DIRECTION, x, y, z);
}

void AudioObject::setPos(const Vector3f &vector)
{
   setPos(vector.x, vector.y, vector.z);
}

void AudioObject::setVelocity(const Vector3f &vector)
{
   setVelocity(vector.x, vector.y, vector.z);
}

void AudioObject::setDirection(const Vector3f &vector)
{
   setDirection(vector.x, vector.y, vector.z);
}


void AudioObject::setPitch(float val)
{
   alSourcef(source, AL_PITCH, val);
}

void AudioObject::setGain(float val)
{
   alSourcef(source, AL_GAIN, val);
}

bool AudioObject::playing() const
{
   ALenum state;
   alGetSourcei(source, AL_SOURCE_STATE, &state);

   return state == AL_PLAYING;
}

void AudioObject::generateSource()
{
   alGenSources(1, &source);
}

void AudioObject::deleteSource()
{
   alDeleteSources(1, &source);
}

int AudioObject::queuedBuffers()
{
   int queued;
   alGetSourcei(source, AL_BUFFERS_QUEUED, &queued);
   
   return queued;
}

int AudioObject::processedBuffers()
{
   int processed;
   alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);
   
   return processed;
}

void AudioObject::unqueueBuffers(int n, ALuint *buffers)
{
   alSourceUnqueueBuffers(source, n, buffers);
}

void AudioObject::queueBuffers(int n, ALuint *buffers)
{
   alSourceQueueBuffers(source, n, buffers);
}

void AudioObject::setBuffer(ALuint buffer)
{
   alSourcei(source, AL_BUFFER, buffer);
}

void AudioObject::playSource()
{
   alSourcePlay(source);
}

void AudioObject::pauseSource()
{
   alSourcePause(source);
}

void AudioObject::stopSource()
{
   alSourceStop(source);
}

template<> VALUE wrap<Sound>(int argc, VALUE *argv, VALUE info)
{
   Sound *ptr = new Sound;
   if (argc >= 1)
      ptr->loadWav(StringValuePtr(argv[0]));

   VALUE tdata = Data_Wrap_Struct(info, 0, wrapped_free<Sound>, ptr);
   return tdata;
}

template<> VALUE wrap<Stream>(int argc, VALUE *argv, VALUE info)
{
   Stream *ptr = new Stream;
   if (argc >= 1)
      ptr->loadOgg(StringValuePtr(argv[0]));

   VALUE tdata = Data_Wrap_Struct(info, 0, wrapped_free<Stream>, ptr);
   return tdata;
}

template<> VALUE wrap<Vector3f>(int argc, VALUE *argv, VALUE info)
{
   Vector3f *ptr = new Vector3f;
   if (argc >= 3)
   {
      ptr->x = NUM2DBL(argv[0]);
      ptr->y = NUM2DBL(argv[1]);
      ptr->z = NUM2DBL(argv[2]);
   }

   VALUE tdata = Data_Wrap_Struct(info, 0, wrapped_free<Vector3f>, ptr);
   return tdata;
}


Sound::Sound(const Sound &obj)
{
   loadWav(obj.sound.c_str());
}

Sound::~Sound()
{
   deleteSource();
}

bool Sound::loadWav(const char *filename)
{
   sound = filename;

   buffer = Manager::getInstance().getBuffer(filename);

   // We create a source, binded to our buffer :
   generateSource();
   if (alGetError() != AL_NO_ERROR)
      return false;

   setBuffer(buffer);

   setPos(0.f, 0.f, 0.f);
   setVelocity(0.f, 0.f, 0.f);
   setDirection(0.f, 0.f, 0.f);
   
   if (alGetError() == AL_NO_ERROR)
      return true;
   return false;
}

void Sound::play()
{
   if (!playing())
      playSource();
}

void Sound::pause()
{
   pauseSource();
}

void Sound::stop()
{
   stopSource();
}

Stream::Stream(const Stream &obj)
{
   loadOgg(obj.sound.c_str());
   if (obj.playing())
      play();
}

Stream::~Stream()
{
   stopSource();
   clear();

   deleteSource();
   alDeleteBuffers(1, buffers);

   ov_clear(&stream);
}

bool Stream::loadOgg(const char *filename)
{
   sound = filename;

   file = fopen(filename, "rb");
   ov_open(file, &stream, NULL, 0);

   info = ov_info(&stream, -1);
   comment = ov_comment(&stream, -1);

   format = SAMPLES_FORMAT;
   
   alGenBuffers(2, buffers);
   if (alGetError() != AL_NO_ERROR)
      return false;

   generateSource();
   if (alGetError() != AL_NO_ERROR)
      return false;

   setPos(0.f, 0.f, 0.f);
   setVelocity(0.f, 0.f, 0.f);
   setDirection(0.f, 0.f, 0.f);

   setPitch(1.f);
   setGain(1.f);

   return true;
}

bool Stream::play()
{
   // We don't want to play this Stream twice, do we ?
   if (playing())
      return true;
   if (!streamBuf(buffers[0]))
      return false;
   if (!streamBuf(buffers[1]))
      return false;

   queueBuffers(2, buffers);
   playSource();

   return true;
}

void Stream::pause()
{
   queueBuffers(2, buffers);
   pauseSource();
}

void Stream::stop()
{
   queueBuffers(2, buffers);
   stopSource();
}

bool Stream::update()
{
   int processed = processedBuffers();

   bool active = true;
   while (processed--)
   {
      ALuint buffer;
      unqueueBuffers(1, &buffer);
      
      active = streamBuf(buffer);

      queueBuffers(1, &buffer);
   }
   return active;
}

bool Stream::streamBuf(ALuint buffer)
{
   char data[BUFFER_SIZE];

   int size = 0;
   int section, result;

   while (size < BUFFER_SIZE)
   {
      result = ov_read(&stream, data + size, BUFFER_SIZE - size,
                       0, 2, 1, &section);
      if (result > 0)
         size += result;
      else
      {
         if (result < 0)
            return false;
         else
            break;
      }
   }

   if (size == 0)
      return false;

   alBufferData(buffer, format, data, size, info->rate);

   return true;
}

void Stream::clear()
{
   int queued = queuedBuffers();
   
   while (queued--)
   {
      ALuint buffer;
      unqueueBuffers(1, &buffer);
   }
}

void initOpenAl()
{
   int argc = Manager::getInstance().getArgc();
   char **argv = Manager::getInstance().getArgv();
   alutInit(&argc, argv);
}

void stopOpenAL()
{
   alutExit();
}

VALUE Audio_init(VALUE self)
{
   initOpenAl();
   return Qnil;
}

VALUE Audio_stop(VALUE self)
{
   stopOpenAL();
   return Qnil;
}

VALUE Vector3f_setX(VALUE self, VALUE val)
{
   Vector3f &ref = getRef<Vector3f>(self);
   ref.x = NUM2DBL(val);

   return Qnil;
}

VALUE Vector3f_setY(VALUE self, VALUE val)
{
   Vector3f &ref = getRef<Vector3f>(self);
   ref.y = NUM2DBL(val);

   return Qnil;
}

VALUE Vector3f_setZ(VALUE self, VALUE val)
{
   Vector3f &ref = getRef<Vector3f>(self);
   ref.z = NUM2DBL(val);

   return Qnil;
}

VALUE Vector3f_x(VALUE self)
{
   Vector3f &ref = getRef<Vector3f>(self);
   return rb_float_new(ref.x);
}

VALUE Vector3f_y(VALUE self)
{
   Vector3f &ref = getRef<Vector3f>(self);
   return rb_float_new(ref.y);
}

VALUE Vector3f_z(VALUE self)
{
   Vector3f &ref = getRef<Vector3f>(self);
   return rb_float_new(ref.z);
}

VALUE Vector3f_add(VALUE self, VALUE op)
{
   Vector3f &first = getRef<Vector3f>(self);
   Vector3f &second = getRef<Vector3f>(op);

   Vector3f ret = first + second;
   return createObject(getClass("Vector3f"), ret);
}

VALUE Vector3f_sub(VALUE self, VALUE op)
{
   Vector3f &first = getRef<Vector3f>(self);
   Vector3f &second = getRef<Vector3f>(op);

   Vector3f ret = first - second;
   return createObject(getClass("Vector3f"), ret);
}

VALUE Vector3f_eq(VALUE self, VALUE op)
{
   Vector3f &first = getRef<Vector3f>(self);
   Vector3f &second = getRef<Vector3f>(op);

   return first == second ? Qtrue : Qfalse;
}

VALUE AudioObject_setPos(VALUE self, VALUE x, VALUE y, VALUE z)
{
   AudioObject &ref = getRef<AudioObject>(self);
   double _x = NUM2DBL(x);
   double _y = NUM2DBL(y);
   double _z = NUM2DBL(z);

   ref.setPos(_x, _y, _z);

   return Qnil;
}

VALUE AudioObject_setVelocity(VALUE self, VALUE x, VALUE y, VALUE z)
{
   AudioObject &ref = getRef<AudioObject>(self);
   double _x = NUM2DBL(x);
   double _y = NUM2DBL(y);
   double _z = NUM2DBL(z);

   ref.setVelocity(_x, _y, _z);

   return Qnil;
}

VALUE AudioObject_setDirection(VALUE self, VALUE x, VALUE y, VALUE z)
{
   AudioObject &ref = getRef<AudioObject>(self);
   double _x = NUM2DBL(x);
   double _y = NUM2DBL(y);
   double _z = NUM2DBL(z);

   ref.setDirection(_x, _y, _z);

   return Qnil;
}

VALUE AudioObject_setPosVector(VALUE self, VALUE val)
{
   AudioObject &ref = getRef<AudioObject>(self);
   Vector3f &vector = getRef<Vector3f>(val);

   ref.setPos(vector);
   return Qnil;
}

VALUE AudioObject_setVelocityVector(VALUE self, VALUE val)
{
   AudioObject &ref = getRef<AudioObject>(self);
   Vector3f &vector = getRef<Vector3f>(val);

   ref.setVelocity(vector);
   return Qnil;
}

VALUE AudioObject_setDirectionVector(VALUE self, VALUE val)
{
   AudioObject &ref = getRef<AudioObject>(self);
   Vector3f &vector = getRef<Vector3f>(val);

   ref.setDirection(vector);
   return Qnil;
}

VALUE Sound_loadWav(VALUE self, VALUE filename)
{
   Sound &ref = getRef<Sound>(self);
   char *str = StringValuePtr(filename);

   if (ref.loadWav(str))
      return Qtrue;
   return Qfalse;
}

VALUE Sound_play(VALUE self)
{
   Sound &ref = getRef<Sound>(self);
   ref.play();
   return Qnil;
}

VALUE Sound_pause(VALUE self)
{
   Sound &ref = getRef<Sound>(self);
   ref.pause();

   return Qnil;
}

VALUE Sound_stop(VALUE self)
{
   Sound &ref = getRef<Sound>(self);
   ref.stop();

   return Qnil;
}

VALUE Stream_loadOgg(VALUE self, VALUE filename)
{
   Stream &ref = getRef<Stream>(self);
   char *str = StringValuePtr(filename);

   if (ref.loadOgg(str))
      return Qtrue;
   return Qfalse;
}

VALUE Stream_play(VALUE self)
{
   Stream &ref = getRef<Stream>(self);
   if (ref.play())
      return Qtrue;
   return Qfalse;
}

VALUE Stream_pause(VALUE self)
{
   Stream &ref = getRef<Stream>(self);
   ref.pause();

   return Qnil;
}

VALUE Stream_stop(VALUE self)
{
   Stream &ref = getRef<Stream>(self);
   ref.stop();

   return Qnil;
}

VALUE Stream_playing(VALUE self)
{
   Stream &ref = getRef<Stream>(self);
   if (ref.playing())
      return Qtrue;
   return Qfalse;
}

VALUE Stream_update(VALUE self)
{
   Stream &ref = getRef<Stream>(self);
   if (ref.update())
      return Qtrue;
   return Qfalse;
}

VALUE Listener_setPos(VALUE self, VALUE x, VALUE y, VALUE z)
{
   double _x = NUM2DBL(x);
   double _y = NUM2DBL(y);
   double _z = NUM2DBL(z);

   alListener3f(AL_POSITION, _x, _y, _z);
   return Qnil;
}

VALUE Listener_setVelocity(VALUE self, VALUE x, VALUE y, VALUE z)
{
   double _x = NUM2DBL(x);
   double _y = NUM2DBL(y);
   double _z = NUM2DBL(z);

   alListener3f(AL_VELOCITY, _x, _y, _z);
   return Qnil;
}

VALUE Listener_setDirection(VALUE self, VALUE x, VALUE y, VALUE z)
{
   double _x = NUM2DBL(x);
   double _y = NUM2DBL(y);
   double _z = NUM2DBL(z);

   alListener3f(AL_DIRECTION, _x, _y, _z);
   return Qnil;
}

VALUE Listener_setOrientation(VALUE self, VALUE atX, VALUE atY, VALUE atZ,
                              VALUE upX, VALUE upY, VALUE upZ)
{
   float args[6];

   args[0] = NUM2DBL(atX);
   args[1] = NUM2DBL(atY);
   args[2] = NUM2DBL(atZ);
   args[3] = NUM2DBL(upX);
   args[4] = NUM2DBL(upY);
   args[5] = NUM2DBL(upZ);

   alListenerfv(AL_ORIENTATION, args);
   return Qnil;
}

VALUE Listener_posOp(VALUE self, VALUE val)
{
   Vector3f &ref = getRef<Vector3f>(val);
   alListener3f(AL_POSITION, ref.x,ref.y, ref.z);

   return Qnil;
}

VALUE Listener_velocityOp(VALUE self, VALUE val)
{
   Vector3f &ref = getRef<Vector3f>(val);
   alListener3f(AL_VELOCITY, ref.x,ref.y, ref.z);

   return Qnil;
}

VALUE Listener_directionOp(VALUE self, VALUE val)
{
   Vector3f &ref = getRef<Vector3f>(val);
   alListener3f(AL_DIRECTION, ref.x,ref.y, ref.z);

   return Qnil;
}

void defineAudio()
{
   VALUE cVector3f = defClass<Vector3f>("Vector3f");
   defMethod(cVector3f, "x=", Vector3f_setX, 1);
   defMethod(cVector3f, "y=", Vector3f_setY, 1);
   defMethod(cVector3f, "z=", Vector3f_setZ, 1);

   defMethod(cVector3f, "x", Vector3f_x, 0);
   defMethod(cVector3f, "y", Vector3f_y, 0);
   defMethod(cVector3f, "z", Vector3f_z, 0);

   defMethod(cVector3f, "+", Vector3f_add, 1);
   defMethod(cVector3f, "-", Vector3f_sub, 1);
   defMethod(cVector3f, "==", Vector3f_eq, 1);

   VALUE cAudioObject = defClass<AudioObject>("AudioObject");
   defMethod(cAudioObject, "setPos", AudioObject_setPos, 3);
   defMethod(cAudioObject, "setDirection", AudioObject_setDirection, 3);
   defMethod(cAudioObject, "setVelocity", AudioObject_setVelocity, 3);
   defMethod(cAudioObject, "pos=", AudioObject_setPosVector, 1);
   defMethod(cAudioObject, "direction=", AudioObject_setDirectionVector, 1);
   defMethod(cAudioObject, "velocity=", AudioObject_setVelocityVector, 1);

   VALUE cSound = defClass<Sound>("Sound", "AudioObject");
   defMethod(cSound, "loadWav", Sound_loadWav, 1);
   defMethod(cSound, "play", Sound_play, 0);
   defMethod(cSound, "pause", Sound_play, 0);
   defMethod(cSound, "stop", Sound_stop, 0);

   VALUE cStream = defClass<Stream>("Stream", "AudioObject");
   defMethod(cStream, "loadOgg", Stream_loadOgg, 1);
   defMethod(cStream, "play", Stream_play, 0);
   defMethod(cStream, "playing", Stream_playing, 0);
   defMethod(cStream, "update", Stream_update, 0);
   defMethod(cStream, "pause", Stream_play, 0);
   defMethod(cStream, "stop", Stream_stop, 0);

   VALUE mListener = defModule("Listener");
   defModFunc(mListener, "setPos", Listener_setPos, 3);
   defModFunc(mListener, "setDirection", Listener_setDirection, 3);
   defModFunc(mListener, "setVelocity", Listener_setVelocity, 3);
   defModFunc(mListener, "pos=", Listener_posOp, 1);
   defModFunc(mListener, "direction=", Listener_directionOp, 1);
   defModFunc(mListener, "velocity=", Listener_velocityOp, 1);
   defModFunc(mListener, "setOrientation", Listener_setOrientation, 6);

   defFunc("initAudio", Audio_init, 0);
   defFunc("stopAudio", Audio_stop, 0);
}
