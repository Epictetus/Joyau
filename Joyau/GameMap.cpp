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

#include "GameMap.hpp"
#include "DrawableRect.hpp"
#include "Line.hpp"

template<> VALUE wrap<CollisionType>(int argc, VALUE *argv, VALUE info)
{
   CollisionType *ptr = new CollisionType;
   if (argc >= 1)
   {
      ptr->content = argv[0] == Qtrue;
      if (argc >= 5)
      {
	 ptr->left = argv[1] == Qtrue;
	 ptr->right = argv[2] == Qtrue;
	 ptr->up = argv[3] == Qtrue;
	 ptr->down = argv[4] == Qtrue;
      }
   }

   VALUE tdata = Data_Wrap_Struct(info, 0, wrapped_free<CollisionType>, ptr);
   return tdata;
}

template<> VALUE wrap<GameMap>(int argc, VALUE *argv, VALUE info)
{
   GameMap *ptr = new GameMap;
   if (argc >= 1)
   {
      if (TYPE(argv[0]) == T_ARRAY)
      {
	 int size = RARRAY_LEN(argv[0]);
	 for (int i = 0; i < size; ++i)
	 {
	    VALUE val = rb_ary_entry(argv[0], i);
	    ptr->addTileset(StringValuePtr(val));
	 }
      }
      else
      {
      	 for (int i = 0; i < argc; ++i)
	    ptr->addTileset(StringValuePtr(argv[i]));
      }
   }

   VALUE tdata = Data_Wrap_Struct(info, 0, wrapped_free<GameMap>, ptr);
   return tdata;
}

template<> VALUE wrap<GameMap::Tile>(int argc, VALUE *argv, VALUE info)
{
   GameMap::Tile *ptr = new GameMap::Tile;
   
   if (argc >= 5)
   {
      ptr->tileset = FIX2INT(argv[0]);
      ptr->tileX = FIX2INT(argv[1]);
      ptr->tileY = FIX2INT(argv[2]);
      ptr->x = FIX2INT(argv[3]);
      ptr->y = FIX2INT(argv[4]);
      if (argc == 6)
	 ptr->type = getRef<CollisionType>(argv[5]);
   }
   
   VALUE tdata = Data_Wrap_Struct(info, 0, wrapped_free<GameMap::Tile>, ptr);
   return tdata;
}

GameMap::GameMap()
{
   _w = 480;
   _h = 272;
   colH = -1;
}

void GameMap::addTileset(char *name)
{
   Sprite spr;
   spr.setPicture(name);

   tilesets.push_back(spr);
}

void GameMap::setTileSize(int w, int h)
{
   tileWidth = w;
   tileHeight = h;
}

Point GameMap::absToRel(int x, int y) const
{
   return Point(x - _x, y - _y);
}

Point GameMap::relToAbs(int x, int y) const
{
   return Point(x + _x, y + _y);
}

void GameMap::centerOn(int x, int y)
{
   Point p = relToAbs(x, y);
   move(240 - p.x, 136 - p.y);
}

void GameMap::addElem(int tileset, int tX, int tY, int x, int y)
{
   Tile t;
   t.tileset = tileset;
   t.tileX = tX;
   t.tileY = tY;
   t.x = x;
   t.y = y;

   tiles.push_back(t);
}

void GameMap::addElem(const Tile &tile)
{
   tiles.push_back(tile);
}

