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

#include "Scrolling.hpp"

/*
  Document-class: Joyau::Scrolling

  Class which displays a Sprite twice, making it scroll in one of the four
  direction.
*/

template<>
/*
  call-seq: new
            new(filename)

  Creates a new Scrolling. You can eventually load a picture during this
  creation.
*/
VALUE wrap<Scrolling>(int argc, VALUE *argv, VALUE info)
{
   Scrolling *ptr = new Scrolling;

   if (argc >= 1)
      ptr->setSprite(StringValuePtr(argv[0]));
   
   VALUE tdata = Data_Wrap_Struct(info, 0, wrapped_free<Scrolling>, ptr);
   return tdata;
}

void Scrolling::setSprite(char *spr)
{
   bg[0].setPicture(spr);
   bg[1].setPicture(spr);
}

void Scrolling::setDir(int dir)
{
   _dir = dir;
   if (_dir == Sprite::RIGHT)
   {
      bg[0].setPos(getX(), getY());
      bg[1].setPos(getX() - 480, getY());
   }
   else if (_dir == Sprite::LEFT)
   {
      bg[0].setPos(getX(), getY());
      bg[1].setPos(getX() + 480, getY());
   }
   else if (_dir == Sprite::UP)
   {
      bg[0].setPos(getX(), getY());
      bg[1].setPos(getX(), getY() + 272);
   }
   else if (_dir == Sprite::DOWN)
   {
      bg[0].setPos(getX(), getY());
      bg[1].setPos(getX(), getY() - 272);
   }
}

int Scrolling::getW() const
{
   if (_dir == Sprite::RIGHT || _dir == Sprite::LEFT)
      return bg[0].getW() * 2;
   return bg[0].getW();
}

int Scrolling::getH() const
{
   if (_dir == Sprite::UP || _dir == Sprite::DOWN)
      return bg[0].getH() * 2;
   return bg[0].getW();
}

void Scrolling::play()
{
   if (_dir == Sprite::RIGHT)
   {
      if (bg[0].getX() >= 480)
	 bg[0].setPos(bg[1].getX() - bg[1].getW(), bg[1].getY());
      else if (bg[1].getX() >= 480)
	 bg[1].setPos(bg[0].getX() - bg[0].getW(), bg[0].getY());
      bg[0].move(speed, 0);
      bg[1].move(speed, 0);
   }
   else if (_dir == Sprite::LEFT)
   {
      if (bg[0].getX() <=  0 - bg[0].getW())
	 bg[0].setPos(bg[1].getX() + bg[1].getW(), bg[1].getY());
      else if (bg[1].getX() <= 0 - bg[1].getW())
	 bg[1].setPos(bg[0].getX() + bg[0].getW(), bg[0].getY());
      bg[0].move(-speed, 0);
      bg[1].move(-speed, 0);
   }
   else if (_dir == Sprite::DOWN)
   {
      if (bg[0].getY() >=  272)
	 bg[0].setPos(bg[1].getX(), bg[1].getY() - bg[1].getW());
      else if (bg[1].getY() >= 272)
	 bg[1].setPos(bg[0].getX(), bg[0].getY() - bg[1].getH());
      bg[0].move(0, speed);
      bg[1].move(0, speed);
   }
   else if (_dir == Sprite::UP)
   {
      if (bg[0].getY() <=  0 - bg[0].getH())
	 bg[0].setPos(bg[1].getX(), bg[1].getY() + bg[1].getW());
      else if (bg[1].getY() <= 0 - bg[1].getH())
	 bg[1].setPos(bg[0].getX(), bg[0].getY() + bg[1].getH());
      bg[0].move(0, -speed);
      bg[1].move(0, -speed);
   }
}

void Scrolling::draw()
{
   bg[0].draw();
   bg[1].draw();
}

void Scrolling::move(int x, int y)
{
   Drawable::move(x, y);

   for (int i = 0; i < 2; ++i)
      bg[i].move(x, y);
}

void Scrolling::clearMove()
{
   movedX = 0;
   movedY = 0;
   bg[0].clearMove();
   bg[1].clearMove();
}

/*
  call-seq: setSprite(filename)

  Loads a picture for the scrolling.
*/
VALUE Scrolling_setSprite(VALUE self, VALUE spr)
{
   Scrolling &ref = getRef<Scrolling>(self);
   char *str = StringValuePtr(spr);

   ref.setSprite(str);
   return spr;
}

/*
  call-seq: setDir(dir)

  Sets the scrolling direction
*/
VALUE Scrolling_setDir(VALUE self, VALUE dir)
{
   Scrolling &ref = getRef<Scrolling>(self);
   int _dir = FIX2INT(dir);
   
   ref.setDir(_dir);
   return dir;
}

/*
  call-seq: setSpeed(s)

  Sets the scrolling speed.
  The speed is the distance that is done by the scrolling at each call to
  play.
*/
VALUE Scrolling_setSpeed(VALUE self, VALUE s)
{
   Scrolling &ref = getRef<Scrolling>(self);
   int speed = FIX2INT(s);
   
   ref.setSpeed(speed);
   return s;
}

/*
  Returns the scrolling's directory.
*/
VALUE Scrolling_dir(VALUE self)
{
   Scrolling &ref = getRef<Scrolling>(self);
   return INT2FIX(ref.getDir());
}

/*
  Returns the scrolling's speed.
*/
VALUE Scrolling_speed(VALUE self)
{
   Scrolling &ref = getRef<Scrolling>(self);
   return INT2FIX(ref.getSpeed());
}

/*
  Moves the scrolling,a nd wraps it automatically.
*/
VALUE Scrolling_play(VALUE self)
{
   Scrolling &ref = getRef<Scrolling>(self);

   ref.play();
   return Qnil;
}

void defineScrolling()
{
   VALUE cScroll = defClass<Scrolling>("Scrolling", "Drawable");
   defMethod(cScroll, "setSprite", Scrolling_setSprite, 1);
   defMethod(cScroll, "setDir", Scrolling_setDir, 1);
   defMethod(cScroll, "setSpeed", Scrolling_setSpeed, 1);
   defMethod(cScroll, "dir", Scrolling_dir, 0);
   defMethod(cScroll, "speed", Scrolling_speed, 0); 
   defMethod(cScroll, "play", Scrolling_play, 0);

   defAlias(cScroll, "setSprite", "sprite=");
   defAlias(cScroll, "setDir", "dir=");
   defAlias(cScroll, "dir", "direction");
   defAlias(cScroll, "setDir", "direction=");
   defAlias(cScroll, "setSpeed", "speed=");

   defConst(cScroll, "LEFT", INT2FIX(Sprite::LEFT));
   defConst(cScroll, "RIGHT", INT2FIX(Sprite::RIGHT));
   defConst(cScroll, "UP", INT2FIX(Sprite::UP));
   defConst(cScroll, "DOWN", INT2FIX(Sprite::DOWN));
}
