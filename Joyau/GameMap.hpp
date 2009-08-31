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

struct CollisionType
{
   CollisionType():
      left(false), right(false), up(false), down(false), content(true)
   {}

   bool left, right, up, down, content;
};

class GameMap: public Drawable
{
public:
   struct Tile
   {
      int tileset;
      int tileX, tileY;
      int x, y;

      CollisionType type;
   };

   struct shouldRemove 
   {
      bool operator()(const Tile &t);
   };

   GameMap();

   void addTileset(char *name);
   void setTileSize(int w, int h);

   int getTileH() const { return tileHeight; }
   int getTileW() const { return tileWidth; }

   void setCollisionH(int val) { colH = val; }
   int getCollisionH() const { return colH; }

   Point absToRel(int x, int y) const;
   Point relToAbs(int x, int y) const;
   void centerOn(int x, int y);

   void addElem(int tileset, int tX, int tY, int x, int y);
   void addElem(const Tile &tile);

   bool collide(Drawable &spr);
   bool isOn(int x, int y);

   void clear();
   void clearTiles() { tiles.clear(); }

   void draw();

   std::list<Tile> &getTiles() { return tiles; }
   std::vector<Sprite> &getTilesets() { return tilesets; }
private:
   std::vector<Sprite> tilesets;
   std::list<Tile> tiles;

   int tileWidth, tileHeight;
   int colH;
protected:
   bool visible(const Tile &t) const;
};

VALUE CollisionType_right(VALUE self);
VALUE CollisionType_left(VALUE self);
VALUE CollisionType_up(VALUE self);
VALUE CollisionType_down(VALUE self);
VALUE CollisionType_content(VALUE self);

VALUE CollisionType_setRight(VALUE self, VALUE val);
VALUE CollisionType_setLeft(VALUE self, VALUE val);
VALUE CollisionType_setUp(VALUE self, VALUE val);
VALUE CollisionType_setDown(VALUE self, VALUE val);
VALUE CollisionType_setContent(VALUE self, VALUE val);

VALUE Tile_tileX(VALUE self);
VALUE Tile_tileY(VALUE self);
VALUE Tile_x(VALUE self);
VALUE Tile_y(VALUE self);
VALUE Tile_tileset(VALUE self);

VALUE Tile_setTileX(VALUE self, VALUE val);
VALUE Tile_setTileY(VALUE self, VALUE val);
VALUE Tile_setX(VALUE self, VALUE val);
VALUE Tile_setY(VALUE self, VALUE val);
VALUE Tile_setTileset(VALUE self, VALUE val);

VALUE GameMap_addTileset(VALUE self, VALUE name);
VALUE GameMap_setTileSize(VALUE self, VALUE w, VALUE h);

VALUE GameMap_tileWidth(VALUE self);
VALUE GameMap_tileHeight(VALUE self);

VALUE GameMap_setCollisionH(VALUE self, VALUE val);
VALUE GameMap_collisionH(VALUE self);

VALUE GameMap_centerOn(VALUE self, VALUE x, VALUE y);

VALUE GameMap_addElem(int argc, VALUE *argv, VALUE self);
VALUE GameMap_push(VALUE self, VALUE tile);

VALUE GameMap_clear(VALUE self);
VALUE GameMap_clearTiles(VALUE self);

VALUE GameMap_tiles(VALUE self);
VALUE GameMap_tilesets(VALUE self);

VALUE GameMap_each_tile(VALUE self);
VALUE GameMap_each_tileset(VALUE self);
VALUE GameMap_reject_tiles(VALUE self);

void defineGameMap();

#endif
