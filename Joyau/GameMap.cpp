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

using namespace std;

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
	 int size = RARRAY(argv[0])->len;
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
bool GameMap::shouldRemove::operator()(const Tile &t)
{
   VALUE obj = createObject(getClass("Tile"), t);
   return rb_yield(obj) == Qtrue;
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

   for (list<Tile>::iterator i = tiles.begin(); i != tiles.end(); ++i)
   {
      Sprite &tile = tilesets[(*i).tileset];
      tile.setTile((*i).tileX, (*i).tileY, tileWidth, tileHeight);
      // Coord relative to the map !
      tile.setPos(getX() + (*i).x, getY() + (*i).y);

      Line line;

      if ((*i).type.right)
      {
	 line.setPos(getX() + (*i).x + tileWidth, getY() + (*i).y);
	 line.setPoint(getX() + (*i).x + tileWidth, 
		       getY() + (*i).y + tileHeight);
	 if (col.collide(line))
	    return true;
      }

      if ((*i).type.left)
      {
	 line.setPos(getX() + (*i).x, getY() + (*i).y);
	 line.setPoint(getX() + (*i).x, getY() + (*i).y + tileHeight);
	 if (col.collide(line))
	    return true;
      }

      if ((*i).type.up)
      {
	 line.setPos(getX() + (*i).x, getY() + (*i).y);
	 line.setPoint(getX() + (*i).x + tileWidth, getY() + (*i).y);
	 if (col.collide(line))
	    return true;
      }

      if ((*i).type.down)
      {
	 line.setPos(getX() + (*i).x, getY() + (*i).y + tileHeight);
	 line.setPoint(getX() + (*i).x + tileWidth, 
		       getY() + (*i).y + tileHeight);
	 if (col.collide(line))
	    return true;
      }

      if ((*i).type.content)
	 if (col.collide(tile))
	    return true; // If it collides with only one tile, it collides..
   }
   return false; // It didn't collide with any sprite.
}

