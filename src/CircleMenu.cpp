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

#define OSL_CIRCLE_MENU _OSL_CIRCLE_MENU

/*
  Simply taking code from the unmainted devslib.
  This might allow to use a maintained library like the oslib mod.
*/

void _oslDeleteCircleMenu(OSL_CIRCLE_MENU *menu) {
   oslDeleteImage(menu->Image);
   free(menu);
}

float Devsmod_f(float x, float y) {
   if (x < 0)
      x += y * (int)((-x)/y);
   if (x >= y)
      x -= y * (int)(x/y);
   if (x < 0)
      x += y;
   return x;
}

float DevsrangeAnglef(float angle)
{
   if (angle >= 360.f)
      angle -= 360.f;
   if (angle < 0.f)
      angle += 360.f;
   return angle;
}

float DevsadapteAnglef(float angle1, float angle2)
{
   if (angle2 > angle1 + 180.f)
      angle2 -= 360.f;
   if (angle2 < angle1 - 180.f)
      angle2 += 360.f;
   return angle2;
}

void DevsmenuDrawImageFloat(OSL_IMAGE *img, float x, float y,
                            float stretchX, float stretchY) {
   OSL_PRECISE_VERTEX *vertices;

   // do a striped blit (takes the page-cache into account)
   oslCorrectImageHalfBorder(img);
   oslSetTexture(img);

   vertices = (OSL_PRECISE_VERTEX*)sceGuGetMemory(2 * sizeof(OSL_PRECISE_VERTEX));

   vertices[0].u = img->offsetX0;
   vertices[0].v = img->offsetY0;
   vertices[0].x = x;
   vertices[0].y = y;
   vertices[0].z = 0;

   vertices[1].u = img->offsetX1;
   vertices[1].v = img->offsetY1;
   vertices[1].x = x + stretchX;
   vertices[1].y = y + stretchY;
   vertices[1].z = 0;

   sceGuDrawArray(GU_SPRITES,
                  GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_2D,
                  2, 0, vertices);
}

OSL_CIRCLE_MENU *_oslCreateCircleMenu(char *file,
                                      int nbIcons,
                                      int dist,
                                      int index,
                                      int icon_w, int icon_h,
                                      int x, int y,
                                      int anim_w, int anim_h) {
   OSL_CIRCLE_MENU *menu;
   menu = (OSL_CIRCLE_MENU*)malloc(sizeof(OSL_CIRCLE_MENU));
   if (!menu)
      return NULL;

   memset(menu, 0, sizeof(OSL_CIRCLE_MENU));

   menu->Image = oslLoadImageFile(file, OSL_IN_RAM, OSL_PF_5551);
   menu->nbIcones = nbIcons;
   menu->Eloignement = dist;
   menu->SelectionEncour = index;
   menu->TailleIconeX = icon_w;
   menu->TailleIconeY = icon_h;
   menu->PossitionX = x;
   menu->PossitionY = y;
   menu->LargeurAnimX = anim_w;
   menu->LargeurAnimY = anim_h;

   menu->AngleVoullue = 360.f - (menu->SelectionEncour * 360.f / menu->nbIcones);
   menu->test_angle = Devsmod_f(menu->AngleVoullue + 224.f, 360.f);

   return menu;
}

void DevsmenuDrawIcons_circle(OSL_CIRCLE_MENU *MenuS,int angle, int Eloignement) {
   int i;
   int iconOffsetY = 0;
   float scale, x, y, stretchX, stretchY;
   int angleDiff = 360 / MenuS->nbIcones;

   angle -= 90;
   for (i=0;i<MenuS->nbIcones;i++)               {
      y = MenuS->PossitionX - oslSin(angle, MenuS->LargeurAnimX + Eloignement);
      x = MenuS->PossitionY - 4 + oslCos(angle, MenuS->LargeurAnimY + Eloignement);
      scale = (y) / 60.0f;
      oslSetImageTileSize(MenuS->Image, 0, iconOffsetY, MenuS->TailleIconeX, MenuS->TailleIconeY);
      stretchX = MenuS->TailleIconeX * scale;
      stretchY = MenuS->TailleIconeY * scale;

      oslSetBilinearFilter((int)(stretchX+0.5f) != MenuS->TailleIconeX || (int)(stretchY+0.5f) != MenuS->TailleIconeY);
      DevsmenuDrawImageFloat(MenuS->Image, x-(stretchY/2), y, stretchX, stretchY);
      angle = (angle + angleDiff) % 360;

      iconOffsetY += MenuS->TailleIconeY;
   }
   oslSetBilinearFilter(0);
}

