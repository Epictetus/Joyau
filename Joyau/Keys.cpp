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

#include "Keys.hpp"

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

bool Pad::held(const std::string &key) const
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

int Pad::str2key(const std::string &key) const
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

/*
  Document-class: Joyau::Cursor

  This is a kind of Sprite which moves according to the analogic stick's move.
  If its "sensibility" is greater, it'll move slower.
*/

template<>
/*
  call-seq: new
            new(filename [, sensibility])

  Creates a new Cursor.
*/
VALUE wrap<Cursor>(int argc, VALUE *argv, VALUE info)
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

/*
  This is an old, and deprecated function which checks the pad.
  Do not use this, use Pad's methods directly.
*/
VALUE checkKeys(VALUE self)
{
   VALUE keys = rb_gv_get("$keys");

   std::string keys_str[] = { "select", "start", "up", "down", "left",
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

/*
  Moves the cursor automatically.
*/
VALUE Cursor_updatePos(VALUE self)
{
   Cursor &ref = getRef<Cursor>(self);
   ref.updatePos();

   return Qnil;
}

/*
  call-seq: setSensibility(val)

  Changes the cursor sensibility.
*/
VALUE Cursor_setSensibility(VALUE self, VALUE s)
{
   Cursor &ref = getRef<Cursor>(self);
   int sens = FIX2INT(s);

   ref.setSensibility(sens);
   return Qnil;
}

/*
  Returns the cursor's sensibility.
*/
VALUE Cursor_sensibility(VALUE self)
{
   Cursor &ref = getRef<Cursor>(self);
   return INT2FIX(ref.getSensibility());
}

/*
  call-seq: setRect(rect)

  Sets the rect in which the cursor can move.
*/
VALUE Cursor_setRect(VALUE self, VALUE rect)
{
   Cursor &ref = getRef<Cursor>(self);
   Rect &arg = getRef<Rect>(rect);
   
   ref.setRect(arg);
   return Qnil;
}

/*
  Returns the rect in which the cursor can move.
*/
VALUE Cursor_rect(VALUE self)
{
   Cursor &ref = getRef<Cursor>(self);
   Rect ret = ref.getRect();

   return createObject(getClass("Rect"), ret);
}

/*
  Returns a string typed through the PSP built-in keyboard.
*/
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

/*
  call-seq: run(input_type, language, text, desc, output_size) { ... }

  Shows the OSK (Sony's default keyboard), and returns the input text.
  Example:
    Joyau::OSK.run(Joyau::OSK::ALL, Joyau::OSK::DEFAULT,
                   "Anonymous", "Enter your nickame", 16) do
      Joyau.clearScreen # this is suffisant to avoid glitches,
                        # Although you might copy the old screen buffer,
			# and draw it here.
 */
VALUE Osk_run(VALUE self, VALUE type, VALUE lang, VALUE rb_input, 
	      VALUE rb_desc, VALUE output_size)
{
   SceUtilityOskData data;
   SceUtilityOskParams params;

   char *char_input = StringValuePtr(rb_input);
   int char_size = strlen(char_input);
   
   unsigned short *input = new unsigned short[char_size + 1];
   for (int i = 0; i < char_size + 1; ++i)
      input[i] = char_input[i];
   
   char *char_desc = StringValuePtr(rb_desc);
   char_size = strlen(char_desc);
   
   unsigned short *desc = new unsigned short[char_size + 1];
   for (int i = 0; i < char_size + 1; ++i)
      desc[i] = char_desc[i];

   unsigned short *output = new unsigned short[FIX2INT(output_size)];

   memset(&data, 0, sizeof(data));
   data.lines = 1;
   data.unk_24 = 1;
   data.inputtype = INT2FIX(type);
   data.desc = desc;
   data.intext = input;
   data.outtextlength = FIX2INT(output_size);
   data.outtextlimit = FIX2INT(output_size);
   data.outtext = output;

   memset(&params, 0, sizeof(params));
   params.base.size = sizeof(params);
   params.base.language = FIX2INT(lang);
   sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_UNKNOWN,
                               &params.base.buttonSwap);
   params.base.soundThread = 16;
   params.base.graphicsThread = 17;
   params.base.accessThread = 19;
   params.base.fontThread = 18;
   params.datacount = 1;
   params.data = &data;

   sceUtilityOskInitStart(&params);

   bool done = false;
   while (!done)
   {
      oslStartDrawing();
      rb_yield(Qnil);
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

   char *ret = new char[FIX2INT(output_size)];
   for (int i = 0; i < FIX2INT(output_size); ++i)
      ret[i] = (char)data.outtext[i];

   delete[] input;
   delete[] desc;
   delete[] output;

   return rb_str_new2(ret);
}

/*
  Document-class: Joyau::Pad
  
  Module used in order to check what keys are pressed or released.
*/

/*
  Updates the pad.
*/
VALUE Pad_update(VALUE self)
{
   Pad::getInstance().update();
   return Qnil;
}

/*
  call-seq: held? key

  Returns whether a key is held.
*/
VALUE Pad_held(VALUE self, VALUE key)
{
   Pad &pad = Pad::getInstance();
   if (TYPE(key) == T_STRING)
      return pad.held(StringValuePtr(key)) ? Qtrue : Qfalse;
   return pad.held(FIX2INT(key)) ? Qtrue : Qfalse;
}

/*
  call-seq: released? key

  Returns whether a key has been released right now.
*/
VALUE Pad_released(VALUE self, VALUE key)
{
   Pad &pad = Pad::getInstance();
   if (TYPE(key) == T_STRING)
      return pad.released(StringValuePtr(key)) ? Qtrue : Qfalse;
    return pad.released(FIX2INT(key)) ? Qtrue : Qfalse;
}

/*
  call-seq: pressed? key

  Returns whether a key has been pressed right now.
*/
VALUE Pad_pressed(VALUE self, VALUE key)
{
   Pad &pad = Pad::getInstance();
   if (TYPE(key) == T_STRING)
      return pad.pressed(StringValuePtr(key)) ? Qtrue : Qfalse;
   return pad.pressed(FIX2INT(key)) ? Qtrue : Qfalse;
}

/*
  Returns the Pad's abscissa. The minimum is -128, and the maximum is 128.
*/
VALUE Pad_stickX(VALUE self)
{
   return INT2FIX(Pad::getInstance().getStickX());
}

/*
  Returns the Pad's ordinate. The minimum is -128, and the maximum is 128.
*/
VALUE Pad_stickY(VALUE self)
{
   return INT2FIX(Pad::getInstance().getStickY());
}

void defineKeys()
{
   VALUE keys = rb_hash_new();
   rb_gv_set("$keys", keys);

   VALUE joyau = JOYAU_MOD;
   defModFunc(joyau, "readKeys", checkKeys, 0);
   defModFunc(joyau, "gets", Joyau_gets, 0);

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

   /*
     Document-class: Joyau::OSK

     There is a built-in keyboard in your firmware, which can be used by your
     program easily through this class. Notice Joyau.gets is a simpliefied
     version of this module, which can handle more advanced settings.

     You can, by using its constants, specify what has to be input.
     you can indicated the used language:

     1. DEFAULT
     2. JAPANESE
     3. ENGLISH
     4. FRENCH
     5. SPANISH
     6. GERMAN
     7. ITALIAN
     8. DUTCH
     9. PORTUGESE
     10. RUSSIAN
     11. KOREAN

     As well as the caracters to input:
     
     1. ALL
     2. LATIN_SYMBOL
     3. LATIN_LOWERCASE
     4. LATIN_UPPERCASE
     5. LATIN_DIGIT
     6. JAPANESE_DIGIT
     7. JAPANESE_SYMBOL
     8. JAPANESE_LOWERCASE
     9. JAPANESE_UPPERCASE
     10. JAPANESE_HIRAGANA
     11. JAPANASE_HALF_KATAKANA
     12. JAPANESE_KATAKANA
     13. JAPANESE_LOWERCASE
     14. RUSSIAN_LOWERCASE
     15. RUSSIAN_UPPERCASE
     16. IT_KOREAN
     17. URL

     You can also set the default text and the description, and give a block
     telling what to draw under the keyboard (if nothing were drawn, there'd
     be graphical glitches).
   */
   VALUE mOSK = defModule("OSK");
   defModFunc(mOSK, "run", Osk_run, 5);
   
   defConst(mOSK, "DEFAULT", INT2FIX(PSP_UTILITY_OSK_LANGUAGE_DEFAULT));
   defConst(mOSK, "JAPANESE", INT2FIX(PSP_UTILITY_OSK_LANGUAGE_JAPANESE));
   defConst(mOSK, "ENGLISH", INT2FIX(PSP_UTILITY_OSK_LANGUAGE_ENGLISH));
   defConst(mOSK, "FRENCH", INT2FIX(PSP_UTILITY_OSK_LANGUAGE_SPANISH));
   defConst(mOSK, "SPANISH", INT2FIX(PSP_UTILITY_OSK_LANGUAGE_SPANISH));
   defConst(mOSK, "GERMAN", INT2FIX(PSP_UTILITY_OSK_LANGUAGE_GERMAN));
   defConst(mOSK, "ITALIAN", INT2FIX(PSP_UTILITY_OSK_LANGUAGE_ITALIAN));
   defConst(mOSK, "PORTUGESE", INT2FIX(PSP_UTILITY_OSK_LANGUAGE_PORTUGESE));
   defConst(mOSK, "RUSSIAN", INT2FIX(PSP_UTILITY_OSK_LANGUAGE_RUSSIAN));
   defConst(mOSK, "KOREAN", INT2FIX(PSP_UTILITY_OSK_LANGUAGE_KOREAN));
   
   defConst(mOSK, "ALL", INT2FIX(PSP_UTILITY_OSK_INPUTTYPE_ALL));
   defConst(mOSK, "LATIN_DIGIT", 
	    INT2FIX(PSP_UTILITY_OSK_INPUTTYPE_LATIN_DIGIT));
   defConst(mOSK, "LATIN_SYMBOL", 
	    INT2FIX(PSP_UTILITY_OSK_INPUTTYPE_LATIN_SYMBOL));
   defConst(mOSK, "LATIN_LOWERCASE", 
	    INT2FIX(PSP_UTILITY_OSK_INPUTTYPE_LATIN_LOWERCASE));
   defConst(mOSK, "LATIN_UPPERCASE", 
	    INT2FIX(PSP_UTILITY_OSK_INPUTTYPE_LATIN_UPPERCASE));
   defConst(mOSK, "JAPANESE_DIGIT", 
	    INT2FIX(PSP_UTILITY_OSK_INPUTTYPE_JAPANESE_DIGIT));
   defConst(mOSK, "JAPANESE_SYMBOL", 
	    INT2FIX(PSP_UTILITY_OSK_INPUTTYPE_JAPANESE_SYMBOL));
   defConst(mOSK, "JAPANESE_LOWERCASE", 
	    INT2FIX(PSP_UTILITY_OSK_INPUTTYPE_JAPANESE_LOWERCASE));
   defConst(mOSK, "JAPANESE_UPPERCASE", 
	    INT2FIX(PSP_UTILITY_OSK_INPUTTYPE_JAPANESE_UPPERCASE));
   defConst(mOSK, "JAPANESE_HIRAGANA", 
	    INT2FIX(PSP_UTILITY_OSK_INPUTTYPE_JAPANESE_HIRAGANA));
   defConst(mOSK, "JAPANASE_HALF_KATAKANA", 
	    INT2FIX(PSP_UTILITY_OSK_INPUTTYPE_JAPANESE_HALF_KATAKANA));
   defConst(mOSK, "JAPANESE_KATAKANA", 
	    INT2FIX(PSP_UTILITY_OSK_INPUTTYPE_JAPANESE_KATAKANA));
   defConst(mOSK, "RUSSIAN_LOWERCASE", 
	    INT2FIX(PSP_UTILITY_OSK_INPUTTYPE_JAPANESE_LOWERCASE));
   defConst(mOSK, "RUSSIAN_UPPERCASE", 
	    INT2FIX(PSP_UTILITY_OSK_INPUTTYPE_RUSSIAN_UPPERCASE));
   defConst(mOSK, "IT_KOREAN", INT2FIX(PSP_UTILITY_OSK_INPUTTYPE_KOREAN));
   defConst(mOSK, "URL", INT2FIX(PSP_UTILITY_OSK_INPUTTYPE_URL));

   VALUE cCursor = defClass<Cursor>("Cursor", "Sprite");
   defMethod(cCursor, "updatePos", Cursor_updatePos, 0);

   defMethod(cCursor, "setSensibility", Cursor_setSensibility, 1);
   defMethod(cCursor, "sensibility", Cursor_sensibility, 0);

   defMethod(cCursor, "rect=", Cursor_setRect, 1);
   defMethod(cCursor, "rect", Cursor_rect, 0);

   defAlias(cCursor, "setSensibility", "sensibility=");
   defAlias(cCursor, "updatePos", "update_pos");
}
