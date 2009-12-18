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

#include "Buffer.hpp"
#include "Drawable.hpp"
#include "Line.hpp"
#include "DrawableRect.hpp"
#include "Circle.hpp"
#include "Triangle.hpp"
#include "RubyDrawable.hpp"
#include "Graphics.hpp"
#include "Sprite.hpp"

Buffer *Buffer::screen = NULL;

Buffer::Buffer(int w, int h, int format): shouldDelete(true) {
   setClass("Buffer");

   if (w > 512 || h > 512)
      throw RubyException(rb_eRuntimeError, 
			  "Either width or height is greter than 512.");

   img = oslCreateImage(w, h, OSL_IN_VRAM, format);
   if (!img)
      throw RubyException(rb_eRuntimeError, "Buffer could not be created.");
}

Buffer::Buffer(OSL_IMAGE *arg):
   img(arg), shouldDelete(false)
{ setClass("Buffer"); }

Buffer::Buffer(const Buffer &obj): shouldDelete(true) {
   setClass("Buffer");

   img = oslCreateImageCopy(obj.img, OSL_IN_VRAM);
   if (!img)
      throw RubyException(rb_eRuntimeError, "Buffer could not be copied.");
}

Buffer::Buffer(Drawable &obj): shouldDelete(true) {
   setClass("Buffer");

   if (obj.getW() > 512 || obj.getH() > 512)
      throw RubyException(rb_eRuntimeError, "Drawable too big for a buffer.");
   
   img = oslCreateImage(obj.getW(), obj.getH(), OSL_IN_VRAM, OSL_PF_8888);
   if (!img)
      throw RubyException(rb_eRuntimeError, "Buffer could not be created");
   
   OSL_IMAGE *old = oslGetDrawBuffer();
   setActual();

   obj.clearMove();
   obj.setPos(0, 0);
   obj.draw();
   obj.cancelMove();

   oslSetDrawBuffer(old);
}

Buffer::Buffer(Sprite &obj): shouldDelete(true) {
   setClass("Buffer");

   img = oslCreateImageCopy(obj.getImage(), OSL_IN_VRAM);
   if (!img)
      throw RubyException(rb_eRuntimeError, "Buffer could not be created");
}

Buffer::~Buffer() {
   if (img && shouldDelete)
      oslDeleteImage(img);
}

void Buffer::setActual() {
   oslSetDrawBuffer(img);
}

void Buffer::draw() {
   oslDrawImage(img);
}

void Buffer::draw(Drawable &obj) {
   OSL_IMAGE *old = oslGetDrawBuffer();
   setActual();
   obj.draw();
   oslSetDrawBuffer(old);
}

void Buffer::draw(Buffer &obj) {
   OSL_IMAGE *old = oslGetDrawBuffer();
   setActual();
   obj.draw();
   oslSetDrawBuffer(old);
}

void Buffer::clear(OSL_COLOR color) {
   oslClearImage(img, color);
}

int Buffer::getX() const {
   return img->x;
}

int Buffer::getY() const {
   return img->y;
}

int Buffer::getW() const {
   return img->sizeX;
}

int Buffer::getH() const {
   return img->sizeY;
}
   
void Buffer::setX(int x) {
   move(x - getX(), 0);
}

void Buffer::setY(int y) {
   move(0, y - getY());
}

void Buffer::setPos(int x, int y) {
   move(x - getX(), y - getY());
}

void Buffer::move(int x, int y) {
   img->x += x;
   img->y += y;
}

void Buffer::resize(int w, int h) {
   OSL_IMAGE *old_image = img;
   OSL_IMAGE *old_buffer = oslGetDrawBuffer();
   
   img = oslCreateImage(w, h, OSL_IN_VRAM, img->pixelFormat);
   if (!img)
      throw RubyException(rb_eRuntimeError, "Could not recreate the buffer.");
   
   setActual();
   oslDrawImageXY(old_image, 0, 0);
   oslSetDrawBuffer(old_buffer);

   oslDeleteImage(old_image);
}

