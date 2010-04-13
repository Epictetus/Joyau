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

#include "Buffer.hpp"
#include "Drawable.hpp"
#include "Line.hpp"
#include "DrawableRect.hpp"
#include "Circle.hpp"
#include "Triangle.hpp"
#include "RubyDrawable.hpp"
#include "Graphics.hpp"
#include "Sprite.hpp"
#include "Manager.hpp"

Buffer *Buffer::screen = NULL;
Buffer *Buffer::default_buf = NULL;

std::map<OSL_IMAGE*, bool> Buffer::enabledImages = std::map<OSL_IMAGE*, bool>();
std::stack<OSL_IMAGE*> Buffer::vramStack = std::stack<OSL_IMAGE*>();

Buffer::Buffer(int w, int h, int format): shouldDelete(true) {
   setClass("Buffer");
   createFromGeom(w, h, format);
}

void Buffer::createFromGeom(int w, int h, int format) {
   shouldDelete = true;

   if (w > 512 || h > 512)
      throw RubyException(rb_eRuntimeError,
			  "Either width or height is greter than 512.");

   img = oslCreateImage(w, h, OSL_IN_VRAM, format);
   if (!img) {
      img = oslCreateImage(w, h, OSL_IN_RAM, format);
      if (!img)
	 throw RubyException(rb_eRuntimeError, "Buffer could not be created.");
   }
   else {
      Buffer::registerInVram(img);
   }
}

Buffer::Buffer(OSL_IMAGE *arg, bool copy): img(arg) {
   shouldDelete = copy;
  
   if (copy) {
      img = oslCreateImageCopy(arg, OSL_IN_VRAM);
      if (!img) {
	 img = oslCreateImageCopy(arg, OSL_IN_RAM);
	 if (!img)
	    throw RubyException(rb_eRuntimeError, "Buffer could not be copied.");
      }
      else {
	 Buffer::registerInVram(img);
      }
   }
   else
      img = arg;

   setClass("Buffer"); 
}

Buffer::Buffer(const Buffer &obj): shouldDelete(true) {
   setClass("Buffer");
   createFromBuffer(obj);
}

void Buffer::createFromBuffer(const Buffer &obj) {
   shouldDelete = true;

   img = oslCreateImageCopy(obj.img, OSL_IN_VRAM);
   if (!img) {
      img = oslCreateImageCopy(obj.img, OSL_IN_RAM);
      if (!img)
	 throw RubyException(rb_eRuntimeError, "Buffer could not be copied.");
   }
}

Buffer::Buffer(Drawable &obj): shouldDelete(true) {
   setClass("Buffer");
   createFromDrawable(obj);
}

void Buffer::createFromDrawable(Drawable &obj) {
   shouldDelete = true;

   if (obj.getW() > 512 || obj.getH() > 512)
      throw RubyException(rb_eRuntimeError, "Drawable too big for a buffer.");

   int width = obj.getW();
   int height = obj.getH();
   img = oslCreateImage(width, height, OSL_IN_VRAM, OSL_PF_8888);
   if (!img) {
      img = oslCreateImage(width, height, OSL_IN_RAM, OSL_PF_8888);
      if (!img)
	 throw RubyException(rb_eRuntimeError, "Buffer could not be created");
   }
   else {
      Buffer::registerInVram(img);
   }

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
   if (!img) {
      img = oslCreateImageCopy(obj.getImage(), OSL_IN_RAM);
      if (!img)
	 throw RubyException(rb_eRuntimeError, "Buffer could not be created");
   }
   else {
      Buffer::registerInVram(img);
   }
}

