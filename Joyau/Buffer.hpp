#include "StdInclude.hpp"

class Drawable;
class Point;

class Buffer: public RubyObject {
public:
   Buffer(int w, int h, int format);
   Buffer(const Buffer &obj);
   Buffer(Drawable &obj);
   Buffer(OSL_IMAGE *img);
   ~Buffer();

   void setActual();

   void draw();
   void draw(Drawable &obj);
   void draw(Buffer &obj);

   void clear(OSL_COLOR color);

   int getX() const;
   int getY() const;

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

   static void updateScreen() {
      if (screen)
         delete screen;
      screen = new Buffer(OSL_DEFAULT_BUFFER);
   }

   static Buffer *getScreen() {
      return screen;
   }

   static void destroyScreen() {
      delete screen;
      screen = NULL;
   }
private:
   OSL_IMAGE *img;
   bool shouldDelete;

   static Buffer *screen;
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

VALUE Buffer_updateScreen(VALUE self);
VALUE Buffer_getScreen(VALUE self);
VALUE Buffer_destroyScreen(VALUE self);

VALUE Painter_drawLine(int argc, VALUE *argv, VALUE self);
VALUE Painter_drawFillRect(int argc, VALUE *argv, VALUE self);
VALUE Painter_drawRect(int argc, VALUE *argv, VALUE self);
VALUE Painter_drawPoint(VALUE self, VALUE x, VALUE y, VALUE col);
VALUE Painter_drawFillCircle(VALUE self, VALUE x, VALUE y, VALUE r, VALUE col);
VALUE Painter_drawCircle(VALUE self, VALUE x, VALUE y, VALUE r, VALUE col);
VALUE Painter_drawTriangle(int argc, VALUE *argv, VALUE self);
VALUE Painter_drawBuffer(int argc, VALUE *argv, VALUE self);

VALUE Painter_clear(VALUE self, VALUE col);

VALUE Joyau_draw(int argc, VALUE *argv, VALUE self);

void defineBuffer();