void Buffer::zoom(int level) {
   img->stretchX += level;
   img->stretchY += level;
}

void Buffer::rotate(int angle) {
   img->angle += angle;
}

void Buffer::lock() {
   oslLockImage(img);
}

void Buffer::unlock() {
   oslUnlockImage(img);
}

OSL_COLOR Buffer::getPixel(int x, int y) {
   return oslConvertColor(OSL_PF_8888, img->pixelFormat, 
			  oslGetImagePixel(img, x, y));
}

void Buffer::setPixel(int x, int y, OSL_COLOR col) {
   oslSetImagePixel(img, x, y, 
		    oslConvertColor(img->pixelFormat, OSL_PF_8888, col));
}

Painter::Painter(Buffer &obj): buf(obj) {}

void Painter::drawLine(int x1, int y1, int x2, int y2,
                       OSL_COLOR col, OSL_COLOR col2) {
   Line line;
   line.setPos(x1, y1);
   line.setPoint(x2, y2);
   OSL_COLOR grad[2] = { col, col2 };
   line.setGradient(grad);

   buf.draw(line);
}

void Painter::drawFillRect(int x1, int y1, int x2, int y2,
                       OSL_COLOR col, OSL_COLOR col2,
                       OSL_COLOR col3, OSL_COLOR col4) {
   DrawableRect rect;
   rect.setPos(x1, y1);
   rect.setCorner(x2, y2);
   OSL_COLOR grad[4] = { col, col2, col3, col4 };
   rect.setGradient(grad);

   buf.draw(rect);
}

void Painter::drawRect(int x1, int y1, int x2, int y2, OSL_COLOR col) {
   DrawableRect rect;
   rect.setFilled(false);
   rect.setPos(x1, y1);
   rect.setCorner(x2, y2);
   rect.setColor(col);

   buf.draw(rect);
}

void Painter::drawPoint(int x, int y, OSL_COLOR col) {
   DrawableRect rect;
   rect.setPos(x, y);
   rect.resize(1, 1);
   rect.setColor(col);

   buf.draw(rect);
}

void Painter::drawFillCircle(int x, int y, int r, OSL_COLOR col) {
   Circle circle;
   circle.setCenter(x, y);
   circle.setRadius(r);
   circle.setColor(col);

   buf.draw(circle);
}

void Painter::drawCircle(int x, int y, int r, OSL_COLOR col) {
   Circle circle;
   circle.setFilled(false);
   circle.setCenter(x, y);
   circle.setRadius(r);
   circle.setColor(col);

   buf.draw(circle);
}

void Painter::drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3,
                           OSL_COLOR col, OSL_COLOR col2, OSL_COLOR col3) {
   Triangle triangle;
   triangle.setPoints(x1, y1, x2, y2, x3, y3);
   OSL_COLOR grad[3] = { col, col2, col3 };
   triangle.setGradient(grad);

   buf.draw(triangle);
}

void Painter::drawBuffer(int x, int y, Buffer &obj) {
   int oldX = obj.getX();
   int oldY = obj.getY();

   obj.setPos(x, y);
   buf.draw(obj);

   obj.setPos(oldX, oldY);
}

void Painter::clear(OSL_COLOR col) {
   buf.clear(col);
}

