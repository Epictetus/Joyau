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

#include "Particles.hpp"

/* Another bit of devslib code */

int DevssysRand(int max)
{
   return (int)rand() % max;
}

PARTICULE *_oslLoadParticles(char *Fichier)
{
   PARTICULE *Part;
   OSL_IMAGE *Image;

   Part = (PARTICULE *) malloc(sizeof(PARTICULE));
   if (!Part) {
      if (Part)
	 free(Part);
      return NULL;
   }
   memset(Part, 0, sizeof(PARTICULE));

   Image = oslLoadImageFile(Fichier, OSL_IN_RAM, OSL_PF_5551);

   Part->Images[0] = oslLoadImageFile(Fichier, OSL_IN_RAM, OSL_PF_5551);
   oslSetImageTileSize(Part->Images[0], 0 * (Part->Images[0]->stretchX / 4), 0,
		       (Part->Images[0]->stretchX / 4),
		       Part->Images[0]->stretchY);
   Part->Images[1] = oslLoadImageFile(Fichier, OSL_IN_RAM, OSL_PF_5551);
   oslSetImageTileSize(Part->Images[1], 1 * (Part->Images[1]->stretchX / 4), 0,
		       (Part->Images[1]->stretchX / 4),
		       Part->Images[1]->stretchY);
   Part->Images[2] = oslLoadImageFile(Fichier, OSL_IN_RAM, OSL_PF_5551);
   oslSetImageTileSize(Part->Images[2], 2 * (Part->Images[2]->stretchX / 4), 0,
		       (Part->Images[2]->stretchX / 4),
		       Part->Images[2]->stretchY);
   Part->Images[3] = oslLoadImageFile(Fichier, OSL_IN_RAM, OSL_PF_5551);
   oslSetImageTileSize(Part->Images[3], 3 * (Part->Images[3]->stretchX / 4), 0,
		       (Part->Images[3]->stretchX / 4),
		       Part->Images[3]->stretchY);

   oslDeleteImage(Image);

   return Part;
}

void _oslInitParticles(PARTICULE * Part, int duree, int vitesse,
		       int Graviter, int vitessemini)
{
   int j;

   for (j = 0; j < MaxParticules; ++j) {
      Part->particle[j].status = 0;
   }
   Part->Graviter = Graviter;
   Part->vie = duree;
   Part->Vitesse_Mini = vitessemini;
   Part->Vitesse = vitesse;
}

void _oslNewParticles(PARTICULE * Part, int x, int y)
{
   int i;
   int j;

   for (j = 0; j < MaxParticules; ++j) {
      if (Part->particle[j].status <= 0) {
	 break;
      }
   }
   j %= MaxParticules;
   Part->particle[j].status = Part->vie;
   for (i = 0; i < cGfxParticleSet; ++i) {
      Part->particle[j].x[i] = x;
      Part->particle[j].y[i] = y;

      Part->particle[j].dx[i] =
	  (DevssysRand(Part->Vitesse_Mini) - Part->Vitesse_Mini / 2);
      Part->particle[j].dy[i] = DevssysRand(Part->Vitesse >> 1) - Part->Vitesse;
   }
}

void _oslMoveXParticles(PARTICULE * Part, int x)
{
   int j;
   int i;
   for (j = 0; j < MaxParticules; ++j) {
      for (i = 0; i < cGfxParticleSet; ++i) {
	 if ((x > 0 && Part->particle[j].sx[i] < 25)
	     || (x < 0 && Part->particle[j].sx[i] > -25))
	    Part->particle[j].sx[i] += x;
      }
   }

}

void _oslMoveYParticles(PARTICULE * Part, int y)
{

   int j;
   int i;
   for (j = 0; j < MaxParticules; ++j) {
      for (i = 0; i < cGfxParticleSet; ++i) {
	 if ((y > 0 && Part->particle[j].sy[i] < 25)
	     || (y < 0 && Part->particle[j].sy[i] > -25))
	    Part->particle[j].sy[i] += y;
      }
   }

}

void _oslUpdateParticles(PARTICULE * Part)
{
   int i;
   int j;

   for (j = 0; j < MaxParticules; ++j) {
      if (Part->particle[j].status > 0) {
	 Part->particle[j].status--;
	 for (i = 0; i < cGfxParticleSet; ++i) {
	    Part->particle[j].x[i] +=
		Part->particle[j].dx[i] + Part->particle[j].sx[i];
	    Part->particle[j].y[i] +=
		Part->particle[j].dy[i] + Part->particle[j].sy[i];
	    Part->particle[j].dy[i] += Part->Graviter;
	 }
      }
   }
}

