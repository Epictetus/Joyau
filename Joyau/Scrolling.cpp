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

void Scrolling::setSprite(char *spr)
{
   bg[0].setPicture(spr);
   bg[1].setPicture(spr);
}

void Scrolling::setPos(int x, int y)
{
   _x = x;
   _y = y;
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
   bg[0].Draw();
   bg[1].Draw();
}

VALUE wrapScrolling(VALUE info)
{
   Scrolling *item = new Scrolling;
   VALUE tdata = Data_Wrap_Struct(info, 0, Scrolling_free, item);
   return tdata;
}

void Scrolling_free(void *info) { delete (Scrolling*)info; }

VALUE Scrolling_setSprite(VALUE self, VALUE spr)
{
   Scrolling *item;
   Data_Get_Struct(self, Scrolling, item);

   char *str = StringValuePtr(spr);
   item->setSprite(str);

   return Qnil;
}

VALUE Scrolling_setPos(VALUE self, VALUE x, VALUE y)
{
   Scrolling *item;
   Data_Get_Struct(self, Scrolling, item);

   int _x = FIX2INT(x);
   int _y = FIX2INT(y);

   item->setPos(_x, _y);

   return Qnil;
}

VALUE Scrolling_setDir(VALUE self, VALUE dir)
{
   Scrolling *item;
   Data_Get_Struct(self, Scrolling, item);

   int _dir = FIX2INT(dir);
   
   item->setDir(_dir);
   return Qnil;
}

VALUE Scrolling_setSpeed(VALUE self, VALUE s)
{
   Scrolling *item;
   Data_Get_Struct(self, Scrolling, item);

   int speed = FIX2INT(s);
   
   item->setSpeed(speed);
   return Qnil;
}

VALUE Scrolling_play(VALUE self)
{
   Scrolling *item;
   Data_Get_Struct(self, Scrolling, item);

   item->play();

   return Qnil;
}

VALUE Scrolling_draw(VALUE self)
{
   Scrolling *item;
   Data_Get_Struct(self, Scrolling, item);

   item->draw();

   return Qnil;
}

void defineScrolling()
{
   VALUE cScroll = rb_define_class("Scrolling", rb_cObject);
   rb_define_singleton_method(cScroll, "new", (VALUE(*)(...))&wrapScrolling,
			      0);
   rb_define_method(cScroll, "setSprite", 
		    (VALUE(*)(...))&Scrolling_setSprite, 1);
   rb_define_method(cScroll, "setPos", 
		    (VALUE(*)(...))&Scrolling_setPos, 2);
   rb_define_method(cScroll, "setDir", 
		    (VALUE(*)(...))&Scrolling_setDir, 1);
   rb_define_method(cScroll, "setSpeed", 
		    (VALUE(*)(...))&Scrolling_setSpeed, 1);
   rb_define_method(cScroll, "play", (VALUE(*)(...))&Scrolling_play, 0);
   rb_define_method(cScroll, "draw", (VALUE(*)(...))&Scrolling_draw, 0);
}
