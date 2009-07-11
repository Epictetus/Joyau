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

//Since nbrX and nbrY are initialized to 1, we cane use them even in
//not-animated sprites. 
int Sprite::getW() { return tiled ? wTile : _w / _nbrX; }
int Sprite::getH() { return tiled ? hTile : _h / _nbrY; }

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

   oslDrawImage(getImage());
   oslSetAlpha(OSL_FX_RGBA, _alpha);
}

void Sprite::setDirection(int dir)
{
   _dir = dir;
}

void Sprite::setAnimation(int nbrX, int nbrY)
{
   if (nbrX != 1 || nbrY != 1)
      animated = true;
   else
      animated = false;
   _nbrX =  nbrX; // We'll change the frame at the draw function, 
   _nbrY = nbrY; // so we can use the same OSL_IMAGE in multiple sprite :)
}

void Sprite::zoom(int increase)
{
   _stretchX += increase;
   _stretchY += increase;
   _zoom += increase;
}

void Sprite::setTile(int x, int y, int w, int h)
{
   tiled = true;
   xTile = x;
   yTile = y;
   wTile = w;
   hTile = h;
}

OSL_IMAGE *Sprite::getImage()
{
   oslSetAlpha(OSL_FX_ALPHA , _alpha);

   sprite->stretchX = _stretchX;
   sprite->stretchY = _stretchY;
   sprite->x = _x;
   sprite->y = _y;
   sprite->angle = _angle;

   if (!tiled)
   {
      if (animated)
	 oslSetImageTileSize(sprite, animeState * getW(), 
			     getDirection() * getH(), getW(), getH());
      else
	 oslSetImageTileSize(sprite, 0, 0, getW(), getH());
      passedTime++;
      if (passedTime == nbrAnime)
      {
	 passedTime = 0;
	 animeState++;
      }
      if (animeState == _nbrX)
	 animeState = 0;
   }
   else
      oslSetImageTileSize(sprite, xTile, yTile, wTile, hTile);

   return sprite;
}

void Sprite::saveImage(const char *fname)
{
   oslWriteImageFile(getImage(), fname, OSL_WRI_ALPHA);
   oslSetAlpha(OSL_FX_RGBA, _alpha);
}

VALUE Sprite_rotate(VALUE self, VALUE angle) 
{
   Sprite *item = getPtr<Sprite>(self);

   item->rotate(FIX2INT(angle));
   return Qnil;
}

VALUE Sprite_getAngle(VALUE self) 
{
   Sprite *item = getPtr<Sprite>(self);

   return INT2FIX(item->getAngle()); 
}

VALUE Sprite_setAngle(VALUE self, VALUE angle) 
{
   Sprite *item = getPtr<Sprite>(self);

   item->setAngle(FIX2INT(angle));

   return Qnil;
}

VALUE Sprite_getZoom(VALUE self)
{
   Sprite *item = getPtr<Sprite>(self);

   return INT2FIX(item->getZoom());
}

VALUE Sprite_getDirection(VALUE self) 
{ 
   Sprite *item = getPtr<Sprite>(self);

   return INT2FIX(item->getDirection()); 
}

VALUE Sprite_setAnimation(VALUE self, VALUE nbrX, VALUE nbrY)
{
   Sprite *item = getPtr<Sprite>(self);

   item->setAnimation(FIX2INT(nbrX), FIX2INT(nbrY));
   return Qnil;
}

VALUE Sprite_setAnimationTime(VALUE self, VALUE t)
{
   Sprite *item = getPtr<Sprite>(self);

   item->setAnimationTime(FIX2INT(t));
   return Qnil;
}

VALUE Sprite_setPicture(VALUE self, VALUE pic)
{
   Sprite *item = getPtr<Sprite>(self);

   item->setPicture(StringValuePtr(pic));

   return Qnil;
}

VALUE Sprite_setPos(VALUE self, VALUE x, VALUE y)
{
   Sprite *item = getPtr<Sprite>(self);

   item->setPos(FIX2INT(x), FIX2INT(y));

   return Qnil;
}

VALUE Sprite_move(VALUE self, VALUE x, VALUE y)
{
   Sprite *item = getPtr<Sprite>(self);

   item->move(FIX2INT(x), FIX2INT(y));
   return Qnil;
}

VALUE Sprite_setAlpha(VALUE self, VALUE alpha)
{
   Sprite *item = getPtr<Sprite>(self);

   item->setAlpha(FIX2INT(alpha));

   return Qnil;
}