Buffer::~Buffer() {
   if (img && shouldDelete) {
      if (img->location == OSL_IN_RAM)
	 oslDeleteImage(img);
      else
	 Buffer::removeFromVram(img);
   }
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
   if (!shouldDelete)
      throw RubyException(rb_eRuntimeError, "Not allowed to resize this Buffer");

   OSL_IMAGE *old_image = img;
   OSL_IMAGE *old_buffer = oslGetDrawBuffer();
   
   img = oslCreateImage(w, h, OSL_IN_VRAM, img->pixelFormat);
   if (!img) {
      img = oslCreateImage(w, h, OSL_IN_RAM, img->pixelFormat);
      if (!img)
	 throw RubyException(rb_eRuntimeError, "Could not recreate the buffer.");
   }
   else {
      Buffer::registerInVram(img);
   }

   setActual();
   oslDrawImageXY(old_image, 0, 0);
   oslSetDrawBuffer(old_buffer);

   if (old_image->location == OSL_IN_RAM)
      oslDeleteImage(old_image);
   else
      Buffer::removeFromVram(old_image);
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

void Buffer::save(const std::string &filename) {
   oslWriteImageFilePNG(img, filename.c_str(), 0);
}

bool Buffer::isScreen() const {
   return img == OSL_DEFAULT_BUFFER;
}

void Painter::drawLine(int x1, int y1, int x2, int y2,
                       OSL_COLOR col, OSL_COLOR col2) {
   Line line;
   line.setPos(x1, y1);
   line.setPoint(x2, y2);
   OSL_COLOR grad[2] = { col, col2 };
   line.setGradient(grad);

   buf->draw(line);
}

void Painter::drawFillRect(int x1, int y1, int x2, int y2,
			   OSL_COLOR col, OSL_COLOR col2,
			   OSL_COLOR col3, OSL_COLOR col4) {
   DrawableRect rect;
   rect.setPos(x1, y1);
   rect.setCorner(x2, y2);
   OSL_COLOR grad[4] = { col, col2, col3, col4 };
   rect.setGradient(grad);

   buf->draw(rect);
}

void Painter::drawRect(int x1, int y1, int x2, int y2, OSL_COLOR col) {
   DrawableRect rect;
   rect.setFilled(false);
   rect.setPos(x1, y1);
   rect.setCorner(x2, y2);
   rect.setColor(col);

   buf->draw(rect);
}

void Painter::drawPoint(int x, int y, OSL_COLOR col) {
   DrawableRect rect;
   rect.setPos(x, y);
   rect.resize(1, 1);
   rect.setColor(col);

   buf->draw(rect);
}

void Painter::drawFillCircle(int x, int y, int r, OSL_COLOR col) {
   Circle circle;
   circle.setCenter(x, y);
   circle.setRadius(r);
   circle.setColor(col);

   buf->draw(circle);
}

void Painter::drawCircle(int x, int y, int r, OSL_COLOR col) {
   Circle circle;
   circle.setFilled(false);
   circle.setCenter(x, y);
   circle.setRadius(r);
   circle.setColor(col);

   buf->draw(circle);
}

void Painter::drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3,
                           OSL_COLOR col, OSL_COLOR col2, OSL_COLOR col3) {
   Triangle triangle;
   triangle.setPoints(x1, y1, x2, y2, x3, y3);
   OSL_COLOR grad[3] = { col, col2, col3 };
   triangle.setGradient(grad);

   buf->draw(triangle);
}

void Painter::drawBuffer(int x, int y, Buffer &obj) {
   int oldX = obj.getX();
   int oldY = obj.getY();

   obj.setPos(x, y);
   buf->draw(obj);

   obj.setPos(oldX, oldY);
}

void Painter::clear(OSL_COLOR col) {
   buf->clear(col);
}

/*
  Document-class: Joyau::Buffer

  Buffers can be displayed on the screen. The screen itself is a Buffer, which
  can be accessed.

  You can modify a buffer by multiple ways, like drawing drawables on it,
  or modifying directly its pixels (really fun effects can be created this
  way!).

  For the first method, consider using +Joyau::draw+ which makes your job
  much more easier.

  A buffer's width cannot be greater than 512. Same for the width. Also,
  multiple pixel formats are usable. PF_8888 looks better, but takes much
  more memory than PF_4444, so be carefull.
*/