bool GameMap::collide(Drawable &spr)
{
   DrawableRect tmp;
   if (colH != -1)
   {
      Rect rect = spr.boundingRect();
      tmp.setPos(rect.x, rect.y + rect.h - colH);
      tmp.resize(rect.w, colH);
   }
   Drawable &col = colH != -1 ? tmp : spr;

   for (unsigned int i = 0; i < tiles.size(); ++i)
   {
      Sprite &tile = tilesets[tiles[i].tileset];
      tile.setTile(tiles[i].tileX, tiles[i].tileY, tileWidth, tileHeight);
      
      // Coord relative to the map !
      tile.setPos(getX() + tiles[i].x, getY() + tiles[i].y);

      Line line;

      if (tiles[i].type.right)
      {
	 line.setPos(getX() + tiles[i].x + tileWidth, getY() + tiles[i].y);
	 line.setPoint(getX() + tiles[i].x + tileWidth, 
		       getY() + tiles[i].y + tileHeight);
	 if (col.collide(line))
	    return true;
      }

      if (tiles[i].type.left)
      {
	 line.setPos(getX() + tiles[i].x, getY() + tiles[i].y);
	 line.setPoint(getX() + tiles[i].x, getY() + tiles[i].y + tileHeight);
	 if (col.collide(line))
	    return true;
      }

      if (tiles[i].type.up)
      {
	 line.setPos(getX() + tiles[i].x, getY() + tiles[i].y);
	 line.setPoint(getX() + tiles[i].x + tileWidth, getY() + tiles[i].y);
	 if (col.collide(line))
	    return true;
      }

      if (tiles[i].type.down)
      {
	 line.setPos(getX() + tiles[i].x, getY() + tiles[i].y + tileHeight);
	 line.setPoint(getX() + tiles[i].x + tileWidth, 
		       getY() + tiles[i].y + tileHeight);
	 if (col.collide(line))
	    return true;
      }

      if (tiles[i].type.content)
	 if (col.collide(tile))
	    return true; // If it collides with only one tile, it collides..
   }
   return false; // It didn't collide with any tiles.
}

bool GameMap::isOn(int x, int y)
{
   for (unsigned int i = 0; i< tiles.size(); ++i)
   {
      Sprite &tile = tilesets[tiles[i].tileset];
      tile.setTile(tiles[i].tileX, tiles[i].tileY, tileWidth, tileHeight);
      tile.setPos(getX() + tiles[i].x, getY() + tiles[i].y);

      if (tile.isOn(x, y))
         return true;
   }
   return false;
}

void GameMap::clear()
{
   tiles.clear(); // Let's say the user'd want to use the same object twice
   tilesets.clear();
}

bool GameMap::visible(const Tile &t) const
{
   int x = t.x + getX();
   int y = t.y + getY();
   int w = tileWidth;
   int h = tileHeight;

   if (x + w < 0 || // Not even visible at the screen left side
       x + h < 0 || // Not even visible at the screen upper side
       x - w > 480 || // Not even visible at the screen right side
       y - h > 272 ) // Not even visible at the screen down side
       return false;
   return true;
}

void GameMap::draw()
{
   for (unsigned int i = 0; i < tiles.size(); ++i)
   {
      /*
        Don't waste time, don't draw not visible tiles.
        ( It's also possible that the size of the map is inferior
	to the screen size )
      */
      if (visible(tiles[i]))
      {
         Sprite &tile = tilesets[tiles[i].tileset];
         tile.setTile(tiles[i].tileX, tiles[i].tileY, tileWidth, tileHeight);
         tile.setPos(getX() + tiles[i].x, getY() + tiles[i].y);

	 tile.draw();
      }
   }
}

void GameMap::rbEachTile()
{
   for (unsigned int i = 0; i < tiles.size(); ++i)
   {
      VALUE obj = tiles[i].toRuby();
      rb_yield(obj);
   }
}

void GameMap::rbEachTileset()
{
   for (unsigned int i = 0; i < tilesets.size(); ++i)
   {
      VALUE obj = tilesets[i].toRuby();
      rb_yield(obj);
   }
}

void GameMap::rbRejectTiles()
{
   std::remove_if(tiles.begin(), tiles.end(), shouldRemove());
}

VALUE GameMap::rbTiles()
{
   VALUE ret = rb_ary_new();
   for (unsigned int i = 0; i < tiles.size(); ++i)
      rb_ary_push(ret, tiles[i].toRuby());

   return ret;
}

VALUE GameMap::rbTilesets()
{
   VALUE ret = rb_ary_new();
   for (unsigned int i = 0; i < tilesets.size(); ++i)
      rb_ary_push(ret, tilesets[i].toRuby());

   return ret;
}

VALUE GameMap_addTileset(VALUE self, VALUE name)
{
   GameMap &ref = getRef<GameMap>(self);

   ref.addTileset(StringValuePtr(name));
   return Qnil;
}

VALUE GameMap_setTileSize(VALUE self, VALUE w, VALUE h)
{
   GameMap &ref = getRef<GameMap>(self);
   int _w = FIX2INT(w);
   int _h = FIX2INT(h);

   ref.setTileSize(_w, _h);
   return Qnil;
}

