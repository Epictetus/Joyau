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

#include "Sprite.hpp"

void Sprite::setPicture(char *pic)
{
   picName = pic;
   sprite = manager->getPic(pic);

   _w = sprite->sizeX;
   _h = sprite->sizeY;

   _stretchX = sprite->stretchX;
   _stretchY = sprite->stretchY;
}

void Sprite::setPos(int x, int y)
{
   _x = x;
   _y = y;
}

void Sprite::move(int x, int y)
{
   _x += x;
   _y += y;
}

void Sprite::setAlpha(int alpha)
{
   _alpha = alpha;
}

int Sprite::getAlpha()
{
   return _alpha;
}

int Sprite::getX() { return _x; }
int Sprite::getY() { return _y; }
int Sprite::getW() { return _w; }
int Sprite::getH() { return _h; }

bool Sprite::isOn(int x, int y)
{
   /*
     A point which is on this sprite should be at the left
     side's right ( x >= getX() ), at the right side's left 
     ( x <= getX() + getW() ) , upper the bottom side ( y <= getY() + getH() )
     , and at the bottom of the upper side ( y >= getY() ).
    */
   if (x >= getX() && x <= getX() + getW() &&
       y >= getY() && y <= getY() + getH())
      return true;
   else
      return false;
}

bool Sprite::collide(Sprite *spr)
{
   /*
     If the A image collides the B image, it means that at least 
     one of A's corner OR one of B's corner is on the other sprite.
    */
   if (isOn(spr->getX(), spr->getY()))
      return true;
   else if (isOn(spr->getX() + spr->getW(), spr->getY()))
      return true;
   else if (isOn(spr->getX() + spr->getW(), spr->getY() + spr->getH()))
      return true;
   else if (isOn(spr->getX(), spr->getY() + spr->getH()))
      return true;
   else if (spr->isOn(getX(), getY()))
      return true;
   else if (spr->isOn(getX() + getW(), getY()))
      return true;
   else if (spr->isOn(getX() + getW(), getY() + getH()))
      return true;
   else if (spr->isOn(getX(), getY() + getH()))
      return true;
   else
      return false;
}

/*
  defaultDraw() can be used in the Draw function from inherited class, 
  so we avoid using private attributes.
 */
void Sprite::Draw()
{
   defaultDraw();
}

void Sprite::defaultDraw()
{
   /*
     Since theses images should only be drawn by that class, we may safely
     change theses attributes value.
    */

   oslSetAlpha(OSL_FX_ALPHA , _alpha);

   sprite->stretchX = _stretchX;
   sprite->stretchY = _stretchY;
   sprite->x = _x;
   sprite->y = _y;
   sprite->angle = _angle;

   oslDrawImage(sprite);
   oslSetAlpha(OSL_FX_RGBA, _alpha);
}

void Sprite::setDirection(int dir)
{
   _dir = dir;
}

void Sprite::zoom(int increase)
{
   _stretchX += increase;
   _stretchY += increase;
   _zoom += increase;
}

VALUE wrapSprite(VALUE info)
{
   Sprite *item = new Sprite(Manager::getInstance());
   VALUE tdata = Data_Wrap_Struct(info, 0, Sprite_free, item);
   return tdata;
}

void Sprite_free(void *p) { delete (Sprite *)p; }

VALUE Sprite_rotate(VALUE self, VALUE angle) 
{
   Sprite *item;
   Data_Get_Struct(self, Sprite, item);

   item->rotate(FIX2INT(angle));
   return Qnil;
}

VALUE Sprite_getAngle(VALUE self) 
{
   Sprite *item;
   Data_Get_Struct(self, Sprite, item);

   return INT2FIX(item->getAngle()); 
}

VALUE Sprite_setAngle(VALUE self, VALUE angle) 
{
   Sprite *item;
   Data_Get_Struct(self, Sprite, item);

   item->setAngle(FIX2INT(angle));

   return Qnil;
}

VALUE Sprite_getZoom(VALUE self)
{
   Sprite *item;
   Data_Get_Struct(self, Sprite, item);

   return INT2FIX(item->getZoom());
}

VALUE Sprite_getDirection(VALUE self) 
{ 
   Sprite *item;
   Data_Get_Struct(self, Sprite, item);

   return INT2FIX(item->getDirection()); 
}

VALUE Sprite_setPicture(VALUE self, VALUE pic)
{
   Sprite *item;
   Data_Get_Struct(self, Sprite, item);

   item->setPicture(StringValuePtr(pic));

   return Qnil;
}

VALUE Sprite_setPos(VALUE self, VALUE x, VALUE y)
{
   Sprite *item;
   Data_Get_Struct(self, Sprite, item);

   item->setPos(FIX2INT(x), FIX2INT(y));

   return Qnil;
}

VALUE Sprite_move(VALUE self, VALUE x, VALUE y)
{
   Sprite *item;
   Data_Get_Struct(self, Sprite, item);

   item->move(FIX2INT(x), FIX2INT(y));
   return Qnil;
}

VALUE Sprite_setAlpha(VALUE self, VALUE alpha)
{
   Sprite *item;
   Data_Get_Struct(self, Sprite, item);

   item->setAlpha(FIX2INT(alpha));

   return Qnil;
}

VALUE Sprite_getAlpha(VALUE self)
{
   Sprite *item;
   Data_Get_Struct(self, Sprite, item);

   return INT2FIX(item->getAlpha());
}

