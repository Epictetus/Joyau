#include "Drawable.hpp"

Drawable::Drawable()
{
   _x = 0;
   _y = 0;
   _w = 0;
   _h = 0;
}

Rect Drawable::boundingRect() 
{ 
   return Rect(getX(), getY(), getW(), getH()); 
}

bool Drawable::collide(Drawable &item)
{
   Rect rect1 = item.boundingRect();
   Rect rect2 = boundingRect();

   Point points1[4];
   points1[0] = rect1.top_right();
   points1[1] = rect1.top_left();
   points1[2] = rect1.bottom_left();
   points1[3] = rect1.bottom_right();

   Point points2[4];
   points2[0] = rect2.top_right();
   points2[1] = rect2.top_left();
   points2[2] = rect2.bottom_left();
   points2[3] = rect2.bottom_right();

   for (int i = 0; i < 4; ++i)
   {
      /*
	If only on corner of this is on item, or if only one of the
	item corner is on this, we return true because it collides.
	Otherwisely, we return true since it don't collide.
       */
      if (isOn(points1[i]))
	 return true;
      if (item.isOn(points2[i]))
	 return true;
   }
   return false;
}

bool Drawable::isOn(int x, int y)
{
   Rect rect = boundingRect();
   int x1 = rect.x;
   int x2 = rect.x + rect.w;
   int y1 = rect.y;
   int y2 = rect.y + rect.h;

   if (x > x1 &&
       x < x2 &&
       y > y1 &&
       y < y2)
      return true;
   return false;
}

bool Drawable::isOn(Point p)
{
   return isOn(p.x, p.y);
}

int Drawable::getX()
{
   return _x;
}

int Drawable::getY()
{
   return _y;
}

int Drawable::getW()
{
   return _w;
}

int Drawable::getH()
{
   return _h;
}

void Drawable::setPos(int x, int y)
{
   _x = x;
   _y = y;
}

void Drawable::setPos(Point p)
{
   setPos(p.x, p.y);
}

void Drawable::move(int x, int y)
{
   _x += x;
   _y += y;
}
