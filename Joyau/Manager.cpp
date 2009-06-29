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



