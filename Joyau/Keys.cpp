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

#include "Keys.hpp"

template<> VALUE wrap<Cursor>(int argc, VALUE *argv, VALUE info)
{
   Cursor *ptr = new Cursor;
   VALUE tdata;
   
   if (argc >= 1)
   {
      ptr->setPicture(StringValuePtr(argv[0]));
      if (argc >= 2)
	 ptr->setSensibility(FIX2INT(argv[1]));
   }
   
   tdata = Data_Wrap_Struct(info, 0, wrapped_free<Cursor>, ptr);
   return tdata;
}

void Cursor::updatePos()
{
   checkKeys(Qnil); // Let the pressed_ and released_ events bve raised.
   if (sensibility != 0)
   {
      int analogX = osl_pad.analogX;
      int analogY = osl_pad.analogY;

      int _x = getX();
      int _y = getY();

      _x += analogX / sensibility;
      _y += analogY / sensibility;

      _x = _x > 480 ? 480 : _x;
      _x = _x < 0 ? 0 : _x;

      _y = _y > 272 ? 272 : _y;
      _y = _y < 0 ? 0 : _y;

      setPos(_x, _y);
   }
}

VALUE Keys_repeatInit(VALUE self, VALUE time)
{
   oslSetKeyAutorepeat(OSL_KEYMASK_UP | OSL_KEYMASK_RIGHT |
                       OSL_KEYMASK_DOWN | OSL_KEYMASK_LEFT |
                       OSL_KEYMASK_R|OSL_KEYMASK_L, 40, FIX2INT(time));
   oslSetKeyAutorepeatInit(FIX2INT(time));
   return Qnil;
}

VALUE Keys_repeatInterval(VALUE self, VALUE time)
{
   oslSetKeyAutorepeatInterval(FIX2INT(time));
   return Qnil;
}

VALUE checkKeys(VALUE self)
{
   oslReadKeys();
   VALUE keys = rb_gv_get("$keys");

   string prefix[] = { "", "released_", "pressed_"};
   OSL_KEYLIST controller[] = { osl_pad.held, osl_pad.released,
                                osl_pad.pressed };

   for (int i = 0; i < 3; ++i)
   {
      if (controller[i].select)
         rb_hash_aset(keys, rb_str_new2((prefix[i] + "select").c_str()), Qtrue);
      else
         rb_hash_aset(keys, rb_str_new2((prefix[i] + "select").c_str()),
                      Qfalse);
      if (controller[i].start)
         rb_hash_aset(keys, rb_str_new2((prefix[i] + "start").c_str()), Qtrue);
      else
         rb_hash_aset(keys, rb_str_new2((prefix[i] + "start").c_str()),
                      Qfalse);

      if (controller[i].up)
         rb_hash_aset(keys, rb_str_new2((prefix[i] + "up").c_str()), Qtrue);
      else
         rb_hash_aset(keys, rb_str_new2((prefix[i] + "up").c_str()), Qfalse);
      if (controller[i].down)
         rb_hash_aset(keys, rb_str_new2((prefix[i] + "down").c_str()), Qtrue);
      else
         rb_hash_aset(keys, rb_str_new2((prefix[i] + "down").c_str()), Qfalse);
      if (controller[i].left)
         rb_hash_aset(keys, rb_str_new2((prefix[i] + "left").c_str()), Qtrue);
      else
         rb_hash_aset(keys, rb_str_new2((prefix[i] + "left").c_str()), Qfalse);
      if (controller[i].right)
         rb_hash_aset(keys, rb_str_new2((prefix[i] + "right").c_str()), Qtrue);
      else
         rb_hash_aset(keys, rb_str_new2((prefix[i] + "right").c_str()),
                      Qfalse);

      if (controller[i].L)
         rb_hash_aset(keys, rb_str_new2((prefix[i] + "L").c_str()), Qtrue);
      else
         rb_hash_aset(keys, rb_str_new2((prefix[i] + "L").c_str()), Qfalse);
      if (controller[i].R)
         rb_hash_aset(keys, rb_str_new2((prefix[i] + "R").c_str()), Qtrue);
      else
         rb_hash_aset(keys, rb_str_new2((prefix[i] + "R").c_str()), Qfalse);

      if (controller[i].circle)
         rb_hash_aset(keys, rb_str_new2((prefix[i] + "circle").c_str()), Qtrue);
      else
         rb_hash_aset(keys, rb_str_new2((prefix[i] + "circle").c_str()),
                      Qfalse);
      if (controller[i].triangle)
         rb_hash_aset(keys, rb_str_new2((prefix[i] + "triangle").c_str()),
                      Qtrue);
      else
         rb_hash_aset(keys, rb_str_new2((prefix[i] + "triangle").c_str()),
                      Qfalse);
      if (controller[i].cross)
         rb_hash_aset(keys, rb_str_new2((prefix[i] + "cross").c_str()), Qtrue);
      else
         rb_hash_aset(keys, rb_str_new2((prefix[i] + "cross").c_str()), Qfalse);

      if (controller[i].square)
         rb_hash_aset(keys, rb_str_new2((prefix[i] + "square").c_str()), Qtrue);
      else
         rb_hash_aset(keys, rb_str_new2((prefix[i] + "square").c_str()),
                      Qfalse);

      if (controller[i].hold)
         rb_hash_aset(keys, rb_str_new2((prefix[i] + "hold").c_str()), Qtrue);
      else
         rb_hash_aset(keys, rb_str_new2((prefix[i] + "hold").c_str()), Qfalse);
   }

   int analogX = osl_pad.analogX;
   int analogY = osl_pad.analogY;

   rb_hash_aset(keys, rb_str_new2("analogX"), INT2FIX(analogX));
   rb_hash_aset(keys, rb_str_new2("analogY"), INT2FIX(analogY));

   rb_gv_set("$keys", keys);

   return Qnil;
}