bool GameMap::isOn(int x, int y)
{
   for (list<Tile>::iterator i = tiles.begin(); i != tiles.end(); ++i)
   {
      Sprite &tile = tilesets[(*i).tileset];
      tile.setTile((*i).tileX, (*i).tileY, tileWidth, tileHeight);
      tile.setPos(getX() + (*i).x, getY() + (*i).y);

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
   for (list<Tile>::iterator i = tiles.begin(); i != tiles.end(); ++i)
   {
      /*
        Don't waste time, don't draw not visible tiles.
        ( It's also possible that the size of the map is inferior
	to the screen size )
      */
      if (visible(*i))
      {
         Sprite &tile = tilesets[(*i).tileset];
         tile.setTile((*i).tileX, (*i).tileY, tileWidth, tileHeight);
         tile.setPos(getX() + (*i).x, getY() + (*i).y);

         tile.getImage();
         tile.draw();
      }
   }
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

   return Qnil;
}

VALUE GameMap_collisionH(VALUE self)
{
   GameMap &ref = getRef<GameMap>(self);
   return INT2FIX(ref.getCollisionH());
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
   return Qnil;
}

VALUE GameMap_push(VALUE self, VALUE tile)
{
   GameMap &ref = getRef<GameMap>(self);
   if (TYPE(tile) == T_ARRAY)
   {
      int size = RARRAY(tile)->len;
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
   
   return Qnil;
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
   list<GameMap::Tile> tiles = ref.getTiles();

   VALUE ret = rb_ary_new();

   for (list<GameMap::Tile>::iterator i = tiles.begin(); i != tiles.end(); ++i)
      rb_ary_push(ret, createObject(getClass("Tile"), *i));
   return ret;
}

VALUE GameMap_tilesets(VALUE self)
{
   GameMap &ref = getRef<GameMap>(self);
   vector<Sprite> tilesets = ref.getTilesets();

   VALUE ret = rb_ary_new();
   
   for (vector<Sprite>::iterator i = tilesets.begin(); i != tilesets.end(); ++i)
      rb_ary_push(ret, createObject(getClass("Sprite"), *i));
   return ret;
}

VALUE GameMap_each_tile(VALUE self)
{
   GameMap &ref = getRef<GameMap>(self);
   list<GameMap::Tile> &tiles = ref.getTiles();

   for (list<GameMap::Tile>::iterator i = tiles.begin(); i != tiles.end(); ++i)
   {
      VALUE obj = createObject(getClass("Tile"), *i);
      VALUE ret = rb_yield(obj);

      (*i) = getRef<GameMap::Tile>(ret);
   }

   return Qnil;
}

VALUE GameMap_each_tileset(VALUE self)
{
   GameMap &ref = getRef<GameMap>(self);
   vector<Sprite> &tilesets = ref.getTilesets();

   for (vector<Sprite>::iterator i = tilesets.begin(); i != tilesets.end(); ++i)
   {
      VALUE obj = createObject(getClass("Sprite"), *i);
      VALUE ret = rb_yield(obj);

      (*i) = getRef<Sprite>(ret);
   }

   return Qnil;
}

VALUE GameMap_reject_tiles(VALUE self)
{
   GameMap &ref = getRef<GameMap>(self);
   ref.getTiles().remove_if(GameMap::shouldRemove());

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

   return Qnil;
}

VALUE CollisionType_setLeft(VALUE self, VALUE val)
{
   CollisionType &ref = getRef<CollisionType>(self);
   ref.left = val == Qtrue;

   return Qnil;
}

VALUE CollisionType_setUp(VALUE self, VALUE val)
{
   CollisionType &ref = getRef<CollisionType>(self);
   ref.up = val == Qtrue;

   return Qnil;
}

VALUE CollisionType_setDown(VALUE self, VALUE val)
{
   CollisionType &ref = getRef<CollisionType>(self);
   ref.down = val == Qtrue;

   return Qnil;
}

VALUE CollisionType_setContent(VALUE self, VALUE val)
{
   CollisionType &ref = getRef<CollisionType>(self);
   ref.content = val == Qtrue;

   return Qnil;
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

VALUE Tile_setTileX(VALUE self, VALUE val)
{
   GameMap::Tile &ref = getRef<GameMap::Tile>(self);
   ref.tileX = FIX2INT(val);

   return Qnil;
}

VALUE Tile_setTileY(VALUE self, VALUE val)
{
   GameMap::Tile &ref = getRef<GameMap::Tile>(self);
   ref.tileY = FIX2INT(val);

   return Qnil;
}

VALUE Tile_setX(VALUE self, VALUE val)
{
   GameMap::Tile &ref = getRef<GameMap::Tile>(self);
   ref.x = FIX2INT(val);

   return Qnil;
}

VALUE Tile_setY(VALUE self, VALUE val)
{
   GameMap::Tile &ref = getRef<GameMap::Tile>(self);
   ref.y = FIX2INT(val);

   return Qnil;
}

VALUE Tile_setTileset(VALUE self, VALUE val)
{
   GameMap::Tile &ref = getRef<GameMap::Tile>(self);
   ref.tileset = FIX2INT(val);

   return Qnil;
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

   defMethod(cTile, "x=", Tile_setX, 1);
   defMethod(cTile, "y=", Tile_setY, 1);
   defMethod(cTile, "tileset=", Tile_setTileset, 1);
   defMethod(cTile, "tileX=", Tile_setTileX, 1);
   defMethod(cTile, "tileY=", Tile_setTileY, 1);
   
   VALUE cMap = defClass<GameMap>("GameMap", "Drawable");
   defMethod(cMap, "addTileset", GameMap_addTileset, 1);
   defMethod(cMap, "setTileSize", GameMap_setTileSize, 2);
   defMethod(cMap, "tileWidth", GameMap_tileWidth, 0);
   defMethod(cMap, "tileHeight", GameMap_tileHeight, 0);
   defMethod(cMap, "collisionH=", GameMap_setCollisionH, 1);
   defMethod(cMap, "collisionH", GameMap_collisionH, 0);
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
}