template<> VALUE wrap<Buffer>(int argc, VALUE *argv, VALUE info)
{
   VALUE arg1, arg2, arg3;
   rb_scan_args(argc, argv, "12", &arg1, &arg2, &arg3);

   Buffer *ptr;

   try {
      if (NIL_P(arg2)) {
	 if (rb_obj_is_kind_of(arg1, getClass("Drawable")) == Qtrue) {
	    RubyDrawable drawable(arg1);
	    ptr = new Buffer(drawable);
	 }
	 else if (rb_obj_is_kind_of(arg1, getClass("Buffer")) == Qtrue) {
	    ptr = new Buffer(getRef<Buffer>(arg1));
	 }
	 else {
	    rb_raise(rb_eTypeError, "Buffer or Drawable expected.");
	 }
      }
      else {
	 if (NIL_P(arg2))
	    rb_raise(rb_eArgError, "Another argument was expected.");
	 if (NIL_P(arg3))
	    arg3 = INT2FIX(OSL_PF_5650);
	 ptr = new Buffer(FIX2INT(arg1), FIX2INT(arg2), FIX2INT(arg3));
      }
   }
   catch (const RubyException &e) {
      e.rbRaise();
   }
   
   VALUE tdata = Data_Wrap_Struct(info, 0, wrapped_free<Buffer>, ptr);
   return tdata;
}

VALUE Buffer_setActual(VALUE self) {
   Buffer &ref = getRef<Buffer>(self);
   ref.setActual();

   return Qnil;
}

VALUE Buffer_draw(int argc, VALUE *argv, VALUE self) {
   Buffer &ref = getRef<Buffer>(self);

   VALUE arg;
   rb_scan_args(argc, argv, "01", &arg);

   if (NIL_P(arg)) // We'll render the buffer.
      ref.draw();
   else {
      if (rb_obj_is_kind_of(arg, getClass("Drawable")) == Qtrue) {
	 RubyDrawable drawable(arg);
	 ref.draw(drawable);
      }
      else if (rb_obj_is_kind_of(arg, getClass("Buffer")) == Qtrue) {
	 Buffer &obj = getRef<Buffer>(arg);
	 ref.draw(obj);
      }
      else {
	 rb_raise(rb_eTypeError, "Buffer or Drawable expected.");
      }
   }
   return Qnil;
}

VALUE Buffer_clear(VALUE self, VALUE color) {
   Buffer &ref = getRef<Buffer>(self);
   ref.clear(hash2col(color));

   return Qnil;
}

VALUE Buffer_getX(VALUE self) {
   Buffer &ref = getRef<Buffer>(self);
   return INT2FIX(ref.getX());
}

VALUE Buffer_getY(VALUE self) {
   Buffer &ref = getRef<Buffer>(self);
   return INT2FIX(ref.getY());
}

VALUE Buffer_getW(VALUE self) {
   Buffer &ref = getRef<Buffer>(self);
   return INT2FIX(ref.getW());
}

VALUE Buffer_getH(VALUE self) {
   Buffer &ref = getRef<Buffer>(self);
   return INT2FIX(ref.getH());
}

VALUE Buffer_setX(VALUE self, VALUE x) {
   Buffer &ref = getRef<Buffer>(self);
   ref.setX(FIX2INT(x));

   return x;
}

VALUE Buffer_setY(VALUE self, VALUE y) {
   Buffer &ref = getRef<Buffer>(self);
   ref.setY(FIX2INT(y));

   return y;
}

VALUE Buffer_setPos(int argc, VALUE *argv, VALUE self) {
   Buffer &ref = getRef<Buffer>(self);
   
   VALUE arg1, arg2;
   rb_scan_args(argc, argv, "11", &arg1, &arg2);

   if (NIL_P(arg2)) {
      Point &point = getRef<Point>(arg1);
      ref.setPos(point.x, point.y);
   }
   else {
      int x = FIX2INT(arg1), y = FIX2INT(arg2);
      ref.setPos(x, y);
   }

   return Qnil;
}

VALUE Buffer_move(VALUE self, VALUE x, VALUE y) {
   Buffer &ref = getRef<Buffer>(self);
   ref.move(FIX2INT(x), FIX2INT(y));

   return Qnil;
}

VALUE Buffer_resize(VALUE self, VALUE w, VALUE h) {
   Buffer &ref = getRef<Buffer>(self);
   try {
      ref.resize(FIX2INT(w), FIX2INT(h));
   }
   catch (const RubyException &e) {
      e.rbRaise();
   }

   return Qnil;
}

