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

#ifndef JOYAU_MSG_SELECTER
#define JOYAU_MSG_SELECTER

#include "GenericSelecter.hpp"
#include "MessageBox.hpp"

struct MsgConfig: public RubyObject
{
   MsgConfig():
      image(NULL),
      bg(NULL),
      textColor(RGBA(255, 255, 255, 255)),
      titleColor(RGBA(255, 255, 255, 255)),
      bgColor(RGBA(0, 0, 0, 255)),
      borderColor(RGBA(0, 0, 0, 255)),
      titleFont(""),
      textFont(""),
      titleX(0),
      titleY(0)
   { setClass("MsgConfig"); }

   MsgConfig& copy(MsgConfig& obj) 
   {
      if (&obj == this)
	 return *this;

      image = obj.image;
      bg = obj.bg;
      
      textColor = obj.textColor;
      titleColor = obj.titleColor;
      bgColor = obj.bgColor;
      borderColor = obj.borderColor;

      //titleFont = obj.titleFont;
      //textFont = obj.textFont;
      
      titleX = obj.titleX;
      titleY = obj.titleY;

      return *this;
   }

   Sprite *image;
   Sprite *bg;

   std::string text;
   std::string title;

   OSL_COLOR textColor;
   OSL_COLOR titleColor;
   OSL_COLOR bgColor;
   OSL_COLOR borderColor;

   std::string titleFont;
   std::string textFont;

   int titleX, titleY;
};

template<typename Father>
class MsgSelecter: public Father
{
public:
   typedef typename Father::content_t content_t;

   void forFocused(content_t &obj);
   void atLoop(content_t &obj, size_t pos);

   void setConf(MsgConfig &val) { conf.copy(val); }
   void setFocusConf(MsgConfig &val) { focusConf.copy(val); }

   MsgConfig& getConf() { return conf; }
   MsgConfig& getFocusConf() { return focusConf; }
protected:
   MsgConfig conf;
   MsgConfig focusConf;

   void applyConf(content_t &obj, MsgConfig &arg);
};

template<typename T> VALUE MsgSelecter_setConf(VALUE self, VALUE val);
template<typename T> VALUE MsgSelecter_setFocusConf(VALUE self, VALUE val);

template<typename T> VALUE MsgSelecter_conf(VALUE self);
template<typename T> VALUE MsgSelecter_focusConf(VALUE self);

template<typename T> VALUE defineMsgSelecterKlass(const char *name);

#include "MsgSelecter.tpp"

typedef MsgSelecter< HorizontalSelecter<Message> > HorizontalMsgSelecter;
typedef MsgSelecter< VerticalSelecter<Message> > VerticalMsgSelecter;

VALUE MsgConfig_image(VALUE self);
VALUE MsgConfig_setImage(VALUE self, VALUE val);

VALUE MsgConfig_bg(VALUE self);
VALUE MsgConfig_setBg(VALUE self, VALUE val);

VALUE MsgConfig_textColor(VALUE self);
VALUE MsgConfig_setTextColor(VALUE self, VALUE val);

VALUE MsgConfig_titleColor(VALUE self);
VALUE MsgConfig_setTitleColor(VALUE self, VALUE val);

VALUE MsgConfig_bgColor(VALUE self);
VALUE MsgConfig_setBgColor(VALUE self, VALUE val);

VALUE MsgConfig_borderColor(VALUE self);
VALUE MsgConfig_setBorderColor(VALUE self, VALUE val);

VALUE MsgConfig_textFont(VALUE self);
VALUE MsgConfig_setTextFont(VALUE self, VALUE val);

VALUE MsgConfig_titleFont(VALUE self);
VALUE MsgConfig_setTitleFont(VALUE self, VALUE val);

VALUE MsgConfig_titlePos(VALUE self);
VALUE MsgConfig_setTitlePos(VALUE self, VALUE val);

void defineMsgConfig();

void defineMsgSelecter();

#endif