/*
  call-seq: new(drawable)
            new(buffer)
            new(w, h)
            new(w, h, pixel_format)

  A buffer can be created from a drawable or from another buffer. It'll
  simply contain the Drawable (after the creation, the two objects aren't
  related at all).

  Another way is to specify the buffer's dimension, and at your option, the
  pixel format (by default, 16 bits colors are used).
*/
VALUE Buffer_initialize(int argc, VALUE *argv, VALUE self)
{
   VALUE arg1, arg2, arg3;
   rb_scan_args(argc, argv, "12", &arg1, &arg2, &arg3);

   Buffer &ref = getRef<Buffer>(self);

   try {
      if (NIL_P(arg2)) {
	 if (rb_obj_is_kind_of(arg1, getClass("Drawable")) == Qtrue) {
	    RubyDrawable drawable(arg1);
	    ref.createFromDrawable(drawable);
	 }
	 else if (rb_obj_is_kind_of(arg1, getClass("Buffer")) == Qtrue) {
	    ref.createFromBuffer(getRef<Buffer>(arg1));
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
      
	 ref.createFromGeom(FIX2INT(arg1), FIX2INT(arg2), FIX2INT(arg3));
      }
   }
   catch (const RubyException &e) {
      e.rbRaise();
   }
   
   return Qnil;
}

/*
  Changes the actual drawbuffer (+Joyau::draw+ can do this automatically).
*/
VALUE Buffer_setActual(VALUE self) {
   Buffer &ref = getRef<Buffer>(self);
   ref.setActual();

   return Qnil;
}

/*
  call-seq: draw
            draw(object)

  If no arguments are given, the buffer is drawn. If either a buffer or
  a drawable is given, then it is drawn on the buffer.
*/
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

/*
  call-seq: clear(color)

  Clears the buffer in a given color.
 */
VALUE Buffer_clear(VALUE self, VALUE color) {
   Buffer &ref = getRef<Buffer>(self);
   ref.clear(hash2col(color));

   return Qnil;
}

/*
  Returns the buffer's abscissa.
*/
VALUE Buffer_getX(VALUE self) {
   Buffer &ref = getRef<Buffer>(self);
   return INT2FIX(ref.getX());
}

/*
  Returns the buffer's ordinate.
*/
VALUE Buffer_getY(VALUE self) {
   Buffer &ref = getRef<Buffer>(self);
   return INT2FIX(ref.getY());
}

/*
  Returns the buffer's width.
*/
VALUE Buffer_getW(VALUE self) {
   Buffer &ref = getRef<Buffer>(self);
   return INT2FIX(ref.getW());
}

/*
  Returns the buffer's width.
*/
VALUE Buffer_getH(VALUE self) {
   Buffer &ref = getRef<Buffer>(self);
   return INT2FIX(ref.getH());
}

/*
  call-seq: x=(val)

  Sets the buffer's abscissa.
*/
VALUE Buffer_setX(VALUE self, VALUE x) {
   Buffer &ref = getRef<Buffer>(self);
   ref.setX(FIX2INT(x));

   return x;
}

/*
  call-seq: y=(val)

  Sets the buffer's ordinate.
*/
VALUE Buffer_setY(VALUE self, VALUE y) {
   Buffer &ref = getRef<Buffer>(self);
   ref.setY(FIX2INT(y));

   return y;
}

/*
  call-seq: setPos(x, y)
            setPos(point)

  Sets the buffer's position.
*/
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

/*
  call-seq: move(x, y)

  Moves the buffer.
*/
VALUE Buffer_move(VALUE self, VALUE x, VALUE y) {
   Buffer &ref = getRef<Buffer>(self);
   ref.move(FIX2INT(x), FIX2INT(y));

   return Qnil;
}

/*
  call-seq: resize(w, h)

  Resizes the buffer. Be carefull, this is rather slow: a buffer is
  created, and the old one is drawn on it before being destroyed.
*/
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

/*
  call-seq: zoom(level)

  Zooms on the buffer.
*/
VALUE Buffer_zoom(VALUE self, VALUE level) {
   Buffer &ref = getRef<Buffer>(self);
   ref.zoom(FIX2INT(level));

   return Qnil;
}

/*
  call-seq: rotate(angle)

  Rotates the buffer.
*/
VALUE Buffer_rotate(VALUE self, VALUE angle) {
   Buffer &ref = getRef<Buffer>(self);
   ref.rotate(FIX2INT(angle));

   return Qnil;
}

/*
  call-seq: lock
            lock { ... } 

  Locks the buffer. Once the buffer is locked, you can modify its pixels.
  If a block is given, then it is executed, and the buffer is unlocked after
  having executed the block.

  Examples:
    buf.lock
    buf[0, 0] = 0
    buf.unlock

    buf.lock do
      buf[0, 0] = 0
    end
*/
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

/*
  call-seq: unlock

  Unlocks the buffer. Call it when you've finished to modify your image.
*/
VALUE Buffer_unlock(VALUE self) {
   Buffer &ref = getRef<Buffer>(self);
   ref.unlock();
   
   return Qnil;
}

/*
  call-seq: [x, y]

  Returns the color of a pixel.
*/
VALUE Buffer_getPixel(VALUE self, VALUE x, VALUE y) {
   Buffer &ref = getRef<Buffer>(self);
   return col2hash(ref.getPixel(FIX2INT(x), FIX2INT(y)));
}

/*
  call-seq: [x, y] = col
    
  Sets the color of a pixel.
*/
VALUE Buffer_setPixel(VALUE self, VALUE x, VALUE y, VALUE col) {
   Buffer &ref = getRef<Buffer>(self);
   ref.setPixel(FIX2INT(x), FIX2INT(y), hash2col(col));

   return col;
}

/*
  call-seq: save(filename)

  Saves the buffer 
*/
VALUE Buffer_save(VALUE self, VALUE filename) {
   filename = rb_obj_as_string(filename);

   Buffer &ref = getRef<Buffer>(self);
   ref.save(StringValuePtr(filename));

   return Qnil;
}

/*
  Converts the buffer in a Sprite.
*/
VALUE Buffer_to_sprite(VALUE self) {
   Buffer &ref = getRef<Buffer>(self);
   try {
      return Data_Wrap_Struct(getClass("Sprite"), 0, wrapped_free<Sprite>, 
			      new Sprite(ref));
   }
   catch (const RubyException &e) {
      e.rbRaise();
      return Qnil;
   }
}

/*
  Returns the buffer representing the screen.
  Use it only for reading.
*/
VALUE Buffer_getScreen(VALUE self) {
   Buffer *buf = Buffer::getScreen();
   if (buf)
      return buf->toRuby();
   return Qnil;
}

/*
  Returns the default buffer.
*/
VALUE Buffer_getDefault(VALUE self) {
   Buffer *buf = Buffer::getDefault();
   if (buf)
      return buf->toRuby();
   return Qnil;
}

/*
  Returns the actual buffer.
*/
VALUE Buffer_getActual(VALUE self) {
   return Data_Wrap_Struct(getClass("Buffer"), 0, wrapped_free<Buffer>, 
			   new Buffer(oslGetDrawBuffer()));
}

/*
  call-seq: [filename, copy = true]

  Returns a Buffer loaded from a file. If you want to return the loaded
  ressource, and not its copy, set copy to false.
*/
VALUE Buffer_find(int argc, VALUE *argv, VALUE self) {
   VALUE filename, copy;
   rb_scan_args(argc, argv, "11", &filename, &copy);
   filename = rb_obj_as_string(filename);

   if (NIL_P(copy))
      copy = Qtrue;

   OSL_IMAGE *buf = Manager::getInstance().getPic(StringValuePtr(filename));
   return Data_Wrap_Struct(getClass("Buffer"), 0, wrapped_free<Buffer>,
			   new Buffer(buf, copy == Qtrue));
}

/*
  Document-class: Joyau::Painter
    
  This class allows to write easily in a buffer, at anytime.
  You can specify the buffer it is linked with at construction time,
  or get a painter through +Joyau::draw+.
*/

/*
  call-seq: new(buffer)

  Creates a new Painter linked to a buffer.
*/
VALUE Painter_initialize(VALUE self, VALUE buffer)
{
   Painter &ref = getRef<Painter>(self);

   if (!rb_obj_is_kind_of(buffer, getClass("Buffer")))
      rb_raise(rb_eTypeError, "Can't convert %s into Joyau::Buffer",
	       rb_obj_classname(buffer));

   ref.setBuffer(getPtr<Buffer>(buffer));
   return Qnil;
}

/*
  call-seq: drawLine(x1, y1, x2, y2, col1, col2 = nil)

  Draws a line on the buffer.
*/
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

/*
  call-seq: drawLine(x1, y1, x2, y2, col1, col2 = nil, col3 = nil, col4 = nil)

  Draws a filled rect on the buffer.
*/
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

/*
  call-seq: drawRect(x1, y1, x2, y2, col1, col2 = nil, col3 = nil, col4 = nil)

  Draws a rect on the buffer.
*/
VALUE Painter_drawRect(int argc, VALUE *argv, VALUE self) {
   Painter &ref = getRef<Painter>(self);
   
   VALUE x1, y1, x2, y2, col;
   rb_scan_args(argc, argv, "5", &x1, &y1, &x2, &y2, &col);

   ref.drawRect(FIX2INT(x1), FIX2INT(y1), FIX2INT(x2), FIX2INT(y2),
		hash2col(col));
   return Qnil;
}

/*
  call-seq: drawPoint(x, y, col)

  Draws a point on the buffer.
*/
VALUE Painter_drawPoint(VALUE self, VALUE x, VALUE y, VALUE col) {
   Painter &ref = getRef<Painter>(self);
   ref.drawPoint(FIX2INT(x), FIX2INT(y), hash2col(col));
   
   return Qnil;
}

/*
  call-seq: drawFillCircle(x, y, radius, col)

  Draws a filled circle on the buffer.
*/
VALUE Painter_drawFillCircle(VALUE self, VALUE x, VALUE y, VALUE r, VALUE col) {
   Painter &ref = getRef<Painter>(self);
   
   ref.drawFillCircle(FIX2INT(x), FIX2INT(y), FIX2INT(r), hash2col(col));
   return Qnil;
}

/*
  call-seq: drawCircle(x, y, radius, col)

  Draws a circle on the buffer.
*/
VALUE Painter_drawCircle(VALUE self, VALUE x, VALUE y, VALUE r, VALUE col) {
   Painter &ref = getRef<Painter>(self);
   
   ref.drawCircle(FIX2INT(x), FIX2INT(y), FIX2INT(r), hash2col(col));
   return Qnil;
}

/*
  call-seq: drawTriangle(x1, y1, x2, y2, x3, y3, col1, col2 = nil, col3 = nil)

  Draws a tiangle on the buffer.
*/
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

/*
  call-seq: drawBuffer(x, y, buffer)

  Draws a buffer on a buffer.
*/
VALUE Painter_drawBuffer(VALUE self, VALUE x, VALUE y, VALUE buffer) {
   Painter &ref = getRef<Painter>(self);
   
   ref.drawBuffer(FIX2INT(x), FIX2INT(y), getRef<Buffer>(buffer));
   return Qnil;
}

/*
  call-seq: clear(color)

  Clears the buffer in a color.
*/
VALUE Painter_clear(VALUE self, VALUE col) {
   Painter &ref = getRef<Painter>(self);
   
   ref.clear(hash2col(col));
   return Qnil;
}

/*
  call-seq: draw(hash = nil) { ... }

  Three keys are checked in the given hash : ":buffer" which is the buffer
  on which manipulation are done (by default, the actual buffer is taken),
  ":painter", which tell us to yield a +Joyau::Painter+ instead of a
  +Joyau::Buffer+ when true (false by default), and ":auto_update" which
  tell us whether we should update the buffer (true by default).

  It is mandatory to give a block to this function.

  Examples:
    Joyau.draw(:buffer => a_buffer, :painter => true)
    Joyau.draw(:auto_update => false)
    Joyau.draw(:painter => true)
    Joyau.draw(:buffer => a_buffer)
*/
VALUE Joyau_draw(int argc, VALUE *argv, VALUE self) {
   static bool can_draw = false;

   VALUE hash, block;
   rb_scan_args(argc, argv, "01&", &hash, &block);

   OSL_IMAGE *oldBuffer = oslGetDrawBuffer();
   Buffer *buffer = NULL;
   VALUE rbPainter = Qnil;
   bool painter = false;
   bool auto_update = true;
   bool ruby_buf = false;

   bool could_draw = can_draw;

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
	 Painter painter(buffer);
	 rbPainter = createObject(getClass("Painter"), painter);
      }
      
      if (rb_hash_aref(hash, ID2SYM(rb_intern("auto_update"))) == Qfalse)
	 auto_update = false;
   }
   else {
      if (!buffer)
	 buffer = new Buffer(oldBuffer);
   }

   if (buffer->isScreen() && !can_draw) {
      can_draw = true;
      Graphics_startDraw(Qnil);
   }
   if (!NIL_P(block)) {
      buffer->setActual();
      
      if (painter)
	 rb_yield(rbPainter);
      else {
	 VALUE rbBuffer = createObject(getClass("Buffer"), *buffer, true);
	 rb_yield(rbBuffer);
      }

      if (buffer->isScreen() && !could_draw) {
	 can_draw = false;
	 Graphics_endDraw(Qnil);
      }
      if (auto_update && buffer->isScreen())
	 Graphics_sync(Qnil);
   }
   else
      rb_raise(rb_eArgError, "Block expected.");

   if (!ruby_buf) // We do not delete the buffer if it comes from Ruby.
      delete buffer;

   oslSetDrawBuffer(oldBuffer);

   return Qnil;
}