VALUE Buffer_zoom(VALUE self, VALUE level) {
   Buffer &ref = getRef<Buffer>(self);
   ref.zoom(FIX2INT(level));

   return Qnil;
}

VALUE Buffer_rotate(VALUE self, VALUE angle) {
   Buffer &ref = getRef<Buffer>(self);
   ref.rotate(FIX2INT(angle));

   return Qnil;
}

VALUE Buffer_lock(int argc, VALUE *argv, VALUE self) {
   Buffer &ref = getRef<Buffer>(self);
   
   VALUE block;
   rb_scan_args(argc, argv, "00&", &block);

   ref.lock();
   if (!NIL_P(block)) {
      rb_yield(Qnil);
      ref.unlock();
   }

   return Qnil;
}

VALUE Buffer_unlock(VALUE self) {
   Buffer &ref = getRef<Buffer>(self);
   ref.unlock();
   
   return Qnil;
}

VALUE Buffer_getPixel(VALUE self, VALUE x, VALUE y) {
   Buffer &ref = getRef<Buffer>(self);
   return col2hash(ref.getPixel(FIX2INT(x), FIX2INT(y)));
}

VALUE Buffer_setPixel(VALUE self, VALUE x, VALUE y, VALUE col) {
   Buffer &ref = getRef<Buffer>(self);
   ref.setPixel(FIX2INT(x), FIX2INT(y), hash2col(col));

   return col;
}

VALUE Buffer_to_sprite(VALUE self) {
   Buffer &ref = getRef<Buffer>(self);
   return Data_Wrap_Struct(getClass("Sprite"), 0, wrapped_free<Sprite>, 
			   new Sprite(ref));
}

VALUE Buffer_updateScreen(VALUE self) {
   Buffer::updateScreen();
   return Qnil;
}

VALUE Buffer_getScreen(VALUE self) {
   return Data_Wrap_Struct(getClass("Buffer"), 0, &no_free, Buffer::getScreen());
}

VALUE Buffer_destroyScreen(VALUE self) {
   Buffer::destroyScreen();
   return Qnil;
}

template<> VALUE wrap<Painter>(int argc, VALUE *argv, VALUE info)
{
   VALUE buffer;
   rb_scan_args(argc, argv, "1", &buffer);

   Painter *ptr = new Painter(getRef<Buffer>(buffer));
   VALUE tdata = Data_Wrap_Struct(info, 0, wrapped_free<Painter>, ptr);
   return tdata;
}

VALUE Painter_drawLine(int argc, VALUE *argv, VALUE self) {
   Painter &ref = getRef<Painter>(self);
   
   VALUE x1, y1, x2, y2, col1, col2;
   rb_scan_args(argc, argv, "51", &x1, &y1, &x2, &y2, &col1, &col2);
   
   if (NIL_P(col2))
      col2 = col1;

   ref.drawLine(FIX2INT(x1), FIX2INT(y1), FIX2INT(x2), FIX2INT(y2),
		hash2col(col1), hash2col(col2));
   return Qnil;
}

VALUE Painter_drawFillRect(int argc, VALUE *argv, VALUE self) {
   Painter &ref = getRef<Painter>(self);
   
   VALUE x1, y1, x2, y2, col1, col2, col3, col4;
   rb_scan_args(argc, argv, "53", &x1, &y1, &x2, &y2, 
		&col1, &col2, &col3, &col4);
   
   if (NIL_P(col2))
      col2 = col1;
   if (NIL_P(col3))
      col3 = col2;
   if (NIL_P(col4))
      col4 = col3;

   ref.drawFillRect(FIX2INT(x1), FIX2INT(y1), FIX2INT(x2), FIX2INT(y2),
		    hash2col(col1), hash2col(col2), 
		    hash2col(col3), hash2col(col4));
   return Qnil;
}

