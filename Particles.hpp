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

#ifndef JOYAU_PARTICLES
#define JOYAU_PARTICLES

#include "Drawable.hpp"

#define cGfxParticleSet	4
#define MaxParticules  600
#define cGfxParticleState 4

typedef struct PARTICLE {
   int x[cGfxParticleSet];
   int y[cGfxParticleSet];
   int dx[cGfxParticleSet];
   int dy[cGfxParticleSet];
   int sx[cGfxParticleSet];
   int sy[cGfxParticleSet];
   int status;
} PARTICLE;

typedef struct PARTICULE {
   PARTICLE particle[MaxParticules];
   OSL_IMAGE *Images[cGfxParticleState];
   int vie;
   int Vitesse;
   int Vitesse_Mini;
   int Graviter;
} PARTICULE;

inline void _oslDeleteParticles(PARTICULE * Part)
{
   oslDeleteImage(Part->Images[0]);
   oslDeleteImage(Part->Images[1]);
   oslDeleteImage(Part->Images[2]);
   oslDeleteImage(Part->Images[3]);

   free(Part);
}

/** @addtogroup Drawables **/
/*@{*/

/** 
 * @class Particles
 * A particles generator.
 */
class Particles:public Drawable {
 public:
   Particles() {
      setClass("Particles");
   }
   
   Particles(const Particles & obj);
   
   ~Particles() {
      _oslDeleteParticles(part);
   }

   /** Loads a particle file.
    *  @param str filename
    */
   void setFile(char *str);

   /** Configurates the generator. Should be called only once.
    *  @param time particle's lifetime
    *  @param speed particle's speed
    *  @param mspeed minimum speed
    */
   void setParam(int time, int speed, int gravity, int mspeed);

   void move(int x, int y);

   /** Adds a particle.
    *  @param x particle's x position
    *  @param y particle's y position
    */
   void addParticles(int x, int y);

   void draw();

   /** Returns a particle's lifetime **/
   int getTime() const {
      return _time;
   }

   /** Returns a particle's speed **/
   int getSpeed() const {
      return _speed;
   }
   /** Returns the gravity **/
   int getGravity() const {
      return _gravity;
   }
   /** Returns the minimum speed **/
   int getMinSpeed() const {
      return _mspeed;
 }
private:
   PARTICULE * part;
   
   int _time, _speed, _gravity, _mspeed;
   std::string filename;
};

/*@}*/

VALUE Particles_setFile(VALUE self, VALUE str);
VALUE Particles_setParam(VALUE self, VALUE time, VALUE speed, VALUE gravity,
			 VALUE mspeed);

VALUE Particles_addParticles(VALUE self, VALUE x, VALUE y);

VALUE Particles_getTime(VALUE self);
VALUE Particles_getSpeed(VALUE self);
VALUE Particles_getGravity(VALUE self);
VALUE Particles_getMinSpeed(VALUE self);

void defineParticles();

#endif
