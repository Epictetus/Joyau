#ifndef __GRAPHICS__
#define __GRAPHCIS__

#include "StdInclude.hpp"

VALUE Graphics_init(VALUE self);
VALUE Graphics_stop(VALUE self);

VALUE lib_start(VALUE self);
VALUE lib_stop(VALUE self);

VALUE Graphics_mayPlay(VALUE self);
VALUE Graphics_startDraw(VALUE self);
VALUE Graphics_endDraw(VALUE self);
VALUE Graphics_sync(VALUE self);

VALUE Graphics_drawLine(VALUE self, VALUE x1, VALUE y1, VALUE x2, 
			VALUE y2, VALUE color);
VALUE Graphics_drawRect(VALUE self, VALUE x1, VALUE y1, VALUE x2, 
			VALUE y2, VALUE color);
VALUE Graphics_drawFillRect(VALUE self, VALUE x1, VALUE y1, VALUE x2, 
			    VALUE y2, VALUE color);

VALUE Graphics_clear(VALUE self);

VALUE setTextBackground(VALUE self, VALUE color);
VALUE setTextColor(VALUE self, VALUE color);
VALUE drawText(VALUE self, VALUE x, VALUE y, VALUE text);

void defineGraphics();

#endif
