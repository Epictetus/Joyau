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

#ifndef JOYAU_LINE
#define JOYAU_LINE

#include "Shape.hpp"

/** @addtogroup Drawables **/
/*@{*/

/** 
 * @class Line
 * A sprite whose moves are done according to the analogic stick.
 */
class Line: public Shape
{
public:
   Line();

   /** Sets the line's second point.
    *  @param x second point's x position
    *  @param y second point's y position
    */
   void setPoint(int x, int y);
   
   /** Sets the line's second point.
    *  @param p second point.
    */
   void setPoint(const Point &p);
   
   /** Returns the line's second point **/
   Point getPoint() const { return Point(_x2, _y2); }

   bool collide(Drawable &item);
   bool isOn(int x, int y);

   void draw();
private:
   int _x2, _y2;
};

/*@}*/

VALUE Line_setPoint(VALUE self, VALUE x, VALUE y);
VALUE Line_setPointPoint(VALUE self, VALUE p);
VALUE Line_getPoint(VALUE self);

void defineLine();

#endif