void _oslMenuUpdateIcons(OSL_CIRCLE_MENU *MenuS)
{
   if (MenuS->Eloignement !=0)MenuS->Eloignement -=2;

   DevsmenuDrawIcons_circle(MenuS,MenuS->test_angle, MenuS->Eloignement);


   if (MenuS->Eloignement > 0)
   {
      MenuS->AlphaMenu = MAX(256 - (MenuS->Eloignement * 4), 0);
      oslSetAlpha(OSL_FX_ALPHA, MenuS->AlphaMenu);
   }
   else
      MenuS->AlphaMenu = 256;



   if (MenuS->Eloignement > 0)                   {
      MenuS->test_angle = DevsrangeAnglef(MenuS->test_angle + 4.f);
   }
   else          {
      float tmpMenuAngle;
      //Arrivée du cercle terminée
      MenuS->Eloignement = 0;
      tmpMenuAngle = DevsadapteAnglef(MenuS->AngleVoullue, MenuS->test_angle);
      if (tmpMenuAngle < MenuS->AngleVoullue)
         MenuS->test_angle += (MenuS->AngleVoullue - tmpMenuAngle) / 5.f;
      else if (tmpMenuAngle > MenuS->AngleVoullue)
         MenuS->test_angle -= (tmpMenuAngle - MenuS->AngleVoullue) / 5.f;
   }
   //Gamme d'angle valable
   MenuS->test_angle = DevsrangeAnglef(MenuS->test_angle);

}

void _oslMenuMoveIcons(OSL_CIRCLE_MENU *MenuS,int sens)
{
   if (sens)
   {
      MenuS->SelectionEncour --;
      MenuS->Eloignement = 0;
      MenuS->SelectionEncour = MenuS->SelectionEncour % MenuS->nbIcones;
      if (MenuS->SelectionEncour < 0)
         MenuS->SelectionEncour += MenuS->nbIcones;
      MenuS->AngleVoullue = 360.f - (MenuS->SelectionEncour * 360.f / MenuS->nbIcones);
   }
   else
   {
      MenuS->SelectionEncour ++;
      MenuS->Eloignement = 0;
      MenuS->SelectionEncour = MenuS->SelectionEncour % MenuS->nbIcones;
      if (MenuS->SelectionEncour > MenuS->nbIcones)
         MenuS->SelectionEncour = 0;
      MenuS->AngleVoullue = 360.f - (MenuS->SelectionEncour * 360.f / MenuS->nbIcones);
   }

}

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
   _menu->TailleIconeX = obj._menu->TailleIconeX;
   _menu->TailleIconeY = obj._menu->TailleIconeY;
   _menu->Eloignement = obj._menu->Eloignement;
   _menu->test_angle = obj._menu->test_angle;
   _menu->PossitionX = obj._menu->PossitionX;
   _menu->PossitionY = obj._menu->PossitionY;
   _menu->LargeurAnimX = obj._menu->LargeurAnimX;
   _menu->LargeurAnimY = obj._menu->LargeurAnimY;

   setClass("CircleMenu");
}

CircleMenu::~CircleMenu()
{
   if (_menu != NULL)
      _oslDeleteCircleMenu(_menu);
}

void CircleMenu::load(char *str, int nbIcons, int dist, int w, int h,
                      int x, int y, int iconW, int iconH)
{
   _w = w;
   _h = h;
   _menu = _oslCreateCircleMenu(str, nbIcons, dist, 0, w, h, x, y, iconW, iconH);
}

void CircleMenu::changeSelection(int dir)
{
   _oslMenuMoveIcons(_menu, dir);
}

int CircleMenu::getIndex()
{
   // SelectionEncour should be the menu index.
   return _menu->SelectionEncour;
}

void CircleMenu::draw()
{
   _oslMenuUpdateIcons(_menu);
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

   str = rb_obj_as_string(str);
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
