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

#include "Keys.hpp"

void Cursor::updatePos()
{
   oslReadKeys();
   int analogX = osl_pad.analogX;
   int analogY = osl_pad.analogY;

   _x += analogX / sensibility;
   _y += analogY / sensibility;
   
   _x = _x > 480 ? 480 : _x;
   _x = _x < 0 ? 0 : _x;

   _y = _y > 272 ? 272 : _y;
   _y = _y < 0 ? 0 : _y;

   picture->setPos(_x, _y);
}

void Cursor::move(int x, int y)
{
   _x += x;
   _y += y;

   picture->setPos(_x, _y);
}

void Cursor::setPos(int x, int y)
{
   _x = x;
   _y = y;

   picture->setPos(_x, _y);
}

VALUE Keys_repeatInit(VALUE self, VALUE time)
{
   oslSetKeyAutorepeat(OSL_KEYMASK_UP | OSL_KEYMASK_RIGHT | 
		       OSL_KEYMASK_DOWN | OSL_KEYMASK_LEFT | 
		       OSL_KEYMASK_R|OSL_KEYMASK_L, 40, FIX2INT(time));
   oslSetKeyAutorepeatInit(FIX2INT(time));
   return Qnil;
}

VALUE Keys_repeatInterval(VALUE self, VALUE time)
{
   oslSetKeyAutorepeatInterval(FIX2INT(time));
   return Qnil;
}