VALUE Cursor_updatePos(VALUE self)
{
   Cursor &ref = getRef<Cursor>(self);
   ref.updatePos();

   return Qnil;
}

VALUE Cursor_setSensibility(VALUE self, VALUE s)
{
   Cursor &ref = getRef<Cursor>(self);
   int sens = FIX2INT(s);

   ref.setSensibility(sens);
   return Qnil;
}

VALUE Cursor_sensibility(VALUE self)
{
   Cursor &ref = getRef<Cursor>(self);
   return INT2FIX(ref.getSensibility());
}

VALUE Joyau_gets(VALUE self)
{
   SceUtilityOskData data;
   SceUtilityOskParams params;

   unsigned short input[128]  = { 'E', 'n', 't', 'e', 'r', ' ',
                                  'y', 'o', 'u', 'r', ' ',
                                  't', 'e','x', 't', 0 };
   unsigned short output[128] = { 0 };

   memset(&data, 0, sizeof(data));
   data.lines = 1;
   data.unk_24 = 1;
   data.inputtype = PSP_UTILITY_OSK_INPUTTYPE_ALL;
   data.desc = input;
   data.intext = input;
   data.outtextlength = 128;
   data.outtextlimit = 128;
   data.outtext = output;

   memset(&params, 0, sizeof(params));
   params.base.size = sizeof(params);
   sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_LANGUAGE,
                               &params.base.language);
   sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_UNKNOWN,
                               &params.base.buttonSwap);
   params.base.graphicsThread = 17;
   params.base.accessThread = 19;
   params.base.fontThread = 18;
   params.base.soundThread = 16;
   params.datacount = 1;
   params.data = &data;

   sceUtilityOskInitStart(&params);

   bool done = false;
   while (!done)
   {
      oslStartDrawing();
      oslDrawFillRect(0, 0, 480, 272, RGBA(0, 0, 0, 255));
      oslEndDrawing();
      
      switch(sceUtilityOskGetStatus())
      {
         case PSP_UTILITY_DIALOG_INIT:
            break;
         case PSP_UTILITY_DIALOG_VISIBLE:
            sceUtilityOskUpdate(1);
            break;
         case PSP_UTILITY_DIALOG_QUIT:
            sceUtilityOskShutdownStart();
            break;
         case PSP_UTILITY_DIALOG_FINISHED:
            break;
         case PSP_UTILITY_DIALOG_NONE:
            done = true;
         default :
            break;
      }

      oslEndDrawing();
      oslEndFrame();
      oslSyncFrame();
   }

   char *ret = new char[128];
   for (int i = 0; i < 128; ++i)
      ret[i] = (char)data.outtext[i];

   return rb_str_new2(ret);
}

void defineKeys()
{
   VALUE keys = rb_hash_new();
   rb_gv_set("$keys", keys);

   defFunc("repeatInit", Keys_repeatInit, 1);
   defFunc("repeatInterval", Keys_repeatInterval, 1);
   defFunc("readKeys", checkKeys, 0);

   defFunc("gets", Joyau_gets, 0);

   VALUE cCursor = defClass<Cursor>("Cursor", "Sprite");
   defMethod(cCursor, "updatePos", Cursor_updatePos, 0);
   defMethod(cCursor, "setSensibility", Cursor_setSensibility, 1);
   defMethod(cCursor, "sensibility", Cursor_sensibility, 0);
}
