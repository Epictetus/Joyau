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

#ifndef __JOYAU_PARTICLES__
#define __JOYAU_PARTICLES__

#include "StdInclude.hpp"

class Particles
{
public:
   void setFile(char *str);
   void setParam(int time, int speed, int gravity, int mspeed);

   void move(int x, int y);
   void addParticles(int x, int y);

   void draw();

   int getTime() { return _time; }
   int getSpeed() { return _speed; }
   int getGravity() { return _gravity; }
   int getMinSpeed() { return _mspeed; }
private:
   OSL_PARTICLES *part;

   int _time, _speed, _gravity, _mspeed;
};

VALUE Particles_setFile(VALUE self, VALUE str);
VALUE Particles_setParam(VALUE self, VALUE time, VALUE speed, VALUE gravity,
			 VALUE mspeed);

VALUE Particles_move(VALUE self, VALUE x, VALUE y);
VALUE Particles_addParticles(VALUE self, VALUE x, VALUE y);

VALUE Particles_draw(VALUE self);

VALUE Particles_getTime(VALUE self);
VALUE Particles_getSpeed(VALUE self);
VALUE Particles_getGravity(VALUE self);
VALUE Particles_getMinSpeed(VALUE self);

void defineParticles();

#endif