void _oslRenderParticles(PARTICULE * Part)
{
   int i;
   int j;
   int index;

   for (j = 0; j < MaxParticules; ++j) {
      if (Part->particle[j].status > 0) {
	 for (i = 0; i < cGfxParticleSet; ++i) {
	    index = DevssysRand(cGfxParticleState);

	    Part->Images[index]->x =
		Part->particle[j].x[i] - (Part->Images[index]->stretchX >> 1);
	    Part->Images[index]->y =
		Part->particle[j].y[i] - (Part->Images[index]->stretchY >> 1);

	    oslDrawImage(Part->Images[index]);
	 }
      }
   }

}

/*
  Document-class: Joyau::Particles
  
  Class representing a particles generator. The steps of a particle's life are
  saved in a picture.
*/

/*
  call-seq: new
            new(filename)

  Creates a new particles generator.
 */
VALUE Particles_initialize(int argc, VALUE * argv, VALUE self) {
   Particles *ptr = getPtr<Particles>(self);

   if (argc >= 1)
      ptr->setFile(StringValuePtr(argv[0]));

   return Qnil;
}

Particles::Particles(const Particles & obj)
{
   setClass("Particles");

   _time = obj._time;
   _speed = obj._speed;
   _gravity = obj._gravity;
   _mspeed = obj._mspeed;
   setParam(_time, _speed, _gravity, _mspeed);

   char txt[256];
   strcpy(txt, obj.filename.c_str());

   setFile(txt);
}

void Particles::setFile(char *file)	// We can't use manager for that.
{
   filename = file;
   part = _oslLoadParticles(file);
}

void Particles::setParam(int time, int speed, int gravity, int mspeed)
{
   _time = time;
   _speed = speed;
   _gravity = gravity;
   _mspeed = mspeed;
   _oslInitParticles(part, _time, _speed, _gravity, _mspeed);
}

void Particles::move(int x, int y)
{
   Drawable::move(x, y);
   _oslMoveXParticles(part, x);
   _oslMoveYParticles(part, y);
}

void Particles::addParticles(int x, int y)
{
   _oslNewParticles(part, x, y);
}

void Particles::draw()
{
   _oslUpdateParticles(part);
   _oslRenderParticles(part);
}

/*
  call-seq: setFile(filename)

  Sets the particle's file (from which particles are loaded)
*/
VALUE Particles_setFile(VALUE self, VALUE str)
{
   Particles & ref = getRef < Particles > (self);
   char *val = StringValuePtr(str);

   ref.setFile(val);
   return Qnil;
}

/*
  call-seq: setParam(time, speed, gravity, minimum_speed)

  Sets the particles generator's parameters. Can only be called once. 
*/
VALUE Particles_setParam(VALUE self, VALUE time, VALUE speed, VALUE gravity,
			 VALUE mspeed)
{
   Particles & ref = getRef < Particles > (self);
   int _time = FIX2INT(time);
   int _speed = FIX2INT(speed);
   int _gravity = FIX2INT(gravity);
   int _mspeed = FIX2INT(mspeed);

   ref.setParam(_time, _speed, _gravity, _mspeed);
   return Qnil;
}

/*
  call-seq: addParticles(x, y)

  Adds a particle at the specified position.
*/
VALUE Particles_addParticles(VALUE self, VALUE x, VALUE y)
{
   Particles & ref = getRef < Particles > (self);
   int _x = FIX2INT(x);
   int _y = FIX2INT(y);

   ref.addParticles(_x, _y);
   return Qnil;
}

/*
  Returns a particle's lifetime.
*/
VALUE Particles_getTime(VALUE self)
{
   Particles & ref = getRef < Particles > (self);
   return FIX2INT(ref.getTime());
}

/*
  Returns a particle's speed.
*/
VALUE Particles_getSpeed(VALUE self)
{
   Particles & ref = getRef < Particles > (self);
   return FIX2INT(ref.getSpeed());
}

/*
  Returns the particles generator's gravity.
*/
VALUE Particles_getGravity(VALUE self)
{
   Particles & ref = getRef < Particles > (self);
   return FIX2INT(ref.getGravity());
}

/*
  Returns a particle's minimum speed.
*/
VALUE Particles_getMinSpeed(VALUE self)
{
   Particles & ref = getRef < Particles > (self);
   return FIX2INT(ref.getMinSpeed());
}

void defineParticles()
{
   VALUE cPart = defClass<Particles>("Particles", "Drawable");
   defMethod(cPart, "setFile", Particles_setFile, 1);
   defMethod(cPart, "setParam", Particles_setParam, 4);
   defMethod(cPart, "addParticles", Particles_addParticles, 2);
   defMethod(cPart, "getTime", Particles_getTime, 0);
   defMethod(cPart, "getSpeed", Particles_getSpeed, 0);
   defMethod(cPart, "getGravity", Particles_getGravity, 0);
   defMethod(cPart, "getMinSpeed", Particles_getMinSpeed, 0);

   defAlias(cPart, "setFile", "file=");
   defAlias(cPart, "getTime", "time");
   defAlias(cPart, "getSpeed", "speed");
   defAlias(cPart, "getGravity", "gravity");
   defAlias(cPart, "getMinSpeed", "minSpeed");
}
