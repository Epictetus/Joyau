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

#ifndef JOYAU_KEYS
#define JOYAU_KEYS

#include "Sprite.hpp"
#include "Manager.hpp"

class Pad: public Singleton<Pad>
{
   friend Pad &Singleton<Pad>::getInstance();
   friend void Singleton<Pad>::deleteInstance();
   Pad();
public:
   void update();

   bool pressed(const std::string &key) const;
   bool released(const std::string &key) const;
   bool held(const std::string &key) const;

   int getStickX() const { return stickX; }
   int getStickY() const { return stickY; }
protected:
   int str2key(const std::string &key) const;
private:
   SceCtrlData pad;
   SceCtrlData oldPad;

   int stickX, stickY;
};

class Cursor: public Sprite
{
public:
   void updatePos();
   void setSensibility(int s) { sensibility = s; }

   int getSensibility() const { return sensibility; }
private:
   int sensibility;
};

VALUE Cursor_updatePos(VALUE self);
VALUE Cursor_setSensibility(VALUE self, VALUE s);
VALUE Cursor_sensibility(VALUE self);

VALUE checkKeys(VALUE self);
VALUE Joyau_gets(VALUE self);

VALUE Pad_update(VALUE self);

VALUE Pad_held(VALUE self, VALUE key);
VALUE Pad_released(VALUE self, VALUE key);
VALUE Pad_pressed(VALUE self, VALUE key);

VALUE Pad_stickX(VALUE self);
VALUE Pad_stickY(VALUE self);

void defineKeys();

#endif
