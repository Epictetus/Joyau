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

#ifndef JOYAU_RECT
#define JOYAU_RECT

#include "Shape.hpp"

/** @addtogroup Drawables **/
/*@{*/

/**
 * @class DrawableRect
 * A class which represents a rect, which can be drawed.
 * (unlike Rect)
 */
class DrawableRect: public FillableShape
{
public:
   DrawableRect();

   /** Resizes the DrawableRect.
    *  @param w the rect's new width
    *  @param h the rect's new height
    */
   void resize(int w, int h);

    /** Changes the DrawableRect's width.
     *  @param w the rect's new width
     */
   void setW(int w);

   /** Changes the DrawableRect's height.
    *  @param h the rect's new height
    */
   void setH(int h);

   /** Changes the corner's position
    *  @param x corner's x position
    *  @param y corner's y position
    */
   void setCorner(int x, int y);

   /** Returns the corner position as a Point **/
   Point getCorner() const { return Point(_x2, _y2); }

   void draw();
private:
   int _x2, _y2;
};

/*@}*/

VALUE DrawableRect_resize(VALUE self, VALUE w, VALUE h);
VALUE DrawableRect_setW(VALUE self, VALUE w);
VALUE DrawableRect_setH(VALUE self, VALUE h);

VALUE DrawableRect_setCorner(VALUE self, VALUE x, VALUE y);
VALUE DrawableRect_cornerOp(VALUE self, VALUE p);
VALUE DrawableRect_getCorner(VALUE self);

void defineDrawableRect();

#endif
