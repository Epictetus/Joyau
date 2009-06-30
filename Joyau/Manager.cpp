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

#include "Manager.hpp"

Manager::~Manager()
{
   for (map<string, OSL_SOUND*>::iterator i = streams.begin(); 
	i != streams.end(); ++i)
      oslDeleteSound(i->second);
   for (map<string, OSL_SOUND*>::iterator i = sounds.begin(); 
	i != sounds.end(); ++i)
      oslDeleteSound(i->second);
   for (map<string, OSL_IMAGE*>::iterator i = images.begin(); 
	i != images.end(); ++i)
      oslDeleteImage(i->second);
   for (map<string, OSL_FONT*>::iterator i = fonts.begin(); 
	i != fonts.end(); ++i)
      oslDeleteFont(i->second);
}

OSL_IMAGE* Manager::getPic(char *name)
{
   if (images.find(name) == images.end())
      images[name] = oslLoadImageFile(name, OSL_IN_RAM, OSL_PF_8888);
   return images[name];
}

OSL_FONT* Manager::getFont(const char *name)
{
   if(fonts.find(name) == fonts.end())
      fonts[name] = oslLoadFontFile(name);
   return fonts[name];
}

OSL_SOUND* Manager::getStream(char *name)
{
   if (streams.find(name) == streams.end())
      streams[name] = oslLoadSoundFile(name, OSL_FMT_STREAM);
   return streams[name];
}

OSL_SOUND* Manager::getSound(char *name)
{
   if (sounds.find(name) == sounds.end())
      sounds[name] = oslLoadSoundFile(name, OSL_FMT_NONE);
   return sounds[name];
}

// Hey, perhaps there won't be enough memory. We'll let the user choice if he
// wants to free the memory. It's also him which should care about the use of
// images which were freed.
void Manager::clearImages()
{
   for (map<string, OSL_IMAGE*>::iterator i = images.begin(); 
	i != images.end(); ++i)
      oslDeleteImage(i->second);
}

void Manager::clearStreams()
{
   for (map<string, OSL_SOUND*>::iterator i = streams.begin(); 
	i != streams.end(); ++i)
      oslDeleteSound(i->second);
}
void Manager::clearSounds()
{
   for (map<string, OSL_SOUND*>::iterator i = sounds.begin(); 
	i != sounds.end(); ++i)
      oslDeleteSound(i->second);
}

void Manager::clearFonts()
{
   for (map<string, OSL_FONT*>::iterator i = fonts.begin(); 
	i != fonts.end(); ++i)
      oslDeleteFont(i->second);
}

VALUE clearImages(VALUE self)
{
   Manager *m = Manager::getInstance();
   m->clearImages();
   
   return Qnil;
}

VALUE clearStreams(VALUE self)
{
   Manager *m = Manager::getInstance();
   m->clearStreams();
   
   return Qnil;
}

VALUE clearSounds(VALUE self)
{
   Manager *m = Manager::getInstance();
   m->clearSounds();
   
   return Qnil;
}

VALUE clearFonts(VALUE self)
{
   Manager *m = Manager::getInstance();
   m->clearFonts();
   
   return Qnil;
}

void defineManager()
{
   rb_define_global_function("clearImages", (VALUE(*)(...))&clearImages, 0);
   rb_define_global_function("clearSounds", (VALUE(*)(...))&clearSounds, 0);
   rb_define_global_function("clearStreams", (VALUE(*)(...))&clearStreams, 0);
   rb_define_global_function("clearFonts", (VALUE(*)(...))&clearFonts, 0);
}