VALUE Painter_drawRect(int argc, VALUE *argv, VALUE self) {
   Painter &ref = getRef<Painter>(self);
   
   VALUE x1, y1, x2, y2, col;
   rb_scan_args(argc, argv, "5", &x1, &y1, &x2, &y2, &col);

   ref.drawRect(FIX2INT(x1), FIX2INT(y1), FIX2INT(x2), FIX2INT(y2),
		hash2col(col));
   return Qnil;
}

VALUE Painter_drawPoint(VALUE self, VALUE x, VALUE y, VALUE col) {
   Painter &ref = getRef<Painter>(self);
   ref.drawPoint(FIX2INT(x), FIX2INT(y), hash2col(col));
   
   return Qnil;
}

VALUE Painter_drawFillCircle(VALUE self, VALUE x, VALUE y, VALUE r, VALUE col) {
   Painter &ref = getRef<Painter>(self);
   
   ref.drawFillCircle(FIX2INT(x), FIX2INT(y), FIX2INT(r), hash2col(col));
   return Qnil;
}

VALUE Painter_drawCircle(VALUE self, VALUE x, VALUE y, VALUE r, VALUE col) {
   Painter &ref = getRef<Painter>(self);
   
   ref.drawCircle(FIX2INT(x), FIX2INT(y), FIX2INT(r), hash2col(col));
   return Qnil;
}

VALUE Painter_drawTriangle(int argc, VALUE *argv, VALUE self) {
   Painter &ref = getRef<Painter>(self);
   
   VALUE x1, y1, x2, y2, x3, y3, col1, col2, col3;
   rb_scan_args(argc, argv, "72", &x1, &y1, &x2, &y2, &x3, &y3, 
		&col1, &col2, &col3);
   
   if (NIL_P(col2))
      col2 = col1;
   if (NIL_P(col3))
      col3 = col2;

   ref.drawTriangle(FIX2INT(x1), FIX2INT(y1),
		    FIX2INT(x2), FIX2INT(y2),
		    FIX2INT(x3), FIX2INT(y3),
		    hash2col(col1), hash2col(col2), hash2col(col3));

   return Qnil;
}

VALUE Painter_drawBuffer(VALUE self, VALUE x, VALUE y, VALUE buffer) {
   Painter &ref = getRef<Painter>(self);
   
   ref.drawBuffer(FIX2INT(x), FIX2INT(y), getRef<Buffer>(buffer));
   return Qnil;
}


VALUE Painter_clear(VALUE self, VALUE col) {
   Painter &ref = getRef<Painter>(self);
   
   ref.clear(hash2col(col));
   return Qnil;
}

VALUE Joyau_draw(int argc, VALUE *argv, VALUE self) {
   VALUE hash, block;
   rb_scan_args(argc, argv, "01&", &hash, &block);

   OSL_IMAGE *oldBuffer = oslGetDrawBuffer();
   Buffer *buffer = NULL;
   VALUE rbPainter = Qnil;
   bool painter = false;
   bool auto_update = true;
   bool ruby_buf = false;

   if (!NIL_P(hash)) {
      if (TYPE(hash) != T_HASH)
	 rb_raise(rb_eTypeError, "Hash expected for Joyau::draw.");
      
      VALUE rbBuffer = rb_hash_aref(hash, ID2SYM(rb_intern("buffer")));
      
      if (rb_obj_is_kind_of(rbBuffer, getClass("Buffer")) == Qfalse) {
	 if (rbBuffer != Qnil)
	    rb_raise(rb_eTypeError, ":buffer should be a Buffer (or nil).");
      }
      else {
	 buffer = getPtr<Buffer>(rbBuffer);
	 ruby_buf = true;
      }

      if (!buffer)
	 buffer = new Buffer(oldBuffer);

      if (rb_hash_aref(hash, ID2SYM(rb_intern("painter"))) == Qtrue) {
	 painter = true;
	 Painter painter(*buffer);
	 rbPainter = createObject(getClass("Painter"), painter);
      }
      
      if (rb_hash_aref(hash, ID2SYM(rb_intern("auto_update"))) == Qfalse)
	 auto_update = false;
   }
   else {
      if (!buffer)
	 buffer = new Buffer(oldBuffer);
   }

   Graphics_startDraw(Qnil);
   if (!NIL_P(block)) {
      buffer->setActual();
      
      if (painter)
	 rb_yield(rbPainter);
      else {
	 VALUE rbBuffer = createObject(getClass("Buffer"), *buffer, true);
	 rb_yield(rbBuffer);
      }

      Graphics_endDraw(Qnil);
      if (auto_update)
	 Graphics_sync(Qnil);
   }
   else
      rb_raise(rb_eArgError, "Block expected.");

   if (!ruby_buf) // We do not delete the buffer if it come from Ruby.
      delete buffer;

   oslSetDrawBuffer(oldBuffer);

   return Qnil;
}

