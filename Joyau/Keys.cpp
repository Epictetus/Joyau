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

using namespace std;

Pad::Pad()
{
   sceCtrlSetSamplingCycle(0);
   sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
   sceCtrlReadBufferPositive(&pad, 1);
}

void Pad::update()
{
   oldPad = pad.Buttons;
   sceCtrlReadBufferPositive(&pad, 1);

   // We'll use an offset of 128 pixels.
   // This means the minimum value is -128, while the maximum is 128.
   stickX = pad.Lx - 128;
   stickY = pad.Ly - 128;
}

bool Pad::pressed(const std::string &key) const
{
   int k = str2key(key);
   return !(oldPad & k) && pad.Buttons & k;
}

bool Pad::released(const std::string &key) const
{
   int k = str2key(key);
   return oldPad & k && !(pad.Buttons & k);
}

bool Pad::held(const string &key) const
{
   return pad.Buttons & str2key(key);
}

bool Pad::pressed(int key) const
{
   return !(oldPad & key) && pad.Buttons & key;
}

bool Pad::released(int key) const
{
   return oldPad & key && !(pad.Buttons & key);
}

bool Pad::held(int key) const
{
   return pad.Buttons & key;
}

int Pad::str2key(const string &key) const
{
   if (key == "select")
      return PSP_CTRL_SELECT;
   else if (key == "start")
      return PSP_CTRL_START;
   else if (key == "up")
      return PSP_CTRL_UP;
   else if (key == "down")
      return PSP_CTRL_DOWN;
   else if (key == "right")
      return PSP_CTRL_RIGHT;
   else if (key == "left")
      return PSP_CTRL_LEFT;
   else if (key == "L")
      return PSP_CTRL_LTRIGGER;
   else if (key == "R")
      return PSP_CTRL_RTRIGGER;
   else if (key == "cross")
      return PSP_CTRL_CROSS;
   else if (key == "triangle")
      return PSP_CTRL_TRIANGLE;
   else if (key == "circle")
      return PSP_CTRL_CIRCLE;
   else if (key == "square")
      return PSP_CTRL_SQUARE;
   else if (key == "hold")
      return PSP_CTRL_HOLD;

   return -1;
}

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
   Pad &pad = Pad::getInstance();
   // since the user can directly check the Pad, it's no longer
   // needed to fill the $keys variable.
   pad.update();
   if (sensibility != 0)
   {
      int analogX = pad.getStickX();
      int analogY = pad.getStickY();

      move(analogX / sensibility, analogY / sensibility);
      
      setX(_x > rect.x + rect.w ? rect.x + rect.w : _x);
      setX(_x < rect.x ? rect.x : _x);

      setY(_y > rect.y + rect.h ? rect.y + rect.h : _y);
      setY(_y < rect.y ? rect.y : _y);
   }
}