VALUE Sprite_getX(VALUE self) 
{
   Sprite *item;
   Data_Get_Struct(self, Sprite, item);
   
   return INT2FIX(item->getX());
}
VALUE Sprite_getY(VALUE self) 
{
   Sprite *item;
   Data_Get_Struct(self, Sprite, item);

   return INT2FIX(item->getY());
}

VALUE Sprite_getW(VALUE self) 
{
   Sprite *item;
   Data_Get_Struct(self, Sprite, item);

   return INT2FIX(item->getW());
}
VALUE Sprite_getH(VALUE self) 
{
   Sprite *item;
   Data_Get_Struct(self, Sprite, item);

   return INT2FIX(item->getH()); 
}

VALUE Sprite_isOn(VALUE self, VALUE rb_x, VALUE rb_y)
{
   Sprite *item;
   Data_Get_Struct(self, Sprite, item);

   if (item->isOn(FIX2INT(rb_x), FIX2INT(rb_y)))
      return Qtrue;
   return Qfalse;
}

VALUE Sprite_collide(VALUE self, VALUE spr)
{
   Sprite *item;
   Data_Get_Struct(self, Sprite, item);

   Sprite *adv;
   Data_Get_Struct(spr, Sprite, adv);

   if (item->collide(adv))
      return Qtrue;
   return Qfalse;
}

VALUE Sprite_draw(VALUE self)
{
   Sprite *item;
   Data_Get_Struct(self, Sprite, item);

   item->Draw();

   return Qnil;
}

VALUE Sprite_setDirection(VALUE self, VALUE dir)
{
   Sprite *item;
   Data_Get_Struct(self, Sprite, item);

   item->setDirection(FIX2INT(dir));

   return Qnil;
}

VALUE Sprite_zoom(VALUE self, VALUE val)
{
   Sprite *item;
   Data_Get_Struct(self, Sprite, item);

   item->zoom(FIX2INT(val));
   
   return Qnil; 
}

void defineSprite()
{
   VALUE dirHash = rb_hash_new();
   rb_hash_aset(dirHash, rb_str_new2("LEFT"), INT2FIX(LEFT));
   rb_hash_aset(dirHash, rb_str_new2("RIGHT"), INT2FIX(RIGHT));
   rb_hash_aset(dirHash, rb_str_new2("UP"), INT2FIX(UP));
   rb_hash_aset(dirHash, rb_str_new2("DOWN"), INT2FIX(DOWN));
   rb_hash_aset(dirHash, rb_str_new2("UP_LEFT"), INT2FIX(UP_LEFT));
   rb_hash_aset(dirHash, rb_str_new2("UP_RIGHT"), INT2FIX(UP_RIGHT));
   rb_hash_aset(dirHash, rb_str_new2("DOWN_LEFT"), INT2FIX(DOWN_LEFT));
   rb_hash_aset(dirHash, rb_str_new2("DOWN_RIGHT"), INT2FIX(DOWN_RIGHT));
   rb_gv_set("$directions", dirHash);

   VALUE cSprite = rb_define_class("Sprite", rb_cObject);

   rb_define_singleton_method(cSprite, "new", (VALUE(*)(...))&wrapSprite, 0);

   rb_define_method(cSprite, "setPicture", (VALUE(*)(...))&Sprite_setPicture, 
		    1);
   rb_define_method(cSprite, "setPos", (VALUE(*)(...))&Sprite_setPos, 2);
   rb_define_method(cSprite, "move", (VALUE(*)(...))&Sprite_move, 2);
   rb_define_method(cSprite, "rotate", (VALUE(*)(...))&Sprite_rotate, 1);
   rb_define_method(cSprite, "getAngle", (VALUE(*)(...))&Sprite_getAngle, 0);
   rb_define_method(cSprite, "setAngle", (VALUE(*)(...))&Sprite_setAngle, 1);
   rb_define_method(cSprite, "zoom", (VALUE(*)(...))&Sprite_zoom, 1);
   rb_define_method(cSprite, "getZoom", (VALUE(*)(...))&Sprite_getZoom, 0);
   rb_define_method(cSprite, "setAngle", (VALUE(*)(...))&Sprite_setAngle, 1);
   rb_define_method(cSprite, "setAlpha", (VALUE(*)(...))&Sprite_setAlpha, 1);
   rb_define_method(cSprite, "getAlpha", (VALUE(*)(...))&Sprite_getAlpha, 0);
   rb_define_method(cSprite, "getX", (VALUE(*)(...))&Sprite_getX, 0);
   rb_define_method(cSprite, "getY", (VALUE(*)(...))&Sprite_getY, 0);
   rb_define_method(cSprite, "getW", (VALUE(*)(...))&Sprite_getW, 0);
   rb_define_method(cSprite, "getH", (VALUE(*)(...))&Sprite_getH, 0);
   rb_define_method(cSprite, "isOn", (VALUE(*)(...))&Sprite_isOn, 2);
   rb_define_method(cSprite, "collide", (VALUE(*)(...))&Sprite_collide, 1);
   rb_define_method(cSprite, "draw", (VALUE(*)(...))&Sprite_draw, 0);
   rb_define_method(cSprite, "setDirection", 
		    (VALUE(*)(...))&Sprite_setDirection, 1);
   rb_define_method(cSprite, "getDirection", 
		    (VALUE(*)(...))&Sprite_getDirection, 0);
}
