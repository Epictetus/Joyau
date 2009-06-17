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

   OSL_SOUND *getStream(char *name);
   OSL_SOUND *getSound(char *name);
protected:
   map<string, OSL_IMAGE*> images;
   map<string, OSL_SOUND*> streams;
   map<string, OSL_SOUND*> sounds;
};

#endif
