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
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA. */

#ifndef JOYAU_MANAGER
#define JOYAU_MANAGER

#include "StdInclude.hpp"

/** @addtogroup Misc **/
/*@{*/

/** 
 * @class Singleton
 * A Singleton class has only one instance.
 */
template<typename T> class Singleton
{
public:
   /** Returns the class instance. **/
   static T& getInstance()
   {
      if (singleton == NULL)
	 singleton = new T;
      return (*singleton);
   }

   /** Deletes the class instance. **/
   static void deleteInstance()
   {
      if (singleton != NULL)
      {
	 delete singleton;
	 singleton = NULL;
      }
   }
protected:
   /** class instance **/
   static T *singleton;
};
/*@}*/

template<typename T> T* Singleton<T>::singleton = NULL;

/** @defgroup Memory
 *  Contains classes for memory management.
 */

/** @addtogroup Memory **/
/*@{*/

/** 
 * @class Manager
 * Contains functions which avoid to load the same ressource
 * twice.
 */
class Manager: public Singleton<Manager>
{
   friend Manager &Singleton<Manager>::getInstance();
   friend void Singleton<Manager>::deleteInstance();
   virtual ~Manager();

public:
   /** Asks for a picture, which is loaded if it's not already done.
    *  @param name picture name.
    *  @return a pointer to the loaded ressource.
    */
   OSL_IMAGE *getPic(char *name);

   /** Asks for a font, which is loaded if it's not already done.
    *  @param name font name.
    *  @return a pointer to the loaded ressource.
    */
   OSL_FONT *getFont(const char *name);

   /** Ask for a font, which is loaded with opt if it's not already done.
    */
   intraFont *getIntraFont(const std::string &name, int opt);

   /** Asks for a WAV bufer, which is loaded if it's not already done.
    *  @param name picture name.
    *  @return a pointer to the loaded ressource.
    */
   ALuint getBuffer(const char *name);

   /** Clears loaded images. **/
   void clearImages();

   /** Clears loaded fonts. **/
   void clearFonts();

   /** Clears loaded buffers. **/
   void clearBuffers();

   /** returns argc, given in setArg **/
   int getArgc() const { return _argc; }

   /** returns argv, given in setArg **/
   char **getArgv() { return _argv; }

   /** Sets the argument, as in main. **/
   void setArg(int argc, char** argv);
private:
   std::map<std::string, OSL_IMAGE*> images;
   std::map<std::string, OSL_FONT*> fonts;
   std::map<std::string, intraFont*> intra_fonts;
   std::map<std::string, ALuint> buffers;

   int _argc;
   char **_argv;
};

/*@}*/

VALUE clearImages(VALUE self);
VALUE clearFonts(VALUE self);
VALUE clearBuffers(VALUE self);

void defineManager();

#endif
