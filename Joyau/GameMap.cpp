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

bool GameMap::collide(Drawable &spr)
{
   for (list<Tile>::iterator i = tiles.begin(); i != tiles.end(); ++i)
   {
      Sprite &tile = tilesets[(*i).tileset];
      tile.setTile((*i).tileX, (*i).tileY, tileWidth, tileHeight);
      // Coord relative to the map !
      tile.setPos(getX() + (*i).x, getY() + (*i).y);

      if (spr.collide(tile))
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
       y - h > 272 || // Not even visible at the screen down side
       x + w < getX() || // Not visible at the left side
       y + h < getY() || // Not visible at the upper side
       x - w > getX() + _w || // Not visible at the right side
       y - h > getY() + _h ) // Not visible at the down side
      return false;
   return true;
}

void GameMap::draw()
{
   clearMove();
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
         tile.setPos(getX() + (*i).x, getY() + (*i).y);

         tile.getImage();
         tile.draw();
      }
   }
}

VALUE GameMap_resize(VALUE self, VALUE w, VALUE h)
{
   GameMap &ref = getRef<GameMap>(self);
   int _w = FIX2INT(w);
   int _h = FIX2INT(h);

   ref.resize(_w, _h);
   return Qnil;
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

VALUE GameMap_clear(VALUE self)
{
   GameMap &ref = getRef<GameMap>(self);
   ref.clear();

   return Qnil;
}

void defineGameMap()
{
   VALUE cMap = defClass<GameMap>("GameMap", "Drawable");
   defMethod(cMap, "resize", GameMap_resize, 2);
   defMethod(cMap, "addTileset", GameMap_addTileset, 1);
   defMethod(cMap, "setTileSize", GameMap_setTileSize, 2);
   defMethod(cMap, "addElem", GameMap_addElem, 5);
   defMethod(cMap, "clear", GameMap_clear, 0);
}