VALUE Sprite_getAlpha(VALUE self)
{
   Sprite *item = getPtr<Sprite>(self);

   return INT2FIX(item->getAlpha());
}

VALUE Sprite_getX(VALUE self) 
{
   Sprite *item = getPtr<Sprite>(self);
   
   return INT2FIX(item->getX());
}
VALUE Sprite_getY(VALUE self) 
{
   Sprite *item = getPtr<Sprite>(self);

   return INT2FIX(item->getY());
}

VALUE Sprite_getW(VALUE self) 
{
   Sprite *item = getPtr<Sprite>(self);

   return INT2FIX(item->getW());
}
VALUE Sprite_getH(VALUE self) 
{
   Sprite *item = getPtr<Sprite>(self);

   return INT2FIX(item->getH()); 
}

VALUE Sprite_isOn(VALUE self, VALUE rb_x, VALUE rb_y)
{
   Sprite *item = getPtr<Sprite>(self);

   if (item->isOn(FIX2INT(rb_x), FIX2INT(rb_y)))
      return Qtrue;
   return Qfalse;
}

VALUE Sprite_collide(VALUE self, VALUE spr)
{
   Sprite *item = getPtr<Sprite>(self);
   Sprite *adv = getPtr<Sprite>(spr);

   if (item->collide(adv))
      return Qtrue;
   return Qfalse;
}

VALUE Sprite_draw(VALUE self)
{
   Sprite *item = getPtr<Sprite>(self);
   item->Draw();

   return Qnil;
}

VALUE Sprite_setDirection(VALUE self, VALUE dir)
{
   Sprite *item = getPtr<Sprite>(self);
   item->setDirection(FIX2INT(dir));

   return Qnil;
}

VALUE Sprite_zoom(VALUE self, VALUE val)
{
   Sprite *item = getPtr<Sprite>(self);
   item->zoom(FIX2INT(val));
   
   return Qnil; 
}

VALUE Sprite_setTile(VALUE self, VALUE x, VALUE y, VALUE w, VALUE h)
{
   Sprite *item = getPtr<Sprite>(self);
   item->setTile(FIX2INT(x), FIX2INT(y), FIX2INT(w), FIX2INT(h));

   return Qnil;
}

VALUE Sprite_saveFile(VALUE self, VALUE pic)
{
   Sprite *item = getPtr<Sprite>(self);
   const char *file = StringValuePtr(pic);

   item->saveImage(file);
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

   VALUE cSprite = defClass<Sprite>("Sprite");
   defMethod(cSprite, "setPicture", Sprite_setPicture, 1);
   defMethod(cSprite, "setPos", Sprite_setPos, 2);
   defMethod(cSprite, "move", Sprite_move, 2);
   defMethod(cSprite, "rotate", Sprite_rotate, 1);
   defMethod(cSprite, "getAngle", Sprite_getAngle, 0);
   defMethod(cSprite, "setAngle", Sprite_setAngle, 1);
   defMethod(cSprite, "zoom", Sprite_zoom, 1);
   defMethod(cSprite, "getZoom", Sprite_getZoom, 0);
   defMethod(cSprite, "setAngle", Sprite_setAngle, 1);
   defMethod(cSprite, "setAlpha", Sprite_setAlpha, 1);
   defMethod(cSprite, "getAlpha", Sprite_getAlpha, 0);
   defMethod(cSprite, "getX", Sprite_getX, 0);
   defMethod(cSprite, "getY", Sprite_getY, 0);
   defMethod(cSprite, "getW", Sprite_getW, 0);
   defMethod(cSprite, "getH", Sprite_getH, 0);
   defMethod(cSprite, "isOn", Sprite_isOn, 2);
   defMethod(cSprite, "collide", Sprite_collide, 1);
   defMethod(cSprite, "draw", Sprite_draw, 0);
   defMethod(cSprite, "saveFile", Sprite_saveFile, 1);
   defMethod(cSprite, "setDirection", Sprite_setDirection, 1);
   defMethod(cSprite, "getDirection", Sprite_getDirection, 0);
   defMethod(cSprite, "setAnim", Sprite_setAnimation, 2);
   defMethod(cSprite, "setAnimTime", Sprite_setAnimationTime, 1);
   defMethod(cSprite, "setTile", Sprite_setTile, 4);
}
