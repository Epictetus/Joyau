#include "Manager.hpp"

Manager::~Manager()
{
   for (map<string, OSL_SOUND*>::iterator i = streams.begin(); 
	i != streams.end(); ++i)
      oslDeleteSound(i->second);
   for (map<string, OSL_SOUND*>::iterator i = sounds.begin(); 
	i != sounds.end(); ++i)
      oslDeleteSound(i->second);
}

OSL_IMAGE* Manager::getPic(char *name)
{
   if (images.find(name) == images.end())
   {
      images[name] = oslLoadImageFile(name, OSL_IN_RAM, OSL_PF_8888);
   }
   return images[name];
}

OSL_SOUND* Manager::getStream(char *name)
{
   if (streams.find(name) == streams.end())
   {
      streams[name] = oslLoadSoundFile(name, OSL_FMT_STREAM);
   }
   return streams[name];
}

OSL_SOUND* Manager::getSound(char *name)
{
   if (sounds.find(name) == sounds.end())
   {
      sounds[name] = oslLoadSoundFile(name, OSL_FMT_NONE);
   }
   return sounds[name];
}


