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

/** @defgroup Audio
 *  Contains classes related to the audio module. There are streams,
 *  short sounds, vectors for their position, ...
 */

/** @addtogroup Audio **/
/*@{*/

/**
 * @class Vector3f
 * A class which represents a position, or an offset,
 * using three float numbers for its localisation. It's usually used 
 * in audio classes. AudioObject implements some methods using Vector3f.
 */
struct Vector3f
{
   /** Returns a new Vector3f. 
    *  @code
    *  Sound obj;
    *  obj.setPos(Vector3f(0.f, 3.5f));
    *  @endcode
    *  @param argX orginal x position
    *  @param argY original y position
    *  @param argZ original z position
    */
   Vector3f(float argX = 0.f, float argY = 0.f, float argZ = 0.f):
      x(argX), y(argY), z(argZ)
   {}
   
   
   /** Returns a Vector3f vector, whose x, y, and z positions are the sum
    *  of the position of this and op.
    *  @code
    *  Vector3f vector1(0.f, 2.f, 5.f);
    *  Vector3f vector2(0.f, 7.f, -9.f);
    *  Vector3f vector3 = vector1 + vector2;
    *  // vector3.x = 0.f, vector3.y = 9.f, vector3.z = -4.f
    *  @endcode
    *  @param op a constant reference to the Vector3f which should be added.
    */
   Vector3f operator+(const Vector3f &op) const;
   
   /** Returns a Vector3f vector, whose x, y, and z positions are the result
    *  of our position - op's position.
    *  @code
    *  Vector3f vector1(0.f, 2.f, 5.f);
    *  Vector3f vector2(0.f, 7.f, -9.f);
    *  Vector3f vector3 = vector1 - vector2;
    *  // vector3.x = 0.f, vector3.y = -5.f, vector3.z = 14.f
    *  @endcode
    *  @param op a constant reference to the Vector3f which should be 
    *  substracted.
    */
   Vector3f operator-(const Vector3f &op) const;

   /** Returns whether our position is the same as op's position.
    *  @code
    *  Vector3f vector1(0.f, 1.f, 1.f);
    *  Vector3f vector2(0.f, 1.f, 1.f);
    *  bool val = vector1 == vector2; // true
    *  vector1 = vector1 + vector2;
    *  val = vector1 == vector2; // false
    *  @endcode
    *  @param op a constant reference to the Vector3f which should be 
    *  compared to.
    */
   bool operator==(const Vector3f &op) const;

   /** x position of the Vector **/
   float x;

   /** y position of the Vector **/
   float y;
   
   /** z position of the Vector **/
   float z; 
};

/**
 * @class AudioObject
 * A class which represents an AudiObject, which can be moved in
 * the space.
 */
class AudioObject
{
public:
   /** changes the object's position.
    *  @param x new x position
    *  @param y new y position
    *  @param z new z position
    */
   void setPos(float x, float y, float z);

   /** changes the object's velocity.
    *  @param x new x position
    *  @param y new y position
    *  @param z new z position
    */
   void setVelocity(float x, float y, float z);

   /** changes the object's direction.
    *  @param x new x position
    *  @param y new y position
    *  @param z new z position
    */
   void setDirection(float x, float y, float z);

   /** Changes the position with a Vector3f. Provided for conveniance.
    *  @param vector the new position.
    */
   void setPos(const Vector3f &vector);

    /** Changes the velocity with a Vector3f. Provided for conveniance.
     *  @param vector the new velocity.
     */
   void setVelocity(const Vector3f &vector);

   /** Changes the velocity with a Vector3f. Provided for conveniance.
    *  @param vector the new direction..
    */
   void setDirection(const Vector3f &vector);

   /** Sets the object's pitch **/
   void setPitch(float val);

   /** Sets the object's gain **/
   void setGain(float val);

   /** Returns whether we're playing. **/
   bool playing() const;
protected:
   /** Generates a source. **/
   void generateSource();

   /** Deletes the source **/
   void deleteSource();

   /** Returns the queued buffers number **/
   int queuedBuffers();

   /** Returns the processed buffers number **/
   int processedBuffers();

   /** Unqueue some buffers
    *  @param n Tells how many buffers are used.
    *  @param buffers array where buffers are set.
    */
   void unqueueBuffers(int n, ALuint *buffers);
   
   /** Queue some buffers
    *  @param n Tells how many buffers are used.
    *  @param buffers array where buffers are set.
    */
   void queueBuffers(int n, ALuint *buffers);

   /** Sets the sound's buffer **/
   void setBuffer(ALuint buffer);

   /** plays the source **/
   void playSource();

   /** pauses the source **/
   void pauseSource();

   /** Stops the source **/
   void stopSource();
private:
   /** Source, from where the sound is emited. **/
   ALuint source;
};

/**
 * @class Sound
 * A class which represents a short sound. The used format is
 * wav.
 */
class Sound: public AudioObject
{
public:
   Sound() {}
   Sound(const Sound &obj);
   virtual ~Sound();

   /** Loads a wav sound.
    *  @param filename sound's filename.
    */
   bool loadWav(const char *filename);

   /** plays the sound **/
   void play();

   /** pauses it **/
   void pause();

   /** stops it **/
   void stop();
private:
   std::string sound;
   ALuint buffer;
};

/**
 * @class Stream
 * @description A class which represents an ogg stream.
 */
class Stream: public AudioObject
{
public:
   Stream() {}
   Stream(const Stream &obj);
   virtual ~Stream();

   /** Loads a wav stream.
    *  @param filename stream's filename.
    */
   bool loadOgg(const char *filename);

   /** Plays the sound.
    *  @return true if it works.
    */
   bool play();

   /** Pauses the sound. **/
   void pause();

   /** Stops the sound. **/
   void stop();

   /** Updates the stream. Should be called as often as possible.
    *  @code
    *  Stream stream;
    *  stream.loadOgg("file.ogg");
    *  while (stream.update() && stream.play()) {}
    *  @endcode
    *  @return true if it works.
    */
   bool update();
private:
   std::string sound;

   bool streamBuf(ALuint buffer);
   void clear();

   FILE *file;

   OggVorbis_File stream;
   vorbis_info *info;
   vorbis_comment *comment;
   
   ALuint buffers[2];
   ALenum format;
};

/** Inits OpenAL and alut. Should be called before using any audio
 *  functions.
 */
void initOpenAl();

/** Stops OpenAL and alut. **/
void stopOpenAL();

/*@}*/

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
