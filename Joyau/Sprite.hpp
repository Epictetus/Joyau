#ifndef __SPRITE__
#define __SPRITE__

#include "Manager.hpp"

#include "Manager.hpp"

class Sprite
{
public:
   Sprite(Manager *a_manager):
      _alpha(255),
      _zoom(160),
      _angle(0)
   {
      manager = a_manager;
      _x = 0;
      _y = 0;
   }

   void setPicture(char *pic);

   void setPos(int x, int y);
   void move(int x, int y);

   void rotate(int angle) { _angle += angle; }
   int getAngle() { return _angle; }
   void setAngle(int angle) { _angle = angle; }
   void zoom(int increase);
   int getZoom() { return _zoom; }

   void setAlpha(int alpha);
   int getAlpha();
   
   int getX();
   int getY();
   int getW();
   int getH();

   bool isOn(int x, int y);

   bool collide(Sprite *spr);

   virtual void Draw();
   virtual void play() {}

   enum DIRECTION { DOWN, LEFT, RIGHT, UP, UP_LEFT, UP_RIGHT, DOWN_LEFT,
		    DOWN_RIGHT };

   void setDirection(int dir);
   int getDirection() { return _dir; }

   virtual int getPv() { return 0; }
   virtual int getScore() { return 0; }

protected:
   string picName;
   Manager *manager;

   void defaultDraw();
private:
   int _x, _y;
   int _w, _h;
   int _stretchX, _stretchY;
   int _alpha;
   int _zoom;
   int _angle;
   int _dir;

   OSL_IMAGE *sprite;
};

VALUE wrapSprite(VALUE info);

VALUE Sprite_setPicture(VALUE self, VALUE pic);

VALUE Sprite_setPos(VALUE self, VALUE x, VALUE y);
VALUE Sprite_move(VALUE self, VALUE x, VALUE y);

VALUE Sprite_rotate(VALUE self, VALUE angle);
VALUE Sprite_getAngle(VALUE self);
VALUE Sprite_setAngle(VALUE self, VALUE angle);

VALUE Sprite_zoom(VALUE self,VALUE increase);
VALUE Sprite_getZoom(VALUE self);

VALUE Sprite_setAlpha(VALUE self,VALUE alpha);
VALUE Sprite_getAlpha(VALUE self);

VALUE Sprite_getX(VALUE self);
VALUE Sprite_getY(VALUE self);
VALUE Sprite_getW(VALUE self);
VALUE Sprite_getH(VALUE self);

VALUE Sprite_isOn(VALUE self, VALUE x, VALUE y);
VALUE Sprite_collide(VALUE self, VALUE spr);

VALUE Sprite_draw(VALUE self);

enum DIRECTION { DOWN, LEFT, RIGHT, UP, UP_LEFT, UP_RIGHT, DOWN_LEFT,
		 DOWN_RIGHT };

VALUE Sprite_setDirection(VALUE self, VALUE dir);
VALUE Sprite_getDirection(VALUE self);

void Sprite_free(void*);
void defineSprite();

#endif
