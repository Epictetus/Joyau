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

#ifndef __JOYAU_MAP__
#define __JOYAU_MAP__

#include "StdInclude.hpp"
#include "Sprite.hpp"

class GameMap
{
public:
   GameMap();

   void setPos(int x, int y);
   void move(int x, int y);
   void resize(int w, int h);

   int getX() { return _x; }
   int getY() { return _y; }
   int getW() { return _w; }
   int getH() { return _h; }

   void addTileset(char *name);
   void setTileSize(int w, int h);

   void addElem(int tileset, int tX, int tY, int x, int y);

   bool collide(Drawable *spr);
   bool isOn(int x, int y);

   void clear();

   void draw();
private:

   struct Tile
   {
      int tileset;
      int tileX, tileY;
      int x, y;
   };

   int _x, _y;
   int _w, _h;

   vector<Sprite> tilesets;
   list<Tile> tiles;

   int tileWidth, tileHeight;
protected:
   bool visible(const Tile &t);
};

VALUE GameMap_setPos(VALUE self, VALUE x, VALUE y);
VALUE GameMap_move(VALUE self, VALUE x, VALUE y);
VALUE GameMap_resize(VALUE self, VALUE w, VALUE h);

VALUE GameMap_getX(VALUE self);
VALUE GameMap_getY(VALUE self);
VALUE GameMap_getW(VALUE self);
VALUE GameMap_getH(VALUE self);

VALUE GameMap_addTileset(VALUE self, VALUE name);
VALUE GameMap_setTileSize(VALUE self, VALUE w, VALUE h);

VALUE GameMap_addElem(VALUE self, VALUE tileset, VALUE tX, VALUE tY,
                      VALUE x, VALUE y);

VALUE GameMap_collide(VALUE self, VALUE spr);
VALUE GameMap_isOn(VALUE self, VALUE x, VALUE y);

VALUE GameMap_clear(VALUE self);

VALUE GameMap_draw(VALUE self);

void defineGameMap();

#endif
