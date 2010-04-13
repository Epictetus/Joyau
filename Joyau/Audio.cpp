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

/*
  Document-class: Joyau::AudioObject

  This class is not here to be instancied, but rather for generic method
  used on +Sound+ as well as on +Stream+.
*/

/*
  Document-class: Joyau::Listener

  This is only used when setting the listner's position, just like you can set
  the others objects position.
*/

/*
  Document-class: Joyau::Sound

  Joyau's class used for short sounds. It allows to load WAV files,
  and to play them at each call of play.
*/

/*
  call-seq: Joyau::Sound.new
            Joyau::Sound.new(filename)

    Creates a new Sound. If a filename is given, it is loaded.
    Examples :
      sound = Joyau::Sound.new
      sound = Joyau::Sound.new("music.wav")
*/
VALUE Sound_initialize(int argc, VALUE *argv, VALUE self)
{
   Sound &ref = getRef<Sound>(self);
   
   VALUE filename;
   rb_scan_args(argc, argv, "01", &filename);

   if (!NIL_P(filename)) {
      try {
	 filename = rb_obj_as_string(filename);
	 ref.loadWav(StringValuePtr(filename));
      }
      catch (const RubyException &e) {
	 e.rbRaise();
      }
   }
   
   return Qnil;
}

/*
  Document-class: Joyau::Stream

  Joyau's class used for rather long music. It loads an OGG file, and plays it
  as a stream.

  Its play method has to be called as often as possible, just like update.
  These methods return true while the stream is playing, so:
    while stream.update and stream.play
  Will loop while there is something to play.
*/


/*
  call-seq: Joyau::Stream.new
            Joyau::Stream.new(filename)

    Creates a new Stream. If a filename is given, it is loaded.
    Examples :
      stream = Joyau::Stream.new
      stream = Joyau::Stream.new("music.ogg")
*/
VALUE Stream_initialize(int argc, VALUE *argv, VALUE self)
{
   Stream &ref = getRef<Stream>(self);

   VALUE filename;
   rb_scan_args(argc, argv, "01", &filename);

   if (!NIL_P(filename)) {
      try {
	 filename = rb_obj_as_string(filename);
	 ref.loadOgg(StringValuePtr(filename));
      }
      catch (const RubyException &e) {
	 e.rbRaise();
      }
   }

   return Qnil;
}

/*
  Document-class: Joyau::Vector3f

  audio objects and listeners are somewhere in the world. This class is mainly
  used for their positions.
*/

/*
  call-seq: Joyau::Vector3f.new
            Joyau::Vector3f.new(x, y, z)

    Creates a new Vector3f. Its position can be set too.
    Examples :
      vector = Joyau::Vector3f.new
      vector = Joyau::Vector3f.new(10, 15, 5)
*/
VALUE Vector3f_initialize(int argc, VALUE *argv, VALUE self)
{
   Vector3f &ref = getRef<Vector3f>(self);

   VALUE x, y, z;
   rb_scan_args(argc, argv, "03", &x, &y, &z);
   
   if (!NIL_P(x)) {
      if (NIL_P(y) || NIL_P(z))
	 rb_raise(rb_eArgError, "3 arguments expected if at least on is given.");
      
      ref.x = NUM2DBL(x);
      ref.y = NUM2DBL(y);
      ref.z = NUM2DBL(z);
   }

   return Qnil;
}


Sound::Sound(const Sound &obj)
{
   loadWav(obj.sound.c_str());
}

Sound::~Sound()
{
   deleteSource();
}

