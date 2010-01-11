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

#ifndef JOYAU_SPRITE
#define JOYAU_SPRITE

#include "Drawable.hpp"

class Buffer;

/** @addtogroup Drawables **/
/*@{*/

/** 
 * @class Sprite
 * An image, which can be animated, ... and which allows
 * to use the same ressource twice.
 */
class Sprite: public Drawable
{
public:
   Sprite():
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
   { setClass("Sprite"); }

   Sprite(const Buffer &buf);

   /** Loads the picture.
    *  @param pic filename.
    */
   void setPicture(char *pic);

   /** Returns the picture's name **/
   std::string getPicName() const { return std::string(picName); }

   /** increase the angle of angle. **/
   void rotate(int angle) { _angle += angle; }

   /** Returns the angle **/
   int getAngle() const { return _angle; }

   /** Changes the angle value **/
   void setAngle(int angle) { _angle = angle; }

   /** Increases the zoom. **/
   void zoom(int increase);

   /** Returns the zoom **/
   int getZoom() const { return _zoom; }

   void move(int x, int y);

   /** Changes the alpha value **/
   void setAlpha(int alpha);

   /** Returns the alpha value **/
   int getAlpha() const;
   
   int getW() const;
   int getH() const;

   virtual void draw();
   
   /** Enumeration containing directions, used in setDirection **/
   enum DIRECTION { 
      DOWN, 
      LEFT, 
      RIGHT, 
      UP, 
      UP_LEFT, 
      UP_RIGHT, 
      DOWN_LEFT,
      DOWN_RIGHT 
   };
   
   /** Changes the used direction **/
   void setDirection(int dir);

   /** Retusn the actual direction **/
   int getDirection() const { return _dir; }
   
   /** Sets how many animation steps there are in the Sprite. **/
   void setAnimation(int nbrX, int nbrY);

   /** sets how many frames there'll be betwenn two animation steps **/
   void setAnimationTime(int t) { nbrAnime = t; }

   /** Enanles tile. The rect Rect(x, y, w, h) will be displayed. **/
   void setTile(int x, int y, int w, int h);
   
   /** Disable tilling. **/
   void unTile() { tiled = false; }

   /** Saves the image in a file.
    *  @param fname file name.
    */
   void saveImage(const char *fname);

   /** Changes autodir value. If it's true, when moving, the
    *  Sprite will guess in which direction it should go.
    */
   void setAutoDir(bool val) { autoDir = val; }

   /** Updates the image, an returns it. **/
    OSL_IMAGE *getImage();
protected:
   char *picName;

   /** Functions which draws the image.**/
   virtual void defaultDraw();

   int _w, _h;
private:
   int _stretchX, _stretchY;
   int _alpha;
   int _zoom;
   int _angle;
   int _dir;

   OSL_IMAGE *sprite;
   
   bool animated;
   int _nbrX;
   int _nbrY;
   int animeState;
   int passedTime;
   int nbrAnime;

   bool tiled;
   int xTile;
   int yTile;
   int wTile;
   int hTile;

   bool autoDir;
};

/*@}*/

VALUE Sprite_setPicture(VALUE self, VALUE pic);
VALUE Sprite_picture(VALUE self);

VALUE Sprite_rotate(VALUE self, VALUE angle);
VALUE Sprite_getAngle(VALUE self);
VALUE Sprite_setAngle(VALUE self, VALUE angle);

VALUE Sprite_zoom(VALUE self,VALUE increase);
VALUE Sprite_getZoom(VALUE self);

VALUE Sprite_setAlpha(VALUE self,VALUE alpha);
VALUE Sprite_getAlpha(VALUE self);

VALUE Sprite_saveFile(VALUE self, VALUE pic);

VALUE Sprite_setDirection(VALUE self, VALUE dir);
VALUE Sprite_getDirection(VALUE self);

VALUE Sprite_setAnimation(VALUE self, VALUE nbrX, VALUE nbrY);
VALUE Sprite_setAnimationTime(VALUE self, VALUE t);

VALUE Sprite_setTile(VALUE self, VALUE x, VALUE y, VALUE w, VALUE h);
VALUE Sprite_unTile(VALUE self);

VALUE Sprite_setAutoDir(VALUE self, VALUE val);

VALUE Sprite_to_buf(VALUE self);

void defineSprite();

#endif
