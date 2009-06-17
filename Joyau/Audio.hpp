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