void Sound::loadWav(const char *filename)
{
   sound = filename;

   buffer = Manager::getInstance().getBuffer(filename);

   // We create a source, binded to our buffer :
   generateSource();
   if (alGetError() != AL_NO_ERROR)
      throw RubyException(rb_eRuntimeError, "Cannot generator source.");

   setBuffer(buffer);

   setPos(0.f, 0.f, 0.f);
   setVelocity(0.f, 0.f, 0.f);
   setDirection(0.f, 0.f, 0.f);
   
   if (alGetError() != AL_NO_ERROR)
      throw RubyException(rb_eRuntimeError, "Cannot set buffer.");
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

void Stream::loadOgg(const char *filename)
{
   sound = filename;

   file = fopen(filename, "rb");
   if (!file)
      throw RubyException(rb_eRuntimeError, "Cannot open the stream file.");
   ov_open(file, &stream, NULL, 0);

   info = ov_info(&stream, -1);
   comment = ov_comment(&stream, -1);

   format = SAMPLES_FORMAT;
   
   alGenBuffers(2, buffers);
   if (alGetError() != AL_NO_ERROR)
      throw RubyException(rb_eRuntimeError, "Cannot generate the buffer.");

   generateSource();
   if (alGetError() != AL_NO_ERROR)
     throw RubyException(rb_eRuntimeError, "Cannot generate sources.");

   setPos(0.f, 0.f, 0.f);
   setVelocity(0.f, 0.f, 0.f);
   setDirection(0.f, 0.f, 0.f);

   setPitch(1.f);
   setGain(1.f);
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

/*
  call-seq: initAudio

  Inits the audio module from Joyau, which allows to use classes like
  +Joyau::Stream+ and +Joyau::Audio+.

  See +Joyau::stopAudio+ in order to stop it.
*/
VALUE Audio_init(VALUE self)
{
   initOpenAl();
   return Qnil;
}

/*
  call-seq: stopAudio

  Stops the audio module from Joyau. Should be called at the end of 
  your program if you use it.
*/
VALUE Audio_stop(VALUE self)
{
   stopOpenAL();
   return Qnil;
}

/*
  call-seq: x=(value)

  Sets a vector's x value to a new value.
*/
VALUE Vector3f_setX(VALUE self, VALUE val)
{
   Vector3f &ref = getRef<Vector3f>(self);
   ref.x = NUM2DBL(val);

   return val;
}

/*
  call-seq: y=(value)

  Sets a vector's y value to a new value.
*/
VALUE Vector3f_setY(VALUE self, VALUE val)
{
   Vector3f &ref = getRef<Vector3f>(self);
   ref.y = NUM2DBL(val);

   return val;
}

/*
  call-seq: z=(value)

  Sets a vector's z value to a new value.
*/
VALUE Vector3f_setZ(VALUE self, VALUE val)
{
   Vector3f &ref = getRef<Vector3f>(self);
   ref.z = NUM2DBL(val);

   return val;
}

/*
  Returns a vector's x value.
*/
VALUE Vector3f_x(VALUE self)
{
   Vector3f &ref = getRef<Vector3f>(self);
   return rb_float_new(ref.x);
}

/*
  Returns a vector's y value.
*/
VALUE Vector3f_y(VALUE self)
{
   Vector3f &ref = getRef<Vector3f>(self);
   return rb_float_new(ref.y);
}

/*
  Returns a vector's z value.
*/
VALUE Vector3f_z(VALUE self)
{
   Vector3f &ref = getRef<Vector3f>(self);
   return rb_float_new(ref.z);
}

/*
  call-seq: vector + vector2

  Returns a new vector:
    Vector3f.new(10, 15, 5) + Vector3f.new(15, 30, 2.5) #=> Vector3f(25, 45, 7.5)
*/
VALUE Vector3f_add(VALUE self, VALUE op)
{
   if (!rb_obj_is_kind_of(op, getClass("Vector3f")))
      rb_raise(rb_eTypeError, "Can't convert %s into Joyau::Vector3f",
	       rb_obj_classname(op));

   Vector3f &first = getRef<Vector3f>(self);
   Vector3f &second = getRef<Vector3f>(op);

   Vector3f ret = first + second;
   return createObject(getClass("Vector3f"), ret);
}

/*
  call-seq: vector - vector2

  Returns a new vector:
    Vector3f.new(10, 15, 5) - Vector3f.new(15, 30, 2.5) #=>Vector3f(-5, -15, 2.5)
*/
VALUE Vector3f_sub(VALUE self, VALUE op)
{
   if (!rb_obj_is_kind_of(op, getClass("Vector3f")))
      rb_raise(rb_eTypeError, "Can't convert %s into Joyau::Vector3f",
	       rb_obj_classname(op));

   Vector3f &first = getRef<Vector3f>(self);
   Vector3f &second = getRef<Vector3f>(op);

   Vector3f ret = first - second;
   return createObject(getClass("Vector3f"), ret);
}

/*
  call-seq: vector == vector2

  Returns true when the vectors have the same x, y, and z values.
*/
VALUE Vector3f_eq(VALUE self, VALUE op)
{
   if (!rb_obj_is_kind_of(op, getClass("Vector3f")))
      rb_raise(rb_eTypeError, "Can't convert %s into Joyau::Vector3f",
	       rb_obj_classname(op));
   
   Vector3f &first = getRef<Vector3f>(self);
   Vector3f &second = getRef<Vector3f>(op);

   return first == second ? Qtrue : Qfalse;
}

/*
  call-seq: setPos(x, y, z)

  Sets an audio object's position.
*/
VALUE AudioObject_setPos(VALUE self, VALUE x, VALUE y, VALUE z)
{
   AudioObject &ref = getRef<AudioObject>(self);
   double _x = NUM2DBL(x);
   double _y = NUM2DBL(y);
   double _z = NUM2DBL(z);

   ref.setPos(_x, _y, _z);

   return Qnil;
}

/*
  call-seq: setVelocity(x, y, z)

  Sets an audio object's velocity.
*/
VALUE AudioObject_setVelocity(VALUE self, VALUE x, VALUE y, VALUE z)
{
   AudioObject &ref = getRef<AudioObject>(self);
   double _x = NUM2DBL(x);
   double _y = NUM2DBL(y);
   double _z = NUM2DBL(z);

   ref.setVelocity(_x, _y, _z);

   return Qnil;
}

/*
  call-seq:
    object.setDirection(x, y, z) -> nil

  Sets an audio object's direction.
*/
VALUE AudioObject_setDirection(VALUE self, VALUE x, VALUE y, VALUE z)
{
   AudioObject &ref = getRef<AudioObject>(self);
   double _x = NUM2DBL(x);
   double _y = NUM2DBL(y);
   double _z = NUM2DBL(z);

   ref.setDirection(_x, _y, _z);

   return Qnil;
}

/*
  call-seq:
    object.direction=(vector) -> Vector3f

  Sets an audio object's position.
*/
VALUE AudioObject_setPosVector(VALUE self, VALUE val)
{
   if (!rb_obj_is_kind_of(val, getClass("Vector3f")))
      rb_raise(rb_eTypeError, "Can't convert %s into Joyau::Vector3f",
	       rb_obj_classname(val));

   AudioObject &ref = getRef<AudioObject>(self);
   Vector3f &vector = getRef<Vector3f>(val);

   ref.setPos(vector);
   return val;
}

/*
  call-seq: velocity=(vector)

  Sets an audio object's velocity.
*/
VALUE AudioObject_setVelocityVector(VALUE self, VALUE val)
{
   if (!rb_obj_is_kind_of(val, getClass("Vector3f")))
      rb_raise(rb_eTypeError, "Can't convert %s into Joyau::Vector3f",
	       rb_obj_classname(val));

   AudioObject &ref = getRef<AudioObject>(self);
   Vector3f &vector = getRef<Vector3f>(val);

   ref.setVelocity(vector);
   return val;
}

/*
  call-seq: direction=(vector)

  Sets an audio object's direction.
*/
VALUE AudioObject_setDirectionVector(VALUE self, VALUE val)
{
   if (!rb_obj_is_kind_of(val, getClass("Vector3f")))
      rb_raise(rb_eTypeError, "Can't convert %s into Joyau::Vector3f",
	       rb_obj_classname(val));

   AudioObject &ref = getRef<AudioObject>(self);
   Vector3f &vector = getRef<Vector3f>(val);

   ref.setDirection(vector);
   return val;
}

/*
  call-seq: object.playing?

  Returns whether the object is playing.
*/
VALUE AudioObject_playing(VALUE self)
{
   AudioObject &ref = getRef<AudioObject>(self);
   if (ref.playing())
      return Qtrue;
   return Qfalse;
}

/*
  call-seq: loadWav(file_name)

  Loads a new WAV file.
  Raises a +RuntimeError+ if something wrong occurs.
*/
VALUE Sound_loadWav(VALUE self, VALUE filename)
{
   filename = rb_obj_as_string(filename);

   Sound &ref = getRef<Sound>(self);
   char *str = StringValuePtr(filename);

   try {
      ref.loadWav(str);
   }
   catch (const RubyException &e) {
      e.rbRaise();
      return Qfalse;
   }
   return Qtrue;
}

/*
  Plays a sound. Call it whenever you want it to be played.
*/
VALUE Sound_play(VALUE self)
{
   Sound &ref = getRef<Sound>(self);
   ref.play();
   return Qnil;
}

/*
  Pauses a sound.
*/
VALUE Sound_pause(VALUE self)
{
   Sound &ref = getRef<Sound>(self);
   ref.pause();

   return Qnil;
}

/*
  Stops a sound.
*/
VALUE Sound_stop(VALUE self)
{
   Sound &ref = getRef<Sound>(self);
   ref.stop();

   return Qnil;
}

/*
  call-seq: stream.loadOgg(filename)

  Loads a stream.
  Raises a +RuntimeError+ when something wrong occurs.
*/
VALUE Stream_loadOgg(VALUE self, VALUE filename)
{
   filename = rb_obj_as_string(filename);
   
   Stream &ref = getRef<Stream>(self);
   char *str = StringValuePtr(filename);

   try  {
      ref.loadOgg(str);
   }
   catch (const RubyException &e) {
      e.rbRaise();
      return Qfalse;
   }
   return Qtrue;
}

/*
  call-seq: stream.play -> true or false

  Plays a stream. Call it as often as possible (i.e. one time per loop).
*/
VALUE Stream_play(VALUE self)
{
   Stream &ref = getRef<Stream>(self);
   if (ref.play())
      return Qtrue;
   return Qfalse;
}

/*
  Pauses a stream.
*/
VALUE Stream_pause(VALUE self)
{
   Stream &ref = getRef<Stream>(self);
   ref.pause();

   return Qnil;
}

/*
  Stops a stream.
*/
VALUE Stream_stop(VALUE self)
{
   Stream &ref = getRef<Stream>(self);
   ref.stop();

   return Qnil;
}

/*
  call-seq: update -> true or false

  Call it one time per stream.play, just before it.
*/
VALUE Stream_update(VALUE self)
{
   Stream &ref = getRef<Stream>(self);
   if (ref.update())
      return Qtrue;
   return Qfalse;
}

/*
  call-seq: setPos(x, y, z)

  Sets the audio listener's position.
*/
VALUE Listener_setPos(VALUE self, VALUE x, VALUE y, VALUE z)
{
   double _x = NUM2DBL(x);
   double _y = NUM2DBL(y);
   double _z = NUM2DBL(z);

   alListener3f(AL_POSITION, _x, _y, _z);
   return Qnil;
}

/*
  call-seq: setVelocity(x, y, z)

  Sets the audio listener's velocity.
*/
VALUE Listener_setVelocity(VALUE self, VALUE x, VALUE y, VALUE z)
{
   double _x = NUM2DBL(x);
   double _y = NUM2DBL(y);
   double _z = NUM2DBL(z);

   alListener3f(AL_VELOCITY, _x, _y, _z);
   return Qnil;
}

/*
  call-seq: setDirection(x, y, z)

  Sets the audio listener's direction.
*/
VALUE Listener_setDirection(VALUE self, VALUE x, VALUE y, VALUE z)
{
   double _x = NUM2DBL(x);
   double _y = NUM2DBL(y);
   double _z = NUM2DBL(z);

   alListener3f(AL_DIRECTION, _x, _y, _z);
   return Qnil;
}

/*
  call-seq: setOrientation(atX, atY, at2, upX, upY, upZ) 

  Sets the audio listener's orientation.
*/
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

/*
  call-seq:
    Joyau::Listener.pos=(vector) -> Vector3f

  Sets the audio listener's position.
*/
VALUE Listener_posOp(VALUE self, VALUE val)
{
   if (!rb_obj_is_kind_of(val, getClass("Vector3f")))
      rb_raise(rb_eTypeError, "Can't convert %s into Joyau::Vector3f",
	       rb_obj_classname(val));

   Vector3f &ref = getRef<Vector3f>(val);
   alListener3f(AL_POSITION, ref.x,ref.y, ref.z);

   return val;
}

/*
  call-seq:
    Joyau::Listener.velocity=(vector) -> Vector3f

  Sets the audio listener's velocity.
*/
VALUE Listener_velocityOp(VALUE self, VALUE val)
{
   if (!rb_obj_is_kind_of(val, getClass("Vector3f")))
      rb_raise(rb_eTypeError, "Can't convert %s into Joyau::Vector3f",
	       rb_obj_classname(val));

   Vector3f &ref = getRef<Vector3f>(val);
   alListener3f(AL_VELOCITY, ref.x,ref.y, ref.z);

   return val;
}

/*
  call-seq: direction=(vector)

  Sets the audio listener's direction.
*/
VALUE Listener_directionOp(VALUE self, VALUE val)
{
   if (!rb_obj_is_kind_of(val, getClass("Vector3f")))
      rb_raise(rb_eTypeError, "Can't convert %s into Joyau::Vector3f",
	       rb_obj_classname(val));
   
   Vector3f &ref = getRef<Vector3f>(val);
   alListener3f(AL_DIRECTION, ref.x, ref.y, ref.z);

   return val;
}

void defineAudio()
{
   VALUE cVector3f = defClass<Vector3f>("Vector3f");
   
   defMethod(cVector3f, "initialize", Vector3f_initialize, -1);
   
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

   defMethod(cAudioObject, "playing?", AudioObject_playing, 0);

   VALUE cSound = defClass<Sound>("Sound", "AudioObject");
   defMethod(cSound, "initialize", Sound_initialize, -1);

   defMethod(cSound, "loadWav", Sound_loadWav, 1);
   defMethod(cSound, "play", Sound_play, 0);
   defMethod(cSound, "pause", Sound_play, 0);
   defMethod(cSound, "stop", Sound_stop, 0);

   defAlias(cSound, "loadWav", "load_wav");

   VALUE cStream = defClass<Stream>("Stream", "AudioObject");
   defMethod(cStream, "initialize", Stream_initialize, -1);

   defMethod(cStream, "loadOgg", Stream_loadOgg, 1);
   defMethod(cStream, "play", Stream_play, 0);
   defMethod(cStream, "update", Stream_update, 0);
   defMethod(cStream, "pause", Stream_play, 0);
   defMethod(cStream, "stop", Stream_stop, 0);

   defAlias(cStream, "loadOgg", "load_ogg");

   VALUE mListener = defModule("Listener");
   defModFunc(mListener, "setPos", Listener_setPos, 3);
   defModFunc(mListener, "setDirection", Listener_setDirection, 3);
   defModFunc(mListener, "setVelocity", Listener_setVelocity, 3);
   defModFunc(mListener, "pos=", Listener_posOp, 1);
   defModFunc(mListener, "direction=", Listener_directionOp, 1);
   defModFunc(mListener, "velocity=", Listener_velocityOp, 1);
   defModFunc(mListener, "setOrientation", Listener_setOrientation, 6);

   VALUE joyau = JOYAU_MOD;
   defModFunc(joyau, "initAudio", Audio_init, 0);
   defModFunc(joyau, "stopAudio", Audio_stop, 0);
}
