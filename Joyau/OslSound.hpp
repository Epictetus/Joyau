#include "StdInclude.hpp"
#include "Manager.hpp"

class OslMusic {
public:
   OslMusic();
   OslMusic(const std::string &title, int stream);

   void load(const std::string &title, int stream);

   void play(int voice);
   void resume();
   void pause();
   void stop();

   int playingOn();
private:
   OSL_SOUND *sound;
};

VALUE OslMusic_init(VALUE self);
VALUE OslMusic_stop_mod(VALUE self);

VALUE OslMusic_load(int argc, VALUE *argv, VALUE self);

VALUE OslMusic_play(VALUE self, VALUE voice);
VALUE OslMusic_resume(VALUE self);
VALUE OslMusic_pause(VALUE self);
VALUE OslMusic_stop(VALUE self);

VALUE OslMusic_playing_on(VALUE self);

void defineOslMusic();
