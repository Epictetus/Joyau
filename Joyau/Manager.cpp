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
   for (map<string, OSL_IMAGE*>::iterator i = images.begin(); 
	i != images.end(); ++i)
      oslDeleteImage(i->second);
   for (map<string, OSL_FONT*>::iterator i = fonts.begin(); 
	i != fonts.end(); ++i)
      oslDeleteFont(i->second);
   for (map<string, ALuint>::iterator i = buffers.begin(); 
	i != buffers.end(); ++i)
      alDeleteBuffers(1, &i->second);
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

ALuint Manager::getBuffer(const char *name)
{
   if (buffers.find(name) == buffers.end())
      buffers[name] = alutCreateBufferFromFile(name);
   return buffers[name];
}

// Hey, perhaps there won't be enough memory. We'll let the user choice if he
// wants to free the memory. It's also him which should care about the use of
// images which were freed.
void Manager::clearImages()
{
   for (map<string, OSL_IMAGE*>::iterator i = images.begin(); 
	i != images.end(); ++i)
      oslDeleteImage(i->second); // We'll free the ressources
   // We don't want to give a null pointer to the user
   images.clear();
}

void Manager::clearFonts()
{
   for (map<string, OSL_FONT*>::iterator i = fonts.begin(); 
	i != fonts.end(); ++i)
      oslDeleteFont(i->second);
   fonts.clear();
}

void Manager::clearBuffers()
{
   for (map<string, ALuint>::iterator i = buffers.begin(); 
	i != buffers.end(); ++i)
      alDeleteBuffers(1, &i->second);
   buffers.clear();
}

void Manager::setArg(int argc, char** argv)
{
   _argc = argc;
   _argv = argv;
}

VALUE clearImages(VALUE self)
{
   Manager &m = Manager::getInstance();
   m.clearImages();
   
   return Qnil;
}

VALUE clearFonts(VALUE self)
{
   Manager &m = Manager::getInstance();
   m.clearFonts();
   
   return Qnil;
}

VALUE clearBuffers(VALUE self)
{
   Manager &m = Manager::getInstance();
   m.clearBuffers();

   return Qnil;
}

void defineManager()
{
   defFunc("clearImages", clearImages, 0);
   defFunc("clearFonts", clearFonts, 0);
   defFunc("clearSounds", clearBuffers, 0);
}
