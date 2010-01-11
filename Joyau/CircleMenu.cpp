/*Copyright (C) 2009-2010 Verhetsel Kilian

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

#include "CircleMenu.hpp"

CircleMenu::CircleMenu()
{
   _menu = NULL;
   setClass("CircleMenu");
}

CircleMenu::CircleMenu(const CircleMenu &obj)
{
   _menu = new OSL_CIRCLE_MENU;
   _menu->AlphaMenu = obj._menu->AlphaMenu;
   _menu->nbIcones = obj._menu->nbIcones;
   _menu->SelectionEncour = obj._menu->SelectionEncour;
   _menu->Image = oslCreateImageCopy(obj._menu->Image, OSL_IN_RAM);
   _menu->sizeX = obj._menu->sizeX;
   _menu->sizeY = obj._menu->sizeY;
   _menu->Eloignement = obj._menu->Eloignement;
   _menu->test_angle = obj._menu->test_angle;
   _menu->X = obj._menu->X;
   _menu->Y = obj._menu->Y;
   _menu->LargeurAnimX = obj._menu->LargeurAnimX;
   _menu->LargeurAnimY = obj._menu->LargeurAnimY;

   setClass("CircleMenu");
}

CircleMenu::~CircleMenu()
{
   if (_menu != NULL)
      oslDeleteCircleMenu(_menu);
}

void CircleMenu::load(char *str, int nbIcons, int dist, int w, int h, 
		      int x, int y, int iconW, int iconH)
{
   _w = w;
   _h = h;
   _menu = oslCreateCircleMenu(str, nbIcons, dist, 0, w, h, x, y, iconW, iconH);
}

void CircleMenu::changeSelection(int dir)
{
   oslMenuMoveIcons(_menu, dir);
}

int CircleMenu::getIndex()
{
   // SelectionEncour should be the menu index.
   return _menu->SelectionEncour;
}

void CircleMenu::draw()
{
   oslMenuUpdateIcons(_menu);
}

void CircleMenu::setAlpha(int alpha)
{
   _menu->AlphaMenu = alpha;
}

void CircleMenu::setDist(int dist)
{
   _menu->Eloignement = dist;
}

void CircleMenu::setAngle(int angle)
{
   _menu->AngleVoullue = angle;
}

/*
  Document-class: Joyau::CircleMenu

  Displays a menu as a Circle. Its picture has to dispose the icons vertically.
*/

/*
  call-seq: load(filename, nb_icons, dist, w, h, x, y, icon_width, icon_height)
  
  Loads the menu.
*/
VALUE CircleMenu_load(VALUE self, VALUE str, VALUE nbIcons, VALUE dist, VALUE w, 
		      VALUE h, VALUE x, VALUE y, VALUE iconW, VALUE iconH)
{
   CircleMenu &ref = getRef<CircleMenu>(self);
   
   char *_str = StringValuePtr(str);
   int _nbIcons = FIX2INT(nbIcons);
   int _dist = FIX2INT(dist);
   int _w = FIX2INT(w);
   int _h = FIX2INT(h);
   int _x = FIX2INT(x);
   int _y = FIX2INT(y);
   int _iconW = FIX2INT(iconW);
   int _iconH = FIX2INT(iconH);

   ref.load(_str, _nbIcons, _dist, _w, _h, _x, _y, _iconW, _iconH);
   return Qnil;
}

/*
  call-seq: changeSelection(dir)

  Changes the selected item. 0 for the right, 1 for the left.
*/
VALUE CircleMenu_changeSelection(VALUE self, VALUE dir)
{
   CircleMenu &ref = getRef<CircleMenu>(self);
   int _dir = FIX2INT(dir);

   ref.changeSelection(_dir);
   return Qnil;
}

/*
  Returns the menu's index.
*/
VALUE CircleMenu_getIndex(VALUE self)
{
   CircleMenu &ref = getRef<CircleMenu>(self);
   int ret = ref.getIndex();

   return INT2FIX(ret);
}

/*
  call-seq: alpha=(alpha)

  Sets the menu's alpha value.
*/
VALUE CircleMenu_setAlpha(VALUE self, VALUE alpha)
{
   CircleMenu &ref = getRef<CircleMenu>(self);
   int _alpha = FIX2INT(alpha);

   ref.setAlpha(_alpha);
   return alpha;
}

/*
  call-seq: dist=(dist)
 
  Sets the menu's distance.
*/
VALUE CircleMenu_setDist(VALUE self, VALUE dist)
{
   CircleMenu &ref = getRef<CircleMenu>(self);
   int _dist = FIX2INT(dist);
   
   ref.setDist(_dist);
   return dist;
}

/*
  call-seq: angle=(angle)
 
  Sets the menu's angle.
*/
VALUE CircleMenu_setAngle(VALUE self, VALUE angle)
{
   CircleMenu &ref = getRef<CircleMenu>(self);
   int _angle = FIX2INT(angle);
   
   ref.setDist(_angle);
   return angle;
}

/*
  Returns the menu's alpha value.
*/
VALUE CircleMenu_alpha(VALUE self)
{
   CircleMenu &ref = getRef<CircleMenu>(self);
   return INT2FIX(ref.getAlpha());
}

/*
  Returns the menu's distance
*/
VALUE CircleMenu_dist(VALUE self)
{
   CircleMenu &ref = getRef<CircleMenu>(self);
   return INT2FIX(ref.getDist());
}

/*
  Returns the menu's angle.
 */
VALUE CircleMenu_angle(VALUE self)
{
   CircleMenu &ref = getRef<CircleMenu>(self);
   return INT2FIX(ref.getAngle());
}

void defineCircleMenu()
{
   VALUE cCircleMenu = defClass<CircleMenu>("CircleMenu", "Drawable");
   defMethod(cCircleMenu, "load", CircleMenu_load, 9);
   defMethod(cCircleMenu, "changeSelection", CircleMenu_changeSelection, 1);
   defMethod(cCircleMenu, "getIndex", CircleMenu_getIndex, 0);
   defMethod(cCircleMenu, "setAlpha", CircleMenu_setAlpha, 1);
   defMethod(cCircleMenu, "setDist", CircleMenu_setDist, 1);
   defMethod(cCircleMenu, "setAngle", CircleMenu_setAngle, 1);
   defMethod(cCircleMenu, "alpha", CircleMenu_alpha, 0);
   defMethod(cCircleMenu, "angle", CircleMenu_angle, 0);
   defMethod(cCircleMenu, "dist", CircleMenu_dist, 0);

   defAlias(cCircleMenu, "getIndex", "index");
   defAlias(cCircleMenu, "setAlpha", "alpha=");
   defAlias(cCircleMenu, "setDist", "dist=");
   defAlias(cCircleMenu, "setAngle", "angle=");
   defAlias(cCircleMenu, "changeSelection", "change_selection");

   defConst(cCircleMenu, "RIGHT", INT2FIX(0));
   defConst(cCircleMenu, "LEFT", INT2FIX(1));
}