VALUE GameMap_tileWidth(VALUE self)
{
   GameMap &ref = getRef<GameMap>(self);
   return INT2FIX(ref.getTileW());
}

VALUE GameMap_tileHeight(VALUE self)
{
   GameMap &ref = getRef<GameMap>(self);
   return INT2FIX(ref.getTileH());
}

VALUE GameMap_setCollisionH(VALUE self, VALUE val)
{
   GameMap &ref = getRef<GameMap>(self);
   ref.setCollisionH(FIX2INT(val));

   return val;
}

VALUE GameMap_collisionH(VALUE self)
{
   GameMap &ref = getRef<GameMap>(self);
   return INT2FIX(ref.getCollisionH());
}

VALUE GameMap_absToRel(VALUE self, VALUE x, VALUE y)
{
   GameMap &ref = getRef<GameMap>(self);
   Point ret = ref.absToRel(FIX2INT(x), FIX2INT(y));

   return createObject(getClass("Point"), ret);
}

VALUE GameMap_relToAbs(VALUE self, VALUE x, VALUE y)
{
   GameMap &ref = getRef<GameMap>(self);
   Point ret = ref.relToAbs(FIX2INT(x), FIX2INT(y));

   return createObject(getClass("Point"), ret);
}

VALUE GameMap_centerOn(VALUE self, VALUE x, VALUE y)
{
   GameMap &ref = getRef<GameMap>(self);
   ref.centerOn(FIX2INT(x), FIX2INT(y));

   return Qnil;
}

VALUE GameMap_addElem(int argc, VALUE *argv, VALUE self)
{
   if (argc == 5)
   {
      GameMap &ref = getRef<GameMap>(self);
      int tileset = FIX2INT(argv[0]);
      int tX = FIX2INT(argv[1]);
      int tY = FIX2INT(argv[2]);
      int x = FIX2INT(argv[3]);
      int y = FIX2INT(argv[4]);

      ref.addElem(tileset, tX, tY, x, y);
   }
   else
      GameMap_push(self, argv[0]);
   return self;
}

VALUE GameMap_push(VALUE self, VALUE tile)
{
   GameMap &ref = getRef<GameMap>(self);
   if (TYPE(tile) == T_ARRAY)
   {
      int size = RARRAY_LEN(tile);
      for (int i = 0; i < size; ++i)
      {
	 VALUE val = rb_ary_entry(tile, i);
	 ref.addElem(getRef<GameMap::Tile>(val));
      }
   }
   else
   {
      GameMap::Tile &tRef = getRef<GameMap::Tile>(tile);
      ref.addElem(tRef);
   }
   
   return self;
}

VALUE GameMap_clear(VALUE self)
{
   GameMap &ref = getRef<GameMap>(self);
   ref.clear();

   return Qnil;
}

VALUE GameMap_clearTiles(VALUE self)
{
   GameMap &ref = getRef<GameMap>(self);
   ref.clearTiles();

   return Qnil;
}

VALUE GameMap_tiles(VALUE self)
{
   GameMap &ref = getRef<GameMap>(self);
   return ref.rbTiles();
}

VALUE GameMap_tilesets(VALUE self)
{
   GameMap &ref = getRef<GameMap>(self);
   return ref.rbTilesets();
}

VALUE GameMap_each_tile(VALUE self)
{
   GameMap &ref = getRef<GameMap>(self);
   ref.rbEachTile();

   return Qnil;
}

VALUE GameMap_each_tileset(VALUE self)
{
   GameMap &ref = getRef<GameMap>(self);
   ref.rbEachTileset();
   
   return Qnil;
}

VALUE GameMap_reject_tiles(VALUE self)
{
   GameMap &ref = getRef<GameMap>(self);
   ref.rbRejectTiles();

   return Qnil;
}

VALUE CollisionType_right(VALUE self)
{
   CollisionType &ref = getRef<CollisionType>(self);
   return ref.right ? Qtrue : Qfalse;
}

VALUE CollisionType_left(VALUE self)
{
   CollisionType &ref = getRef<CollisionType>(self);
   return ref.left ? Qtrue : Qfalse;
}

