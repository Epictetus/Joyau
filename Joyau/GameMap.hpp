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

/** @addtogroup Drawables **/
/*@{*/

/**
 * @class CollisionType
 * A class which tells where to check collision to a GameMap.
 */
struct CollisionType: public RubyObject
{
   CollisionType():
      left(false), right(false), up(false), down(false), content(true)
   { setClass("CollisionType"); }

   /** Creates a new CollisionType
    *  @param aContent True if we have to check collision with everything
    *  @param aLeft True if we have to check collision with the left side.
    *  @param aRight True if we have to check collision with the right side.
    *  @param aUp True if we have to check collision with the upper side.
    *  @param aDown True if we have to check collision with the downer side.
    */
   CollisionType(bool aContent, bool aLeft, bool aRight, bool aUp, bool aDown):
      left(aLeft), right(aRight), up(aUp), down(aDown), content(aContent)
   { setClass("CollisionType"); }

   bool left, right, up, down, content;
};

/**
 * @class GameMap
 * A class which draws a list of tiles, using some tilesets.
 */
class GameMap: public Drawable
{
public:
   /**
    * @class Tile
    * Represents an element in a GameMap
    */
   struct Tile: public RubyObject
   {
      Tile() { setClass("Tile"); }

      /** used tileset's id **/
      int tileset;

      /** x position on the tileset **/
      int tileX;

      /** y position on the tileset **/
      int tileY;
      
      /** x position relatively to the map **/
      int x;

      /** y position relatively to the map **/
      int y;

      /** tells where to check collision for that tile **/
      CollisionType type;
   };

   /** Used in order to sort tiles **/
   struct SortTile {
      /** Returns true if obj2.y is greater than obj.y **/
      bool operator()(const Tile &obj, const Tile &obj2) {
	 return obj.y < obj2.y;
      }
   };

   struct SortBetween {
      bool operator()(const Drawable *obj, const Drawable *obj2) {
	 return obj->getY() < obj2->getY();
      }
   };

   typedef RubyReject<Tile> shouldRemove;

   GameMap();

   /** Loads a tileset.
    *  @param name tileset's filename.
    */
   void addTileset(char *name);

   /** Sets the size for all the tilesets in the map.
    *  @param w each tile's width.
    *  @param h each tile's height.
    */
   void setTileSize(int w, int h);

   /** returns each tile's height **/
   int getTileH() const { return tileHeight; }

   /** returns each tile's width **/
   int getTileW() const { return tileWidth; }

   /** Sets collisionH to val. When it's different to -1, collision are only
    *  checked for the the drawable's collisionH lower pixels.
    *  @param val collisionH value
    */
   void setCollisionH(int val) { colH = val; }

   /** returns collisionH **/
   int getCollisionH() const { return colH; }

   /** Returns a Point's relative position.
    *  @param x x (absolute position)
    *  @param y y (absolute poision)
    */
   Point absToRel(int x, int y) const;

   /** Returns a Point's absolute position.
    *  @param x x (relative position)
    *  @param y y (relative poision)
    */
   Point relToAbs(int x, int y) const;

   /** Centers the map on a point.
    *  @param x x position, relative to the map.
    *  @param y y position, relative to the map.
    */
   void centerOn(int x, int y);

   /** Creates a new tileset and insert it in the map.
    *  @param tileset tile's tileset
    *  @param tX tile's x position on the tileset
    *  @param tY tile's y position on the tileset
    *  @param x tile's x position on the map
    *  @param y tile's y position on the map
    */
   void addElem(int tileset, int tX, int tY, int x, int y);

   /** Inserts a tile in the GameMap.
    *  @param tile the tile which should be added.
    */
   void addElem(const Tile &tile);

   bool collide(Drawable &spr);
   bool isOn(int x, int y);

   /** Clears both tilesets and tiles **/
   void clear();

   /** Clears all the tiles **/
   void clearTiles() { tiles.clear(); }

   void draw();

   /** Yields each tile in Ruby **/
   void rbEachTile();

   /** Yields each tileset in Ruby **/
   void rbEachTileset();

   /** removes tiles which match to a condition, via Ruby. **/
   void rbRejectTiles();

   /** Returns a ruby array containing all the tiles. Both
    *  Both read and write access are allowed.
    */
   VALUE rbTiles();

   /** Returns a ruby array containing all the tilesets. Both
    *  Both read and write access are allowed.
    */
   VALUE rbTilesets();

   /** Sets the object drawn between the map's tiles.
    *  It'll be drawn under the tiles which have a lower ordinate.
    */
   void addBetween(Drawable *obj);
private:
   std::vector<Sprite> tilesets;
   std::vector<Tile> tiles;

   int tileWidth, tileHeight;
   int colH;

   std::vector<Drawable*> between;
protected:
   /** returns whether a tile is visible **/
   bool visible(const Tile &t) const;
};

/*@}*/

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
VALUE Tile_type(VALUE self);

VALUE Tile_setTileX(VALUE self, VALUE val);
VALUE Tile_setTileY(VALUE self, VALUE val);
VALUE Tile_setX(VALUE self, VALUE val);
VALUE Tile_setY(VALUE self, VALUE val);
VALUE Tile_setTileset(VALUE self, VALUE val);
VALUE Tile_setType(VALUE self, VALUE val);

VALUE GameMap_addTileset(VALUE self, VALUE name);
VALUE GameMap_setTileSize(VALUE self, VALUE w, VALUE h);

VALUE GameMap_tileWidth(VALUE self);
VALUE GameMap_tileHeight(VALUE self);

VALUE GameMap_setCollisionH(VALUE self, VALUE val);
VALUE GameMap_collisionH(VALUE self);

VALUE GameMap_absToRel(VALUE self, VALUE x, VALUE y);
VALUE GameMap_relToAbs(VALUE self, VALUE x, VALUE y);
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

VALUE GameMap_addBetween(VALUE self, VALUE obj);

void defineGameMap();

#endif
