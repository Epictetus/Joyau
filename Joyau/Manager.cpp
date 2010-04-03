
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

#include "Manager.hpp"

OSL_SOUND *loadSoundFile(const char *filename, int stream) {
   if (!strcmp(filename + strlen(filename) - 4, ".bgm"))
      return oslLoadSoundFileBGM(filename, stream);
   else if (!strcmp(filename + strlen(filename) - 4, ".wav"))
      return oslLoadSoundFileWAV(filename, stream);
   else if (!strcmp(filename + strlen(filename) - 4, ".mp3"))
      return oslLoadSoundFileMP3(filename, stream);
   else if (!strcmp(filename + strlen(filename) - 4, ".at3"))
      return oslLoadSoundFileAT3(filename, stream);
   // else if (!strcmp(filename + strlen(filename) - 4, ".mod"))
   //   return oslLoadSoundFileMOD(filename, stream);
   return NULL;
}

Manager::~Manager()
{
   for (std::map<std::string, OSL_IMAGE*>::iterator i = images.begin(); 
	i != images.end(); ++i) {
      if (i->second != NULL)
	 oslDeleteImage(i->second);
   }
   for (std::map<std::string, OSL_FONT*>::iterator i = fonts.begin(); 
	i != fonts.end(); ++i) {
      if (i->second != NULL)
	 oslDeleteFont(i->second);
   }
   for (std::map<std::string, OSL_FONT*>::iterator i = intra_fonts.begin();
	i != intra_fonts.end(); ++i) {
      if (i->second != NULL)
	 oslDeleteFont(i->second);
   }
   for (std::map<std::string, ALuint>::iterator i = buffers.begin(); 
	i != buffers.end(); ++i)
      alDeleteBuffers(1, &i->second);
   for (std::map<std::string, OSL_SOUND*>::iterator i = musics.begin(); 
	i != musics.end(); ++i)
      oslDeleteSound(i->second);
}

OSL_IMAGE* Manager::getPic(char *name)
{
   if (images.find(name) == images.end())
      images[name] = oslLoadImageFile(name, OSL_IN_RAM, OSL_PF_8888);
   return images[name];
}

OSL_FONT* Manager::getFont(const char *name)
{
   if (fonts.find(name) == fonts.end())
      fonts[name] = oslLoadFontFile(name);
   return fonts[name];
}

OSL_FONT* Manager::getIntraFont(const std::string &name, int opt) {
   if (intra_fonts.find(name) == intra_fonts.end())
      intra_fonts[name] = oslLoadIntraFontFile(name.c_str(), opt);
   return intra_fonts[name];
}

ALuint Manager::getBuffer(const char *name)
{
   if (buffers.find(name) == buffers.end())
      buffers[name] = alutCreateBufferFromFile(name);
   return buffers[name];
}

OSL_SOUND* Manager::getOslMusic(const std::string &filename, int stream) {
   if (musics.find(filename) == musics.end())
      musics[filename] = loadSoundFile(filename.c_str(), stream);
   return musics[filename];
}

// Hey, perhaps there won't be enough memory. We'll let the user choice if he
// wants to free the memory. It's also him which should care about the use of
// images which were freed.
void Manager::clearImages()
{
   for (std::map<std::string, OSL_IMAGE*>::iterator i = images.begin(); 
	i != images.end(); ++i) {
      if (i->second != NULL) // Unless they're null,
	 oslDeleteImage(i->second); // We'll free the ressources
   }   
   // We don't want to give a null pointer to the user
   images.clear();
}

void Manager::clearFonts()
{
   for (std::map<std::string, OSL_FONT*>::iterator i = fonts.begin(); 
	i != fonts.end(); ++i) {
      if (i->second != NULL)
	 oslDeleteFont(i->second);
   }
   for (std::map<std::string, OSL_FONT*>::iterator i = intra_fonts.begin(); 
	i != intra_fonts.end(); ++i) {    
      if (i->second != NULL)
	 oslDeleteFont(i->second);
   }
   fonts.clear();
   intra_fonts.clear();
}

void Manager::clearBuffers()
{
   for (std::map<std::string, ALuint>::iterator i = buffers.begin(); 
	i != buffers.end(); ++i)
      alDeleteBuffers(1, &i->second);
   buffers.clear();

   for (std::map<std::string, OSL_SOUND*>::iterator i = musics.begin(); 
	i != musics.end(); ++i)
      oslDeleteSound(i->second);
   musics.clear();
}

void Manager::setArg(int argc, char** argv)
{
   _argc = argc;
   _argv = argv;
}

/*
  Frees the pictures.
  When manipulating the memory by yourself, be carefull: you have to reload
  the ressource you still want to use (If you don't, a crash will occur).
*/
VALUE clearImages(VALUE self)
{
   Manager &m = Manager::getInstance();
   m.clearImages();
   
   return Qnil;
}

/*
  Frees all the fonts.
*/
VALUE clearFonts(VALUE self)
{
   Manager &m = Manager::getInstance();
   m.clearFonts();
   
   return Qnil;
}

/*
  Frees the sound buffers.
*/
VALUE clearBuffers(VALUE self)
{
   Manager &m = Manager::getInstance();
   m.clearBuffers();

   return Qnil;
}

void defineManager()
{
   VALUE joyau = JOYAU_MOD;
   defModFunc(joyau, "clearImages", clearImages, 0);
   defModFunc(joyau, "clearFonts", clearFonts, 0);
   defModFunc(joyau, "clearSounds", clearBuffers, 0);
}