VALUE CollisionType_up(VALUE self)
{
   CollisionType &ref = getRef<CollisionType>(self);
   return ref.up ? Qtrue : Qfalse;
}

VALUE CollisionType_down(VALUE self)
{
   CollisionType &ref = getRef<CollisionType>(self);
   return ref.down ? Qtrue : Qfalse;
}

VALUE CollisionType_content(VALUE self)
{
   CollisionType &ref = getRef<CollisionType>(self);
   return ref.content ? Qtrue : Qfalse;
}

VALUE CollisionType_setRight(VALUE self, VALUE val)
{
   CollisionType &ref = getRef<CollisionType>(self);
   ref.right = val == Qtrue;

   return val;
}

VALUE CollisionType_setLeft(VALUE self, VALUE val)
{
   CollisionType &ref = getRef<CollisionType>(self);
   ref.left = val == Qtrue;

   return val;
}

VALUE CollisionType_setUp(VALUE self, VALUE val)
{
   CollisionType &ref = getRef<CollisionType>(self);
   ref.up = val == Qtrue;

   return val;
}

VALUE CollisionType_setDown(VALUE self, VALUE val)
{
   CollisionType &ref = getRef<CollisionType>(self);
   ref.down = val == Qtrue;

   return val;
}

VALUE CollisionType_setContent(VALUE self, VALUE val)
{
   CollisionType &ref = getRef<CollisionType>(self);
   ref.content = val == Qtrue;

   return val;
}

VALUE Tile_tileX(VALUE self)
{
   return INT2FIX(getRef<GameMap::Tile>(self).tileX);
}

VALUE Tile_tileY(VALUE self)
{
   return INT2FIX(getRef<GameMap::Tile>(self).tileY);
}

VALUE Tile_x(VALUE self)
{
   return INT2FIX(getRef<GameMap::Tile>(self).x);
}

VALUE Tile_y(VALUE self)
{
   return INT2FIX(getRef<GameMap::Tile>(self).y);
}

VALUE Tile_tileset(VALUE self)
{
   return INT2FIX(getRef<GameMap::Tile>(self).tileset);
}

VALUE Tile_type(VALUE self)
{
   GameMap::Tile &ref = getRef<GameMap::Tile>(self);
   return ref.type.toRuby();
}

VALUE Tile_setTileX(VALUE self, VALUE val)
{
   GameMap::Tile &ref = getRef<GameMap::Tile>(self);
   ref.tileX = FIX2INT(val);

   return val;
}

VALUE Tile_setTileY(VALUE self, VALUE val)
{
   GameMap::Tile &ref = getRef<GameMap::Tile>(self);
   ref.tileY = FIX2INT(val);

   return val;
}

VALUE Tile_setX(VALUE self, VALUE val)
{
   GameMap::Tile &ref = getRef<GameMap::Tile>(self);
   ref.x = FIX2INT(val);

   return val;
}

VALUE Tile_setY(VALUE self, VALUE val)
{
   GameMap::Tile &ref = getRef<GameMap::Tile>(self);
   ref.y = FIX2INT(val);

   return val;
}

VALUE Tile_setTileset(VALUE self, VALUE val)
{
   GameMap::Tile &ref = getRef<GameMap::Tile>(self);
   ref.tileset = FIX2INT(val);

   return val;
}

VALUE Tile_setType(VALUE self, VALUE val)
{
   GameMap::Tile &ref = getRef<GameMap::Tile>(self);
   CollisionType &type = getRef<CollisionType>(val);
   
   ref.type = type;
   return val;
}

