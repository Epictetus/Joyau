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

#include "Sprite.hpp"
#include "Manager.hpp"
#include "Buffer.hpp"

/*
  Document-class: Joyau::Sprite
  
  This drawable represents an image. You can do lots of manipulations, like
  showing an animated sprite.
*/

template<>
/*
  call-seq: new()
            new(filename)
	    new(buffer)

  Creates a Sprite.
*/
VALUE wrap<Sprite>(int argc, VALUE *argv, VALUE info)
{
   Sprite *ptr = NULL;
   VALUE tdata;
   
   if (argc >= 1) {
      if (rb_obj_is_kind_of(argv[0], getClass("Buffer"))) {
	 ptr = new Sprite(getRef<Buffer>(argv[0]));
      }
      else {
	 argv[0] = rb_obj_as_string(argv[0]);

	 try {
	    ptr = new Sprite;
	    ptr->setPicture(StringValuePtr(argv[0]));
	 }
	 catch (const RubyException &e) {
	    e.rbRaise();
	 }
      }
   }
   else
      ptr = new Sprite;
   
   tdata = Data_Wrap_Struct(info, 0, wrapped_free<Sprite>, ptr);
   return tdata;
}

Sprite::Sprite(const Buffer &buf):
   _alpha(255),
   _zoom(160),
   _angle(0),
   animated(false),
   _nbrX(1),
   _nbrY(1),
   animeState(0),
   passedTime(0),
   nbrAnime(6),
   tiled(false),
   xTile(0),
   yTile(0),
   wTile(0),
   hTile(0),
   autoDir(false)
{
   sprite = buf.img;
   if (!sprite) {
      throw RubyException(rb_eRuntimeError, 
			  "The given buffer is not valid.");
   }

   _w = sprite->sizeX;
   _h = sprite->sizeY;

   _stretchX = sprite->stretchX;
   _stretchY = sprite->stretchY;

   setClass("Sprite");
}

void Sprite::setResName(const std::string &name) {
   picName = name;
}

void Sprite::setPicture(char *pic)
{
   setResName(pic);

   sprite = Manager::getInstance().getPic(pic);
   if (sprite == NULL)
     throw RubyException(rb_eRuntimeError, "the sprite could not be loaded.");

   _w = sprite->sizeX;
   _h = sprite->sizeY;

   _stretchX = sprite->stretchX;
   _stretchY = sprite->stretchY;
}

void Sprite::setBuffer(const Buffer &buffer) {
   sprite = buffer.img;
   if (!sprite) {
      throw RubyException(rb_eRuntimeError, 
			  "The given buffer is not valid.");
   }
}

void Sprite::setAlpha(int alpha)
{
   _alpha = alpha;
}

int Sprite::getAlpha() const
{
   return _alpha;
}

//Since nbrX and nbrY are initialized to 1, we can use them even in
//not-animated sprites. 
int Sprite::getW() const { return tiled ? wTile : _w / _nbrX; }
int Sprite::getH() const { return tiled ? hTile : _h / _nbrY; }

/*
  defaultDraw() can be used in the Draw function from inherited class, 
  so we avoid using private attributes.
 */
void Sprite::draw()
{
   defaultDraw();
}

