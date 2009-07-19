#ifndef __JOYAU_DRAWABLE__
#define __JOYAU_DRAWABLE__

struct Point
{
   Point() {}

   Point(int X, int Y):
      x(X), y(Y)
   {}

   int x, y;
};

struct Rect
{
   Rect(int X, int Y, int W, int H):
      x(X), y(Y), w(W), h(H)
   {}

   int x, y;
   int w, h;

   Point top_right() { return Point(x + w, y); }
   Point top_left() { return Point(x, y); }
   Point bottom_right() { return Point(x + w, y + h); }
   Point bottom_left() { return Point(x, y + h); }
};

class Drawable
{
public:
   Drawable();

   virtual Rect boundingRect();

   virtual bool collide(Drawable &item);
   virtual bool isOn(int x, int y);
   virtual bool isOn(Point p);

   int getX();
   int getY();

   virtual int getW();
   virtual int getH();

   void setPos(int x, int y);
   void setPos(Point p);

   void move(int x, int y);
private:
   int _x, _y;
protected:
   int _w, _h;
};

#endif
