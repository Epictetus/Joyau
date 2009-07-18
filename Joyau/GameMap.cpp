
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

GameMap::GameMap()
{
   _w = 480;
   _h = 272;
}

void GameMap::setPos(int x, int y)
{
   _x = x;
   _y = y;
}

void GameMap::move(int x, int y)
{
   _x += x;
   _y += y;
}

void GameMap::resize(int w, int h)
{
   _w = w;
   _h = h;
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

bool GameMap::collide(Sprite *spr)
{
   for (list<Tile>::iterator i = tiles.begin(); i != tiles.end(); ++i)
   {
      Sprite &tile = tilesets[(*i).tileset];
      tile.setTile((*i).tileX, (*i).tileY, tileWidth, tileHeight);
      tile.setPos(_x + (*i).x, _y + (*i).y); // Coord relative to the map !

      tile.getImage(); // We update informations

      if (tile.collide(*spr))
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
      tile.setPos(_x + (*i).x, _y + (*i).y);

      tile.getImage();

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

bool GameMap::visible(const Tile &t)
{
   int x = t.x + _x;
   int y = t.y + _y;
   int w = tileWidth;
   int h = tileHeight;

   if (x + w < 0 || // Not even visible at the screen left side
       x + h < 0 || // Not even visible at the screen upper side
       x - w > 480 || // Not even visible at the screen right side
       y - h > 272 || // Not even visible at the screen down side
       x + w < _x || // Not visible at the left side
       y + h < _y || // Not visible at the upper side
       x - w > _x + _w || // Not visible at the right side
       y - h > _y + _h ) // Not visible at the down side
      return false;
   return true;
}

void GameMap::draw()
{
   for (list<Tile>::iterator i = tiles.begin(); i != tiles.end(); ++i)
   {
      /*
        Don't wase time, don't draw not visible tiles.
        Also here to draw a map of which doesn't do t full screen.
      */
      if (visible(*i))
      {
         Sprite &tile = tilesets[(*i).tileset];
         tile.setTile((*i).tileX, (*i).tileY, tileWidth, tileHeight);
         tile.setPos(_x + (*i).x, _y + (*i).y);

         tile.getImage();
         tile.Draw();
      }
   }
}

VALUE GameMap_setPos(VALUE self, VALUE x, VALUE y)
{
   GameMap &ref = getRef<GameMap>(self);
   int _x = FIX2INT(x);
   int _y = FIX2INT(y);

   ref.setPos(_x, _y);
   return Qnil;
}

VALUE GameMap_move(VALUE self, VALUE x, VALUE y)
{
   GameMap &ref = getRef<GameMap>(self);
   int _x = FIX2INT(x);
   int _y = FIX2INT(y);

   ref.move(_x, _y);
   return Qnil;
}

VALUE GameMap_resize(VALUE self, VALUE w, VALUE h)
{
   GameMap &ref = getRef<GameMap>(self);
   int _w = FIX2INT(w);
   int _h = FIX2INT(h);

   ref.resize(_w, _h);
   return Qnil;
}

VALUE GameMap_getX(VALUE self)
{
   GameMap &ref = getRef<GameMap>(self);
   return INT2FIX(ref.getX());
}

VALUE GameMap_getY(VALUE self)
{
   GameMap &ref = getRef<GameMap>(self);
   return INT2FIX(ref.getY());
}

VALUE GameMap_getW(VALUE self)
{
   GameMap &ref = getRef<GameMap>(self);
   return INT2FIX(ref.getW());
}

VALUE GameMap_getH(VALUE self)
{
   GameMap &ref = getRef<GameMap>(self);
   return INT2FIX(ref.getH());
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

VALUE GameMap_addElem(VALUE self, VALUE tileset, VALUE tX, VALUE tY,
                      VALUE x, VALUE y)
{
   GameMap &ref = getRef<GameMap>(self);
   int _tileset = FIX2INT(tileset);
   int _tX = FIX2INT(tX);
   int _tY = FIX2INT(tY);
   int _x = FIX2INT(x);
   int _y = FIX2INT(y);

   ref.addElem(_tileset, _tX, _tY, _x, _y);
   return Qnil;
}

VALUE GameMap_collide(VALUE self, VALUE spr)
{
   GameMap &ref = getRef<GameMap>(self);
   Sprite *_spr = getPtr<Sprite>(self);

   if (ref.collide(_spr))
      return Qtrue;
   return Qfalse;
}

VALUE GameMap_isOn(VALUE self, VALUE x, VALUE y)
{
   GameMap &ref = getRef<GameMap>(self);
   int _x = FIX2INT(x);
   int _y = FIX2INT(y);

   if (ref.isOn(_x, _y))
      return Qtrue;
   return Qfalse;
}

VALUE GameMap_clear(VALUE self)
{
   GameMap *ptr = getPtr<GameMap>(self);
   ptr->clear();

   return Qnil;
}

VALUE GameMap_draw(VALUE self)
{
   GameMap &ref = getRef<GameMap>(self);
   ref.draw();

   return Qnil;
}

void defineGameMap()
{
   VALUE cMap = defClass<GameMap>("GameMap");
   defMethod(cMap, "setPos", GameMap_setPos, 2);
   defMethod(cMap, "move", GameMap_move, 2);
   defMethod(cMap, "resize", GameMap_resize, 2);
   defMethod(cMap, "getX", GameMap_getX, 0);
   defMethod(cMap, "getY", GameMap_getY, 0);
   defMethod(cMap, "getW", GameMap_getW, 0);
   defMethod(cMap, "getH", GameMap_getH, 0);
   defMethod(cMap, "addTileset", GameMap_addTileset, 1);
   defMethod(cMap, "setTileSize", GameMap_setTileSize, 2);
   defMethod(cMap, "addElem", GameMap_addElem, 5);
   defMethod(cMap, "collide", GameMap_collide, 1);
   defMethod(cMap, "isOn", GameMap_isOn, 2);
   defMethod(cMap, "clear", GameMap_clear, 0);
   defMethod(cMap, "draw", GameMap_draw, 0);
}
