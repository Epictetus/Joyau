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

#ifndef JOYAU_BUFFER
#define JOYAU_BUFFER

#include "StdInclude.hpp"
#include "Sprite.hpp"

class Buffer: public RubyObject {
public:
   Buffer(int w, int h, int format);
   Buffer(const Buffer &obj);
   Buffer(Drawable &obj);
   Buffer(Sprite &obj);
   Buffer(OSL_IMAGE *img);
   ~Buffer();

   friend Sprite::Sprite(const Buffer &buf);

   void setActual();

   void draw();
   void draw(Drawable &obj);
   void draw(Buffer &obj);

   void clear(OSL_COLOR color);

   int getX() const;
   int getY() const;

   int getW() const;
   int getH() const;

   void setX(int x);
   void setY(int y);

   void setPos(int x, int y);
   void move(int x, int y);

   void resize(int w, int h);
   void zoom(int level);
   void rotate(int angle);

   void lock();
   void unlock();

   OSL_COLOR getPixel(int x, int y);
   void setPixel(int x, int y, OSL_COLOR col);

   void save(const std::string &filename);

   static void updateScreen() {
      if (screen)
         delete screen;
      if (default_buf)
	 delete default_buf;
      
      screen = new Buffer(OSL_SECONDARY_BUFFER);
      default_buf = new Buffer(OSL_DEFAULT_BUFFER);
   }

   static Buffer *getScreen() {
      return screen;
   }

   static Buffer *getDefault() {
      return default_buf;
   }

   static void destroyScreen() {
      if (screen)
	 delete screen;
      if (default_buf)
	 delete default_buf;
      
      screen = NULL;
      default_buf = NULL;
   }
private:
   OSL_IMAGE *img;
   bool shouldDelete;

   static Buffer *screen;
   static Buffer *default_buf;
};

class Painter {
public:
   Painter(Buffer &obj);

   void drawLine(int x1, int y1, int x2, int y2,
                 OSL_COLOR col, OSL_COLOR col2);

   void drawFillRect(int x1, int y1, int x2, int y2,
                     OSL_COLOR col, OSL_COLOR col2,
                     OSL_COLOR col3, OSL_COLOR col4);

   void drawRect(int x1, int y1, int x2, int y2, OSL_COLOR col);

   void drawPoint(int x, int y, OSL_COLOR col);

   void drawFillCircle(int x, int y, int r, OSL_COLOR col);

   void drawCircle(int x, int y, int r, OSL_COLOR col);

   void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3,
                     OSL_COLOR col, OSL_COLOR col2, OSL_COLOR col3);

   void drawBuffer(int x, int y, Buffer &obj);

   void clear(OSL_COLOR col);
private:
   Buffer &buf;
};

VALUE Buffer_setActual(VALUE self);

VALUE Buffer_draw(int argc, VALUE *argv, VALUE self);

VALUE Buffer_clear(VALUE self, VALUE color);

VALUE Buffer_getX(VALUE self);
VALUE Buffer_getY(VALUE self);

VALUE Buffer_getW(VALUE self);
VALUE Buffer_getH(VALUE self);

VALUE Buffer_setX(VALUE self, VALUE x);
VALUE Buffer_setY(VALUE self, VALUE y);

VALUE Buffer_setPos(int argc, VALUE *argv, VALUE self);
VALUE Buffer_move(VALUE self, VALUE x, VALUE y);

VALUE Buffer_resize(VALUE self, VALUE w, VALUE h);
VALUE Buffer_zoom(VALUE self, VALUE level);
VALUE Buffer_rotate(VALUE self, VALUE angle);

VALUE Buffer_lock(int argc, VALUE *argv, VALUE self);
VALUE Buffer_unlock(VALUE self);

VALUE Buffer_getPixel(VALUE self, VALUE x, VALUE y);
VALUE Buffer_setPixel(VALUE self, VALUE x, VALUE y, VALUE col);

VALUE Buffer_save(VALUE self, VALUE filename);

VALUE Buffer_to_sprite(VALUE self);

VALUE Buffer_getScreen(VALUE self);
VALUE Buffer_getDefault(VALUE self);
VALUE Buffer_getActual(VALUE self);

VALUE Painter_drawLine(int argc, VALUE *argv, VALUE self);
VALUE Painter_drawFillRect(int argc, VALUE *argv, VALUE self);
VALUE Painter_drawRect(int argc, VALUE *argv, VALUE self);
VALUE Painter_drawPoint(VALUE self, VALUE x, VALUE y, VALUE col);
VALUE Painter_drawFillCircle(VALUE self, VALUE x, VALUE y, VALUE r, VALUE col);
VALUE Painter_drawCircle(VALUE self, VALUE x, VALUE y, VALUE r, VALUE col);
VALUE Painter_drawTriangle(int argc, VALUE *argv, VALUE self);
VALUE Painter_drawBuffer(VALUE self, VALUE x, VALUE y, VALUE buf);

VALUE Painter_clear(VALUE self, VALUE col);

VALUE Joyau_draw(int argc, VALUE *argv, VALUE self);

void defineBuffer();

#endif