void Sprite::defaultDraw()
{
   /*
     Since theses images should only be drawn by that class, we may safely
     change theses attributes value.
    */

   if (sprite == NULL)
      return; // Nothing to draw
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

void Sprite::move(int x, int y)
{
   Drawable::move(x, y);

   if (autoDir)
   {
      if (x > 0 && y == 0)
	 setDirection(RIGHT);
      else if (x < 0 && y == 0)
	 setDirection(LEFT);
      else if (x == 0 && y > 0)
	 setDirection(DOWN);
      else if (x == 0 && y < 0)
	 setDirection(UP);
      else if (x > 0 && y > 0)
	 setDirection(DOWN_RIGHT);
      else if (x < 0 && y > 0)
	 setDirection(DOWN_LEFT);
      else if (x > 0 && y < 0)
	 setDirection(UP_RIGHT);
      else if (x < 0 && y < 0)
	 setDirection(UP_LEFT);
   }
}

void Sprite::setTile(int x, int y, int w, int h)
{
   tiled = true;
   xTile = x;
   yTile = y;
   wTile = w;
   hTile = h;

   _w = w;
   _h = h;
}

OSL_IMAGE* Sprite::getImage()
{
   oslSetAlpha(OSL_FX_ALPHA, _alpha);

   sprite->stretchX = _stretchX;
   sprite->stretchY = _stretchY;
   sprite->x = getX();
   sprite->y = getY();
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
   oslWriteImageFile(getImage(), fname, 0);
   oslSetAlpha(OSL_FX_RGBA, _alpha);
}

/*
  call-seq: rotate(angle)

  Rotates the sprite.
*/
VALUE Sprite_rotate(VALUE self, VALUE angle) 
{
   Sprite &item = getRef<Sprite>(self);

   item.rotate(FIX2INT(angle));
   return Qnil;
}

/*
  Returns the sprite's angle.
*/
VALUE Sprite_getAngle(VALUE self) 
{
   Sprite &item = getRef<Sprite>(self);

   return INT2FIX(item.getAngle()); 
}

/*
  call-seq: setAngle(val)

  Sets the sprite's angle.
*/
VALUE Sprite_setAngle(VALUE self, VALUE angle) 
{
   Sprite &item = getRef<Sprite>(self);

   item.setAngle(FIX2INT(angle));

   return angle;
}

/*
  Returns the sprite's zoom.
*/
VALUE Sprite_getZoom(VALUE self)
{
   Sprite &item = getRef<Sprite>(self);

   return INT2FIX(item.getZoom());
}

/*
  Returns the sprite's direction.
*/
VALUE Sprite_getDirection(VALUE self) 
{ 
   Sprite &item = getRef<Sprite>(self);

   return INT2FIX(item.getDirection()); 
}

/*
  call-seq: setAnimation(x, y)

  Sets how many animation steps there are in the sprite.
  This considers that each step has the same size.
*/
VALUE Sprite_setAnimation(VALUE self, VALUE nbrX, VALUE nbrY)
{
   Sprite &item = getRef<Sprite>(self);

   item.setAnimation(FIX2INT(nbrX), FIX2INT(nbrY));
   return Qnil;
}

/*
  call-seq: setAnimTime(frames)

  Sets how many frame the sprite should stay on the same animation.
*/
VALUE Sprite_setAnimationTime(VALUE self, VALUE t)
{
   Sprite &item = getRef<Sprite>(self);

   item.setAnimationTime(FIX2INT(t));
   return t;
}

/*
  call-seq: setPicture(pic)

  Sets the sprite's file. Notice that the same picture won't be loaded
  twice: all the sprites will share that ressource.
*/
VALUE Sprite_setPicture(VALUE self, VALUE pic)
{
   Sprite &item = getRef<Sprite>(self);
   try { 
      item.setPicture(StringValuePtr(pic)); 
   }
   catch(const RubyException &e) {
      e.rbRaise();
   }

   return pic;
}

/*
  call-seq: buffer=(buf)

  Changes the sprite's buffer directly.
*/
VALUE Sprite_setBuffer(VALUE self, VALUE buf) {
   if (!rb_obj_is_kind_of(buf, getClass("Buffer")))
      rb_raise(rb_eTypeError, "Can't convert %s into Joyau::Buffer",
	       rb_obj_classname(buf));
   Sprite &ref = getRef<Sprite>(self);
   try {
      ref.setBuffer(getRef<Buffer>(buf));
   }
   catch (const RubyException &e) {
      e.rbRaise();
   }

   return buf;
}

/*
  call-seq: res_name=(val)

  Sets the sprite's ressource name. This might be useful when used along
  with Buffers.
*/
VALUE Sprite_setResName(VALUE self, VALUE pic) {
   Sprite &ref = getRef<Sprite>(self);
   
   pic = rb_obj_as_string(pic);
   ref.setResName(StringValuePtr(pic));

   return pic;
}

/*
  Returns the sprite's picture name.
*/
VALUE Sprite_picture(VALUE self)
{
   Sprite &ref = getRef<Sprite>(self);
   return rb_str_new2(ref.getPicName().c_str());
}

/*
  call-seq: setAlpga(val)

  Sets the sprite's transparency.
*/
VALUE Sprite_setAlpha(VALUE self, VALUE alpha)
{
   Sprite &item = getRef<Sprite>(self);
   item.setAlpha(FIX2INT(alpha));

   return alpha;
}

/*
  Returns the sprite's transparency.
*/
VALUE Sprite_getAlpha(VALUE self)
{
   Sprite &item = getRef<Sprite>(self);

   return INT2FIX(item.getAlpha());
}

/*
  call-seq: setDirection(dir)

  Sets the sprite's direction.
*/
VALUE Sprite_setDirection(VALUE self, VALUE dir)
{
   Sprite &item = getRef<Sprite>(self);
   item.setDirection(FIX2INT(dir));

   return dir;
}

/*
  call-seq: zoom(val)

  Increase or decrease the zooming level.
*/
VALUE Sprite_zoom(VALUE self, VALUE val)
{
   Sprite &item = getRef<Sprite>(self);
   item.zoom(FIX2INT(val));
   
   return Qnil; 
}

/*
  call-seq: setTile(x, y, w, h)

  Sets the rect which is shown.
*/
VALUE Sprite_setTile(VALUE self, VALUE x, VALUE y, VALUE w, VALUE h)
{
   Sprite &item = getRef<Sprite>(self);
   item.setTile(FIX2INT(x), FIX2INT(y), FIX2INT(w), FIX2INT(h));

   return Qnil;
}

/*
  Disable the tilling effect.
 */
VALUE Sprite_unTile(VALUE self)
{
   Sprite &ref = getRef<Sprite>(self);
   ref.unTile();

   return Qnil;
}

/*
  call-seq: saveFile(filename)

  Saves the sprite in a file.
*/
VALUE Sprite_saveFile(VALUE self, VALUE pic)
{
   Sprite &item = getRef<Sprite>(self);
   const char *file = StringValuePtr(pic);

   item.saveImage(file);
   return Qnil;
}

/*
  Sets the autoDir value. When true, the sprite's direction changes
  according to its moves.
*/
VALUE Sprite_setAutoDir(VALUE self, VALUE val)
{
   Sprite &ref = getRef<Sprite>(self);
   ref.setAutoDir(val == Qtrue);

   return val;
}

/*
  Converts the sprite in a buffer.
*/
VALUE Sprite_to_buf(VALUE self) {
   Sprite &ref = getRef<Sprite>(self);
   
   try {
      return Data_Wrap_Struct(getClass("Buffer"), 0, wrapped_free<Buffer>,
			      new Buffer(ref));
   }
   catch (const RubyException &e) {
      e.rbRaise();
      return Qnil;
   }
}

/*
  This function has got a bang in its name, because it is a very dangerous
  variant of to_buf: It returns the buffer used by the Sprite. Which means
  you might be modifying a ressource which is used at multiple places in your
  program. Use it with care.
*/
VALUE Sprite_to_buf2(VALUE self) {
   Sprite &ref = getRef<Sprite>(self);
   return Data_Wrap_Struct(getClass("Buffer"), 0, wrapped_free<Buffer>,
			   new Buffer(ref.getImage()));
}

void defineSprite()
{
   VALUE cSprite = defClass<Sprite>("Sprite", "Drawable");
   defMethod(cSprite, "setPicture", Sprite_setPicture, 1);
   defMethod(cSprite, "buffer=", Sprite_setBuffer, 1);
   defMethod(cSprite, "res_name=", Sprite_setResName, 1);
   defMethod(cSprite, "picture", Sprite_picture, 0);
   defMethod(cSprite, "getAngle", Sprite_getAngle, 0);
   defMethod(cSprite, "setAngle", Sprite_setAngle, 1);
   defMethod(cSprite, "zoom", Sprite_zoom, 1);
   defMethod(cSprite, "getZoom", Sprite_getZoom, 0);
   defMethod(cSprite, "setAlpha", Sprite_setAlpha, 1);
   defMethod(cSprite, "getAlpha", Sprite_getAlpha, 0);
   defMethod(cSprite, "saveFile", Sprite_saveFile, 1);
   defMethod(cSprite, "setDirection", Sprite_setDirection, 1);
   defMethod(cSprite, "getDirection", Sprite_getDirection, 0);
   defMethod(cSprite, "setAnim", Sprite_setAnimation, 2);
   defMethod(cSprite, "setAnimTime", Sprite_setAnimationTime, 1);
   defMethod(cSprite, "setTile", Sprite_setTile, 4);
   defMethod(cSprite, "unTile", Sprite_unTile, 0);
   defMethod(cSprite, "autoDir=", Sprite_setAutoDir, 1);

   defMethod(cSprite, "to_buf", Sprite_to_buf, 0);
   defMethod(cSprite, "to_buf!", Sprite_to_buf2, 0);

   defAlias(cSprite, "getAngle", "angle");
   defAlias(cSprite, "setAngle", "angle=");
   defAlias(cSprite, "getAlpha", "alpha");
   defAlias(cSprite, "setAlpha", "alpha=");
   defAlias(cSprite, "getDirection", "direction");
   defAlias(cSprite, "setDirection", "direction=");
   defAlias(cSprite, "getDirection", "dir");
   defAlias(cSprite, "setDirection", "dir=");
   defAlias(cSprite, "setPicture", "picture=");
   
   defConst(cSprite, "LEFT", INT2FIX(Sprite::LEFT));
   defConst(cSprite, "RIGHT", INT2FIX(Sprite::RIGHT));
   defConst(cSprite, "UP", INT2FIX(Sprite::UP));
   defConst(cSprite, "DOWN", INT2FIX(Sprite::DOWN));
   defConst(cSprite, "UP_LEFT", INT2FIX(Sprite::UP_LEFT));
   defConst(cSprite, "UP_RIGHT", INT2FIX(Sprite::UP_RIGHT));
   defConst(cSprite, "DOWN_LEFT", INT2FIX(Sprite::DOWN_LEFT));
   defConst(cSprite, "DOWN_RIGHT", INT2FIX(Sprite::DOWN_RIGHT));
}