void defineBuffer() {
   VALUE cBuffer = defClass<Buffer>("Buffer");
   
   /* 
      Document-const: PF_4444      
      PF_4444: Pixel format for 8 bits colors.
   */
   
   /*
      Document-const: PF_5650   
      PF_5650: Pixel format for 16 bits colors.
   */
   
   /*
      Document-const: PF_8888   
      PF_8888: Pixel format for 32 bits colors.
   */

   defConst(cBuffer, "PF_4444", INT2FIX(OSL_PF_4444));
   defConst(cBuffer, "PF_5650", INT2FIX(OSL_PF_5650));
   defConst(cBuffer, "PF_8888", INT2FIX(OSL_PF_8888));
   
   defMethod(cBuffer, "initialize", Buffer_initialize, -1);

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
   defMethod(cBuffer, "save", Buffer_save, 1);
   defMethod(cBuffer, "to_sprite", Buffer_to_sprite, 0);
   
   defClassMethod(cBuffer, "screen", Buffer_getScreen, 0);
   defClassMethod(cBuffer, "default", Buffer_getDefault, 0);
   defClassMethod(cBuffer, "actual", Buffer_getActual, 0);

   defClassMethod(cBuffer, "[]", Buffer_find, -1);
   
   defAlias(cBuffer, "setPos", "pos=");

   VALUE cPainter = defClass<Painter>("Painter");
   defMethod(cPainter, "initialize", Painter_initialize, 1);

   defMethod(cPainter, "drawLine", Painter_drawLine, -1);
   defMethod(cPainter, "drawFillRect", Painter_drawFillRect, -1);
   defMethod(cPainter, "drawRect", Painter_drawRect, -1);
   defMethod(cPainter, "drawPoint", Painter_drawPoint, 3);
   defMethod(cPainter, "drawCircle", Painter_drawCircle, 4);
   defMethod(cPainter, "drawFillCircle", Painter_drawFillCircle, 4);
   defMethod(cPainter, "drawTriangle", Painter_drawTriangle, -1);
   defMethod(cPainter, "drawBuffer", Painter_drawBuffer, 3);
 
   defMethod(cPainter, "clear", Painter_clear, 1);
   
   VALUE joyau = JOYAU_MOD;
   defModFunc(joyau, "draw", Joyau_draw, -1);
}
