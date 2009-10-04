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

/** @addtogroup Input **/
/*@{*/

/** 
 * @class Pad
 * Singleton which checks the input.
 */
class Pad: public Singleton<Pad>
{
   friend Pad &Singleton<Pad>::getInstance();
   friend void Singleton<Pad>::deleteInstance();
   Pad();
public:
   /** Updates the keys. **/
   void update();

   /** Returns whether a key has been pressed right now.
    *  @param key key's name
    */
   bool pressed(const std::string &key) const;

   /** Returns whether a key has been released right now.
    *  @param key key's name
    */
   bool released(const std::string &key) const;

   /** Returns whether a key is held.
    *  @param key key's name
    */
   bool held(const std::string &key) const;

   /** Returns whether a key has been pressed right now.
    *  @param key key's value
    */
   bool pressed(int key) const;

   /** Returns whether a key has been released right now.
    *  @param key key's value
    */
   bool released(int key) const;

   /** Returns whether a key is held.
    *  @param key key's value (defined in pspctrl.h)
    */
   bool held(int key) const;

   /** Returns the analogic stick's x position.
    * @note the minimum value is -128, and the maximum is 128.
    */
   int getStickX() const { return pad.Lx - 128; }

   /** Returns the analogic stick's y position.
    * @note the minimum value is -128, and the maximum is 128.
    */
   int getStickY() const { return pad.Ly - 128; }
protected:
   /** Converts a key from its name to its value **/
   int str2key(const std::string &key) const;
private:
   SceCtrlData pad;
   int oldPad;
};

/*@}*/

/** @addtogroup Drawables **/
/*@{*/

/** 
 * @class Cursor
 * A sprite whose moves are done according to the analogic stick.
 */
class Cursor: public Sprite
{
public:
   Cursor():
      rect(0, 0, 480, 272)
   { setClass("Cursor"); }

   /** Updates the Cursor position **/
   void updatePos();

   /** Changes the sensibility. If the sensibility is bigger, the cursor
    *  moved less.
    *  @param s new sensibility.
    */
   void setSensibility(int s) { sensibility = s; }

   /** Retruns the cursor's sensibility. **/
   int getSensibility() const { return sensibility; }

   /** Sets the rect the cursor can move in. **/
   void setRect(const Rect &r) { rect = r; }

   /** Returns the rect the cursor can move in. **/
   Rect getRect() const { return rect; }
private:
   int sensibility;

   Rect rect;
};

/*@}*/

VALUE Cursor_updatePos(VALUE self);

VALUE Cursor_setSensibility(VALUE self, VALUE s);
VALUE Cursor_sensibility(VALUE self);

VALUE Cursor_setRect(VALUE self, VALUE rect);
VALUE Cursor_rect(VALUE self);

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
