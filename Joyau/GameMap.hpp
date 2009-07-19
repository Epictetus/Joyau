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

#ifndef JOYAU_MAP
#define JOYAU_MAP

#include "StdInclude.hpp"
#include "Sprite.hpp"

class GameMap: public Drawable
{
public:
   GameMap();

   void resize(int w, int h);

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

   vector<Sprite> tilesets;
   list<Tile> tiles;

   int tileWidth, tileHeight;
protected:
   bool visible(const Tile &t);
};

VALUE GameMap_resize(VALUE self, VALUE w, VALUE h);

VALUE GameMap_addTileset(VALUE self, VALUE name);
VALUE GameMap_setTileSize(VALUE self, VALUE w, VALUE h);

VALUE GameMap_addElem(VALUE self, VALUE tileset, VALUE tX, VALUE tY,
                      VALUE x, VALUE y);

VALUE GameMap_clear(VALUE self);

void defineGameMap();

#endif
