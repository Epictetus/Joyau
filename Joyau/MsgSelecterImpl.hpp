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

#ifndef JOYAU_MSG_SELECTER2
#define JOYAU_MSG_SELECTER2

template<typename Father>
void MsgSelecter<Father>::forFocused(content_t &obj)
{
   applyConf(obj, focusConf);
}

template<typename Father>
void MsgSelecter<Father>::atLoop(content_t &obj, size_t pos)
{
   Father::atLoop(obj, pos);
   applyConf(obj, conf);
}

template<typename Father>
void MsgSelecter<Father>::applyConf(content_t &obj, MsgConfig &arg)
{
   obj.setBgColor(arg.bgColor);
   obj.setTitleColor(arg.titleColor);
   obj.setTextColor(arg.textColor);
   obj.setBorderColor(arg.borderColor);

   if (arg.image != NULL)
      obj.setImage(arg.image);
   if (arg.bg != NULL)
      obj.setBackground(arg.bg);

   //obj.setTextFont(arg.textFont);
   //obj.setTitleFont(arg.titleFont);

   obj.setTitlePos(arg.titleX, arg.titleY);
}

template<typename T> VALUE MsgSelecter_setConf(VALUE self, VALUE val)
{
   T &ref = getRef<T>(self);
   MsgConfig &arg = getRef<MsgConfig>(self);
   
   ref.setConf(arg);
   return val;
}

template<typename T> VALUE MsgSelecter_setFocusConf(VALUE self, VALUE val)
{
   T &ref = getRef<T>(self);
   MsgConfig &arg = getRef<MsgConfig>(self);
   
   ref.setFocusConf(arg);
   return Qnil;
}

template<typename T> VALUE MsgSelecter_conf(VALUE self)
{
   T &ref = getRef<T>(self);
   MsgConfig &conf = ref.getConf();

   return conf.toRuby();
}

template<typename T> VALUE MsgSelecter_focusConf(VALUE self)
{
   T &ref = getRef<T>(self);
   MsgConfig &conf = ref.getFocusConf();

   return conf.toRuby();
}

template<typename T> VALUE defineMsgSelecterKlass(const char *name)
{
   VALUE klass = defineGenericSelecter<T>(name);

   defTplMethod(klass, "conf", MsgSelecter_conf<T>);
   defTplMethod(klass, "focusConf", MsgSelecter_focusConf<T>);
   defTplMethod1(klass, "conf=", MsgSelecter_setConf<T>);
   defTplMethod1(klass, "focusConf=", MsgSelecter_setFocusConf<T>);

   return klass;
}

#endif