void defineGameMap()
{
   VALUE cCollisionType = defClass<CollisionType>("CollisionType");
   defMethod(cCollisionType, "left", CollisionType_left, 0);
   defMethod(cCollisionType, "right", CollisionType_right, 0);
   defMethod(cCollisionType, "up", CollisionType_up, 0);
   defMethod(cCollisionType, "down", CollisionType_down, 0);
   defMethod(cCollisionType, "content", CollisionType_content, 0);

   defMethod(cCollisionType, "left=", CollisionType_setLeft, 1);
   defMethod(cCollisionType, "right=", CollisionType_setRight, 1);
   defMethod(cCollisionType, "up=", CollisionType_setUp, 1);
   defMethod(cCollisionType, "down=", CollisionType_setDown, 1);
   defMethod(cCollisionType, "content=", CollisionType_setContent, 1);
   
   VALUE cTile = defClass<GameMap::Tile>("Tile");
   defMethod(cTile, "x", Tile_x, 0);
   defMethod(cTile, "y", Tile_y, 0);
   defMethod(cTile, "tileset", Tile_tileset, 0);
   defMethod(cTile, "tileX", Tile_tileX, 0);
   defMethod(cTile, "tileY", Tile_tileY, 0);
   defMethod(cTile, "type", Tile_type, 0);

   defMethod(cTile, "x=", Tile_setX, 1);
   defMethod(cTile, "y=", Tile_setY, 1);
   defMethod(cTile, "tileset=", Tile_setTileset, 1);
   defMethod(cTile, "tileX=", Tile_setTileX, 1);
   defMethod(cTile, "tileY=", Tile_setTileY, 1);
   defMethod(cTile, "type=", Tile_setType, 1);
 
   VALUE cMap = defClass<GameMap>("GameMap", "Drawable");
   defMethod(cMap, "addTileset", GameMap_addTileset, 1);
   defMethod(cMap, "setTileSize", GameMap_setTileSize, 2);
   defMethod(cMap, "tileWidth", GameMap_tileWidth, 0);
   defMethod(cMap, "tileHeight", GameMap_tileHeight, 0);
   defMethod(cMap, "collisionH=", GameMap_setCollisionH, 1);
   defMethod(cMap, "collisionH", GameMap_collisionH, 0);
   defMethod(cMap, "absToRel", GameMap_absToRel, 2);
   defMethod(cMap, "relToAbs", GameMap_relToAbs, 2);
   defMethod(cMap, "centerOn", GameMap_centerOn, 2);
   defMethod(cMap, "addElem", GameMap_addElem, -1);
   defMethod(cMap, "<<", GameMap_push, 1);
   defMethod(cMap, "clear", GameMap_clear, 0);
   defMethod(cMap, "clearTiles", GameMap_clearTiles, 0);
   defMethod(cMap, "tiles", GameMap_tiles, 0);
   defMethod(cMap, "tilesets", GameMap_tilesets, 0);
   defMethod(cMap, "each_tile", GameMap_each_tile, 0);
   defMethod(cMap, "each_tileset", GameMap_each_tileset, 0);
   defMethod(cMap, "reject_tiles", GameMap_reject_tiles, 0);

   CollisionType full(true, false, false, false, false);
   CollisionType left(false, true, false, false, false);
   CollisionType right(false, false, true, false, false);
   CollisionType up(false, false, false, true, false);
   CollisionType down(false, false, false, false, true);
   CollisionType no(false, false, false, false, false);
   CollisionType left_right(false, true, true, false, false);
   CollisionType left_up(false, true, false, true, false);
   CollisionType left_down(false, true, false, false, true);
   CollisionType right_up(false, false, true, true, false);
   CollisionType right_down(false, false, true, false, true);
   CollisionType up_down(false, false, false, true, true);

   defConst(cMap, "COL_FULL", createObject(cCollisionType, full));
   defConst(cMap, "COL_LEFT", createObject(cCollisionType, left));
   defConst(cMap, "COL_RIGHT", createObject(cCollisionType, right));
   defConst(cMap, "COL_UP", createObject(cCollisionType, up));
   defConst(cMap, "COL_DOWN", createObject(cCollisionType, down));
   defConst(cMap, "COL_NO", createObject(cCollisionType, no));
   defConst(cMap, "COL_LEFT_RIGHT", createObject(cCollisionType, left_right));
   defConst(cMap, "COL_LEFT_UP", createObject(cCollisionType, left_up));
   defConst(cMap, "COL_LEFT_DOWN", createObject(cCollisionType, left_down));
   defConst(cMap, "COL_RIGHT_UP", createObject(cCollisionType, right_up));
   defConst(cMap, "COL_RIGHT_DOWN", createObject(cCollisionType, right_down));
   defConst(cMap, "COL_UP_DOWN", createObject(cCollisionType, up_down));
}