VALUE checkKeys(VALUE self)
{
   VALUE keys = rb_gv_get("$keys");

   string keys_str[] = { "select", "start", "up", "down", "left",
			 "right", "L", "R", "cross", "triangle", "square",
			 "hold" };
   
   Pad &pad = Pad::getInstance();
   pad.update();
   for (int i = 0; i < 12; ++i)
   {
      if (pad.held(keys_str[i]))
	 rb_hash_aset(keys, rb_str_new2(keys_str[i].c_str()), Qtrue);
      else
	 rb_hash_aset(keys, rb_str_new2(keys_str[i].c_str()), Qfalse);

      if (pad.pressed(keys_str[i]))
	 rb_hash_aset(keys, rb_str_new2(("pressed_" + keys_str[i]).c_str()),
		      Qtrue);
      else
	 rb_hash_aset(keys, rb_str_new2(("pressed_" + keys_str[i]).c_str()), 
		      Qfalse);

      if (pad.released(keys_str[i]))
	 rb_hash_aset(keys, rb_str_new2(("released_" + keys_str[i]).c_str()),
		      Qtrue);
      else
	 rb_hash_aset(keys, rb_str_new2(("released_" + keys_str[i]).c_str()), 
		      Qfalse);
   }
   
   int analogX = pad.getStickX();
   int analogY = pad.getStickY();

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

VALUE Cursor_setRect(VALUE self, VALUE rect)
{
   Cursor &ref = getRef<Cursor>(self);
   Rect &arg = getRef<Rect>(rect);
   
   ref.setRect(arg);
   return Qnil;
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

VALUE Pad_update(VALUE self)
{
   Pad::getInstance().update();
   return Qnil;
}

VALUE Pad_held(VALUE self, VALUE key)
{
   Pad &pad = Pad::getInstance();
   if (TYPE(key) == T_STRING)
      return pad.held(StringValuePtr(key)) ? Qtrue : Qfalse;
   return pad.held(FIX2INT(key)) ? Qtrue : Qfalse;
}

VALUE Pad_released(VALUE self, VALUE key)
{
   Pad &pad = Pad::getInstance();
   if (TYPE(key) == T_STRING)
      return pad.released(StringValuePtr(key)) ? Qtrue : Qfalse;
    return pad.released(FIX2INT(key)) ? Qtrue : Qfalse;
}

VALUE Pad_pressed(VALUE self, VALUE key)
{
   Pad &pad = Pad::getInstance();
   if (TYPE(key) == T_STRING)
      return pad.pressed(StringValuePtr(key)) ? Qtrue : Qfalse;
   return pad.pressed(FIX2INT(key)) ? Qtrue : Qfalse;
}

VALUE Pad_stickX(VALUE self)
{
   return INT2FIX(Pad::getInstance().getStickX());
}

VALUE Pad_stickY(VALUE self)
{
   return INT2FIX(Pad::getInstance().getStickX());
}

void defineKeys()
{
   VALUE keys = rb_hash_new();
   rb_gv_set("$keys", keys);

   defFunc("readKeys", checkKeys, 0);
   defFunc("gets", Joyau_gets, 0);

   VALUE mPad = defModule("Pad");
   defModFunc(mPad, "update", Pad_update, 0);
   defModFunc(mPad, "held?", Pad_held, 1);
   defModFunc(mPad, "pressed?", Pad_pressed, 1);
   defModFunc(mPad, "released?", Pad_released, 1);
   defModFunc(mPad, "stickX", Pad_stickX, 0);
   defModFunc(mPad, "stickY", Pad_stickY, 0);

   defConst(mPad, "SELECT", INT2FIX(PSP_CTRL_SELECT));
   defConst(mPad, "START", INT2FIX(PSP_CTRL_START));

   defConst(mPad, "L", INT2FIX(PSP_CTRL_LTRIGGER));
   defConst(mPad, "R", INT2FIX(PSP_CTRL_RTRIGGER));

   defConst(mPad, "CROSS", INT2FIX(PSP_CTRL_CROSS));
   defConst(mPad, "SQUARE", INT2FIX(PSP_CTRL_SQUARE));
   defConst(mPad, "TRIANGLE", INT2FIX(PSP_CTRL_TRIANGLE));
   defConst(mPad, "CIRCLE", INT2FIX(PSP_CTRL_CIRCLE));

   defConst(mPad, "UP", INT2FIX(PSP_CTRL_UP));
   defConst(mPad, "DOWN", INT2FIX(PSP_CTRL_DOWN));
   defConst(mPad, "LEFT", INT2FIX(PSP_CTRL_LEFT));
   defConst(mPad, "RIGHT", INT2FIX(PSP_CTRL_RIGHT));

   defConst(mPad, "HOLD", INT2FIX(PSP_CTRL_HOLD));

   VALUE cCursor = defClass<Cursor>("Cursor", "Sprite");
   defMethod(cCursor, "updatePos", Cursor_updatePos, 0);

   defMethod(cCursor, "setSensibility", Cursor_setSensibility, 1);
   defMethod(cCursor, "sensibility", Cursor_sensibility, 0);

   defMethod(cCursor, "rect=", Cursor_setRect, 1);

   defAlias(cCursor, "setSensibility", "sensibility=");
}