void defineBuffer() {
   VALUE cBuffer = defClass<Buffer>("Buffer");
   
   defConst(cBuffer, "PF_4444", INT2FIX(OSL_PF_4444));
   defConst(cBuffer, "PF_5650", INT2FIX(OSL_PF_5650));
   defConst(cBuffer, "PF_8888", INT2FIX(OSL_PF_8888));

   defMethod(cBuffer, "set_actual", Buffer_setActual, 0);
   defMethod(cBuffer, "draw", Buffer_draw, -1);
   defMethod(cBuffer, "clear", Buffer_clear, 1);
   defMethod(cBuffer, "x", Buffer_getX, 0);
   defMethod(cBuffer, "y", Buffer_getY, 0);
   defMethod(cBuffer, "w", Buffer_getW, 0);
   defMethod(cBuffer, "h", Buffer_getH, 0);
   defMethod(cBuffer, "x=", Buffer_setX, 1);
   defMethod(cBuffer, "y=", Buffer_setY, 1);
   defMethod(cBuffer, "setPos", Buffer_setPos, -1);
   defMethod(cBuffer, "move", Buffer_move, 2);
   defMethod(cBuffer, "resize", Buffer_resize, 2);
   defMethod(cBuffer, "zoom", Buffer_zoom, 1);
   defMethod(cBuffer, "rotate", Buffer_rotate, 1);
   defMethod(cBuffer, "lock", Buffer_lock, -1);
   defMethod(cBuffer, "unlock", Buffer_unlock, 0);
   defMethod(cBuffer, "[]", Buffer_getPixel, 2);
   defMethod(cBuffer, "[]=", Buffer_setPixel, 3);
   defMethod(cBuffer, "to_sprite", Buffer_to_sprite, 0);
   
   defModFunc(cBuffer, "updateScreen", Buffer_updateScreen, 0);
   defModFunc(cBuffer, "screen", Buffer_updateScreen, 0);
   defModFunc(cBuffer, "destroyScreen", Buffer_destroyScreen, 0);

   defAlias(cBuffer, "setPos", "pos=");

   VALUE cPainter = defClass<Painter>("Painter");
   defMethod(cPainter, "drawLine", Painter_drawLine, -1);
   defMethod(cPainter, "drawFillRect", Painter_drawFillRect, -1);
   defMethod(cPainter, "drawRect", Painter_drawRect, -1);
   defMethod(cPainter, "drawPoint", Painter_drawPoint, 3);
   defMethod(cPainter, "drawCircle", Painter_drawCircle, 4);
   defMethod(cPainter, "drawFillCircle", Painter_drawFillCircle, 4);
   defMethod(cPainter, "drawTriangle", Painter_drawTriangle, -1);
   defMethod(cPainter, "drawBuffer", Painter_drawBuffer, 3);
 
   defMethod(cPainter, "clear", Painter_clear, 1);
   
   defModFunc(JOYAU_MOD, "draw", Joyau_draw, -1);
}