VALUE checkKeys(VALUE self)
{
   oslReadKeys();
   VALUE keys = rb_gv_get("$keys");

   if (osl_pad.held.select)
      rb_hash_aset(keys, rb_str_new2("select"), Qtrue);
   else
      rb_hash_aset(keys, rb_str_new2("select"), Qfalse);

   if (osl_pad.held.start)
      rb_hash_aset(keys, rb_str_new2("start"), Qtrue);
   else
      rb_hash_aset(keys, rb_str_new2("start"), Qfalse);

   if (osl_pad.held.up)
      rb_hash_aset(keys, rb_str_new2("up"), Qtrue);
   else
      rb_hash_aset(keys, rb_str_new2("up"), Qfalse);

   if (osl_pad.held.down)
      rb_hash_aset(keys, rb_str_new2("down"), Qtrue);
   else
      rb_hash_aset(keys, rb_str_new2("down"), Qfalse);

   if (osl_pad.held.left)
      rb_hash_aset(keys, rb_str_new2("left"), Qtrue);
   else
      rb_hash_aset(keys, rb_str_new2("left"), Qfalse);
   
   if (osl_pad.held.right)
      rb_hash_aset(keys, rb_str_new2("right"), Qtrue);
   else
      rb_hash_aset(keys, rb_str_new2("right"), Qfalse);

   if (osl_pad.held.L)
      rb_hash_aset(keys, rb_str_new2("L"), Qtrue);
   else
      rb_hash_aset(keys, rb_str_new2("L"), Qfalse);

   if (osl_pad.held.R)
      rb_hash_aset(keys, rb_str_new2("R"), Qtrue);
   else
      rb_hash_aset(keys, rb_str_new2("R"), Qfalse);

   if (osl_pad.held.circle)
      rb_hash_aset(keys, rb_str_new2("circle"), Qtrue);
   else
      rb_hash_aset(keys, rb_str_new2("circle"), Qfalse);

   if (osl_pad.held.triangle)
      rb_hash_aset(keys, rb_str_new2("triangle"), Qtrue);
   else
      rb_hash_aset(keys, rb_str_new2("triangle"), Qfalse);

   if (osl_pad.held.cross)
      rb_hash_aset(keys, rb_str_new2("cross"), Qtrue);
   else
      rb_hash_aset(keys, rb_str_new2("cross"), Qfalse);

   if (osl_pad.held.square)
      rb_hash_aset(keys, rb_str_new2("square"), Qtrue);
   else
      rb_hash_aset(keys, rb_str_new2("square"), Qfalse);

   if (osl_pad.held.hold)
      rb_hash_aset(keys, rb_str_new2("hold"), Qtrue);
   else
      rb_hash_aset(keys, rb_str_new2("hold"), Qfalse);

   //Hum... I dont like copying and pasting but... I can't use a list...
   //I've to use the oslib structure. Thus, yeah, the next block looks really
   //like the last one.

   if (osl_pad.pressed.select)
      rb_hash_aset(keys, rb_str_new2("pressed_select"), Qtrue);
   else
      rb_hash_aset(keys, rb_str_new2("pressed_select"), Qfalse);

   if (osl_pad.pressed.start)
      rb_hash_aset(keys, rb_str_new2("pressed_start"), Qtrue);
   else
      rb_hash_aset(keys, rb_str_new2("pressed_start"), Qfalse);

   if (osl_pad.pressed.up)
      rb_hash_aset(keys, rb_str_new2("pressed_up"), Qtrue);
   else
      rb_hash_aset(keys, rb_str_new2("pressed_up"), Qfalse);

   if (osl_pad.pressed.down)
      rb_hash_aset(keys, rb_str_new2("pressed_down"), Qtrue);
   else
      rb_hash_aset(keys, rb_str_new2("pressed_down"), Qfalse);

   if (osl_pad.pressed.left)
      rb_hash_aset(keys, rb_str_new2("pressed_left"), Qtrue);
   else
      rb_hash_aset(keys, rb_str_new2("pressed_left"), Qfalse);
   
   if (osl_pad.pressed.right)
      rb_hash_aset(keys, rb_str_new2("pressed_right"), Qtrue);
   else
      rb_hash_aset(keys, rb_str_new2("pressed_right"), Qfalse);

   if (osl_pad.pressed.L)
      rb_hash_aset(keys, rb_str_new2("pressed_L"), Qtrue);
   else
      rb_hash_aset(keys, rb_str_new2("pressed_L"), Qfalse);

   if (osl_pad.pressed.R)
      rb_hash_aset(keys, rb_str_new2("pressed_R"), Qtrue);
   else
      rb_hash_aset(keys, rb_str_new2("pressed_R"), Qfalse);

   if (osl_pad.pressed.circle)
      rb_hash_aset(keys, rb_str_new2("pressed_circle"), Qtrue);
   else
      rb_hash_aset(keys, rb_str_new2("pressed_circle"), Qfalse);

   if (osl_pad.pressed.triangle)
      rb_hash_aset(keys, rb_str_new2("pressed_triangle"), Qtrue);
   else
      rb_hash_aset(keys, rb_str_new2("pressed_triangle"), Qfalse);

   if (osl_pad.pressed.cross)
      rb_hash_aset(keys, rb_str_new2("pressed_cross"), Qtrue);
   else
      rb_hash_aset(keys, rb_str_new2("pressed_cross"), Qfalse);

   if (osl_pad.pressed.square)
      rb_hash_aset(keys, rb_str_new2("pressed_square"), Qtrue);
   else
      rb_hash_aset(keys, rb_str_new2("pressed_square"), Qfalse);

   if (osl_pad.pressed.hold)
      rb_hash_aset(keys, rb_str_new2("pressed_hold"), Qtrue);
   else
      rb_hash_aset(keys, rb_str_new2("pressed_hold"), Qfalse);

   if (osl_pad.pressed.select)
      rb_hash_aset(keys, rb_str_new2("pressed_select"), Qtrue);
   else
      rb_hash_aset(keys, rb_str_new2("pressed_select"), Qfalse);

   if (osl_pad.pressed.start)
      rb_hash_aset(keys, rb_str_new2("pressed_start"), Qtrue);
   else
      rb_hash_aset(keys, rb_str_new2("pressed_start"), Qfalse);

   //Hum... should I copy and past the last comment ?

   if (osl_pad.released.up)
      rb_hash_aset(keys, rb_str_new2("released_up"), Qtrue);
   else
      rb_hash_aset(keys, rb_str_new2("released_up"), Qfalse);

   if (osl_pad.released.down)
      rb_hash_aset(keys, rb_str_new2("released_down"), Qtrue);
   else
      rb_hash_aset(keys, rb_str_new2("released_down"), Qfalse);

   if (osl_pad.released.left)
      rb_hash_aset(keys, rb_str_new2("released_left"), Qtrue);
   else
      rb_hash_aset(keys, rb_str_new2("released_left"), Qfalse);
   
   if (osl_pad.released.right)
      rb_hash_aset(keys, rb_str_new2("released_right"), Qtrue);
   else
      rb_hash_aset(keys, rb_str_new2("released_right"), Qfalse);

   if (osl_pad.released.L)
      rb_hash_aset(keys, rb_str_new2("released_L"), Qtrue);
   else
      rb_hash_aset(keys, rb_str_new2("released_L"), Qfalse);

   if (osl_pad.released.R)
      rb_hash_aset(keys, rb_str_new2("released_R"), Qtrue);
   else
      rb_hash_aset(keys, rb_str_new2("released_R"), Qfalse);

   if (osl_pad.released.circle)
      rb_hash_aset(keys, rb_str_new2("released_circle"), Qtrue);
   else
      rb_hash_aset(keys, rb_str_new2("released_circle"), Qfalse);

   if (osl_pad.released.triangle)
      rb_hash_aset(keys, rb_str_new2("released_triangle"), Qtrue);
   else
      rb_hash_aset(keys, rb_str_new2("released_triangle"), Qfalse);

   if (osl_pad.released.cross)
      rb_hash_aset(keys, rb_str_new2("released_cross"), Qtrue);
   else
      rb_hash_aset(keys, rb_str_new2("released_cross"), Qfalse);

   if (osl_pad.released.square)
      rb_hash_aset(keys, rb_str_new2("released_square"), Qtrue);
   else
      rb_hash_aset(keys, rb_str_new2("released_square"), Qfalse);

   if (osl_pad.released.hold)
      rb_hash_aset(keys, rb_str_new2("released_hold"), Qtrue);
   else
      rb_hash_aset(keys, rb_str_new2("released_hold"), Qfalse);

   int analogX = osl_pad.analogX;
   int analogY = osl_pad.analogY;

   rb_hash_aset(keys, rb_str_new2("analogX"), INT2FIX(analogX));
   rb_hash_aset(keys, rb_str_new2("analogY"), INT2FIX(analogY));

   rb_gv_set("$keys", keys);
   
   return Qnil;
}

