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

#include "Particles.hpp"

template<> VALUE wrap<Particles>(int argc, VALUE *argv, VALUE info)
{
   Particles *ptr = new Particles;
   VALUE tdata;
   
   if (argc >= 1)
      ptr->setFile(StringValuePtr(argv[0]));
   
   tdata = Data_Wrap_Struct(info, 0, wrapped_free<Particles>, ptr);
   return tdata;
}

Particles::Particles(const Particles &obj)
{
   _time = obj._time;
   _speed = obj._speed;
   _gravity = obj._gravity;
   _mspeed = obj._mspeed;
   setParam(_time, _speed, _gravity, _mspeed);

   char txt[256];
   strcpy(txt, obj.filename.c_str());

   setFile(txt);
}
 
void Particles::setFile(char *file) // We can't use manager for that.
{
   filename = file;
   part = oslLoadParticles(file);
}

void Particles::setParam(int time, int speed, int gravity, int mspeed)
{
   _time = time;
   _speed = speed;
   _gravity = gravity;
   _mspeed = mspeed;
   oslInitParticles(part, _time, _speed, _gravity, _mspeed);
}

void Particles::move(int x, int y)
{
   oslMoveXParticles(part, x);
   oslMoveYParticles(part, y);
}

void Particles::addParticles(int x, int y)
{
   oslNewParticles(part, x, y);
}

void Particles::draw()
{
   oslUpdateParticles(part);
   oslRenderParticles(part);
}

VALUE Particles_setFile(VALUE self, VALUE str)
{
   Particles &ref = getRef<Particles>(self);
   char *val = StringValuePtr(str);

   ref.setFile(val);
   return Qnil;
}

VALUE Particles_setParam(VALUE self, VALUE time, VALUE speed, VALUE gravity,
			 VALUE mspeed)
{
   Particles &ref = getRef<Particles>(self);
   int _time = FIX2INT(time);
   int _speed = FIX2INT(speed);
   int _gravity = FIX2INT(gravity);
   int _mspeed = FIX2INT(mspeed);

   ref.setParam(_time, _speed, _gravity, _mspeed);
   return Qnil;
}

VALUE Particles_move(VALUE self, VALUE x, VALUE y)
{
   Particles &ref = getRef<Particles>(self);
   int _x = FIX2INT(x);
   int _y = FIX2INT(y);

   ref.move(_x, _y);
   return Qnil;
}

VALUE Particles_addParticles(VALUE self, VALUE x, VALUE y)
{
   Particles &ref = getRef<Particles>(self);
   int _x = FIX2INT(x);
   int _y = FIX2INT(y);

   ref.addParticles(_x, _y);
   return Qnil;
}

VALUE Particles_draw(VALUE self)
{
   Particles &ref = getRef<Particles>(self);
   ref.draw();
   return Qnil;
}

VALUE Particles_getTime(VALUE self)
{
   Particles &ref = getRef<Particles>(self);
   return FIX2INT(ref.getTime());
}

VALUE Particles_getSpeed(VALUE self)
{
   Particles &ref = getRef<Particles>(self);
   return FIX2INT(ref.getSpeed());
}

VALUE Particles_getGravity(VALUE self)
{
   Particles &ref = getRef<Particles>(self);
   return FIX2INT(ref.getGravity());
}

VALUE Particles_getMinSpeed(VALUE self)
{
   Particles &ref = getRef<Particles>(self);
   return FIX2INT(ref.getMinSpeed());
}

void defineParticles()
{
   VALUE cPart = defClass<Particles>("Particles");
   defMethod(cPart, "setFile", Particles_setFile, 1);
   defMethod(cPart, "setParam", Particles_setParam, 4);
   defMethod(cPart, "move", Particles_move, 2);
   defMethod(cPart, "addParticles", Particles_addParticles, 2);
   defMethod(cPart, "draw", Particles_draw, 0);
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
