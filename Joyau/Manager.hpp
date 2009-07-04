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
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA. */

#ifndef __MANAGER__
#define __MANAGER__

#include "StdInclude.hpp"

using namespace std;

template<typename T> class Singleton
{
public:
   static T* getInstance()
   {
      if (singleton == NULL)
	 singleton = new T;
      return singleton;
   }

   static void deleteInstance()
   {
      if (singleton != NULL)
      {
	 delete singleton;
	 singleton = NULL;
      }
   }
protected:
   static T *singleton;
};

template<typename T> T* Singleton<T>::singleton = NULL;

class Manager: public Singleton<Manager>
{
   friend Manager *Singleton<Manager>::getInstance();
   friend void Singleton<Manager>::deleteInstance();
   virtual ~Manager();

public:
   
   OSL_IMAGE *getPic(char *name);
   OSL_FONT *getFont(const char *name);
   OSL_SOUND *getStream(char *name);
   OSL_SOUND *getSound(char *name);

   void clearImages();
   void clearStreams();
   void clearSounds();
   void clearFonts();
   void clearParticles();

protected:
   map<string, OSL_IMAGE*> images;
   map<string, OSL_SOUND*> streams;
   map<string, OSL_SOUND*> sounds;
   map<string, OSL_FONT*> fonts;
};

VALUE clearImages(VALUE self);
VALUE clearStreams(VALUE self);
VALUE clearSounds(VALUE self);
VALUE clearFonts(VALUE self);

void defineManager();

#endif