VALUE Cursor_draw(VALUE self)
{
   Cursor *ptr = getPtr<Cursor>(self);
   ptr->draw();
   
   return Qnil;
}

VALUE Cursor_updatePos(VALUE self)
{
   Cursor *ptr = getPtr<Cursor>(self);
   ptr->updatePos();
   
   return Qnil;
}

VALUE Cursor_setSensibility(VALUE self, VALUE s)
{
   Cursor *ptr = getPtr<Cursor>(self);
   int sens = FIX2INT(s);

   ptr->setSensibility(sens);   
   return Qnil;
}

VALUE Cursor_move(VALUE self, VALUE x, VALUE y)
{
   Cursor *ptr = getPtr<Cursor>(self);
   int _x = FIX2INT(x);
   int _y = FIX2INT(y);

   ptr->move(_x, _y);
   return Qnil;
}

VALUE Cursor_setPos(VALUE self, VALUE x, VALUE y)
{
   Cursor *ptr = getPtr<Cursor>(self);
   int _x = FIX2INT(x);
   int _y = FIX2INT(y);

   ptr->setPos(_x, _y);
   return Qnil;
}

VALUE Cursor_getX(VALUE self)
{
   Cursor *ptr = getPtr<Cursor>(self);
   int ret = ptr->getX();

   return INT2FIX(ret);
}

VALUE Cursor_getY(VALUE self)
{
   Cursor *ptr = getPtr<Cursor>(self);
   int ret = ptr->getY();

   return INT2FIX(ret);
}

VALUE Cursor_setPicture(VALUE self, VALUE spr)
{
   Cursor *ptr = getPtr<Cursor>(self);
   Sprite *pic = getPtr<Sprite>(spr);

   ptr->setPicture(pic);
   return Qnil;
}

VALUE Cursor_collide(VALUE self, VALUE spr)
{
   Cursor *ptr = getPtr<Cursor>(self);
   Sprite *pic = getPtr<Sprite>(spr);

   return ptr->collide(pic) ? Qtrue : Qfalse;
}

VALUE Cursor_isOn(VALUE self, VALUE x, VALUE y)
{
   Cursor *ptr = getPtr<Cursor>(self);
   int _x = FIX2INT(x);
   int _y = FIX2INT(y);

   return ptr->isOn(_x, _y) ? Qtrue : Qfalse;
}

void defineKeys()
{
   VALUE keys = rb_hash_new();
   rb_gv_set("$keys", keys);
   
   defFunc("repeatInit", Keys_repeatInit, 1);
   defFunc("repeatInterval", Keys_repeatInterval, 1);
   defFunc("readKeys", checkKeys, 0);

   VALUE cCursor = defClass<Cursor>("Cursor");
   defMethod(cCursor, "draw", Cursor_draw, 0);
   defMethod(cCursor, "updatePos", Cursor_updatePos, 0);
   defMethod(cCursor, "setSensibility", Cursor_setSensibility, 1);
   defMethod(cCursor, "move", Cursor_move, 2);
   defMethod(cCursor, "setPos", Cursor_setPos, 2);
   defMethod(cCursor, "getX", Cursor_getX, 0);
   defMethod(cCursor, "getY", Cursor_getY, 0);
   defMethod(cCursor, "setPicture", Cursor_setPicture, 1);
   defMethod(cCursor, "collide", Cursor_collide, 1);
   defMethod(cCursor, "isOn", Cursor_isOn, 2);
}
