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
 
void Particles::setFile(char *file) // We can't use manager for that.
{
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
   Particles *item = getPtr<Particles>(self);
   char *val = StringValuePtr(str);

   item->setFile(val);
   return Qnil;
}

VALUE Particles_setParam(VALUE self, VALUE time, VALUE speed, VALUE gravity,
			 VALUE mspeed)
{
   Particles *item = getPtr<Particles>(self);
   int _time = FIX2INT(time);
   int _speed = FIX2INT(speed);
   int _gravity = FIX2INT(gravity);
   int _mspeed = FIX2INT(mspeed);

   item->setParam(_time, _speed, _gravity, _mspeed);
   return Qnil;
}

VALUE Particles_move(VALUE self, VALUE x, VALUE y)
{
   Particles *item = getPtr<Particles>(self);
   int _x = FIX2INT(x);
   int _y = FIX2INT(y);

   item->move(_x, _y);
   return Qnil;
}

VALUE Particles_addParticles(VALUE self, VALUE x, VALUE y)
{
   Particles *item = getPtr<Particles>(self);
   int _x = FIX2INT(x);
   int _y = FIX2INT(y);

   item->addParticles(_x, _y);
   return Qnil;
}

VALUE Particles_draw(VALUE self)
{
   Particles *item = getPtr<Particles>(self);
   item->draw();
   return Qnil;
}

VALUE Particles_getTime(VALUE self)
{
   Particles *item = getPtr<Particles>(self);
   return FIX2INT(item->getTime());
}

VALUE Particles_getSpeed(VALUE self)
{
   Particles *item = getPtr<Particles>(self);
   return FIX2INT(item->getSpeed());
}

VALUE Particles_getGravity(VALUE self)
{
   Particles *item = getPtr<Particles>(self);
   return FIX2INT(item->getGravity());
}

VALUE Particles_getMinSpeed(VALUE self)
{
   Particles *item = getPtr<Particles>(self);
   return FIX2INT(item->getMinSpeed());
}

void defineParticles()
{
   VALUE cPart = defClass<Particles>("Particles");
   rb_define_method(cPart, "setFile", RPROTO(Particles_setFile), 1);
   rb_define_method(cPart, "setParam", RPROTO(Particles_setParam), 4);
   rb_define_method(cPart, "move", RPROTO(Particles_move), 2);
   rb_define_method(cPart, "addParticles", RPROTO(Particles_addParticles), 2);
   rb_define_method(cPart, "draw", RPROTO(Particles_draw), 0);
   rb_define_method(cPart, "getTime", RPROTO(Particles_getTime), 0);
   rb_define_method(cPart, "getSpeed", RPROTO(Particles_getSpeed), 0);
   rb_define_method(cPart, "getGravity", RPROTO(Particles_getGravity), 0);
   rb_define_method(cPart, "getMinSpeed", RPROTO(Particles_getMinSpeed), 0);
}
