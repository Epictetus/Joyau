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

#include "MsgSelecter.hpp"
#include "Sprite.hpp"

template<>
VALUE wrap<HorizontalMsgSelecter>(int argc, VALUE *argv, VALUE info)
{
   HorizontalMsgSelecter *ptr = new HorizontalMsgSelecter;
   ptr->setClass("HorizontalMsgSelecter");
   VALUE tdata = Data_Wrap_Struct(info, 0, 
				  wrapped_free<HorizontalMsgSelecter>, ptr);
   return tdata;
}

template<>
VALUE wrap<VerticalMsgSelecter>(int argc, VALUE *argv, VALUE info)
{
   VerticalMsgSelecter *ptr = new VerticalMsgSelecter;
   ptr->setClass("VerticalMsgSelecter");
   VALUE tdata = Data_Wrap_Struct(info, 0, 
				  wrapped_free<VerticalMsgSelecter>, ptr);
   return tdata;
}

VALUE MsgConfig_image(VALUE self)
{
   MsgConfig &ref = getRef<MsgConfig>(self);
   return ref.image->toRuby();
}

VALUE MsgConfig_setImage(VALUE self, VALUE val)
{
   MsgConfig &ref = getRef<MsgConfig>(self);
   ref.image = getPtr<Sprite>(val);

   return val;
}

VALUE MsgConfig_bg(VALUE self)
{
   MsgConfig &ref = getRef<MsgConfig>(self);
   return ref.bg->toRuby();
}

VALUE MsgConfig_setBg(VALUE self, VALUE val)
{
   MsgConfig &ref = getRef<MsgConfig>(self);
   ref.bg = getPtr<Sprite>(val);

   return val;
}

VALUE MsgConfig_textColor(VALUE self)
{
   MsgConfig &ref = getRef<MsgConfig>(self);
   return col2hash(ref.textColor);
}

VALUE MsgConfig_setTextColor(VALUE self, VALUE val)
{
   MsgConfig &ref = getRef<MsgConfig>(self);
   ref.textColor = hash2col(val);

   return val;
}

VALUE MsgConfig_titleColor(VALUE self)
{
   MsgConfig &ref = getRef<MsgConfig>(self);
   return col2hash(ref.titleColor);
}

VALUE MsgConfig_setTitleColor(VALUE self, VALUE val)
{
   MsgConfig &ref = getRef<MsgConfig>(self);
   ref.titleColor = hash2col(val);

   return val;
}

VALUE MsgConfig_bgColor(VALUE self)
{
   MsgConfig &ref = getRef<MsgConfig>(self);
   return col2hash(ref.bgColor);
}

VALUE MsgConfig_setBgColor(VALUE self, VALUE val)
{
   MsgConfig &ref = getRef<MsgConfig>(self);
   ref.bgColor = hash2col(val);

   return val;
}

VALUE MsgConfig_borderColor(VALUE self)
{
   MsgConfig &ref = getRef<MsgConfig>(self);
   return col2hash(ref.borderColor);
}

VALUE MsgConfig_setBorderColor(VALUE self, VALUE val)
{
   MsgConfig &ref = getRef<MsgConfig>(self);
   ref.borderColor = hash2col(val);

   return val;
}

VALUE MsgConfig_textFont(VALUE self)
{
   MsgConfig &ref = getRef<MsgConfig>(self);
   return rb_str_new2(ref.textFont.c_str());
}

VALUE MsgConfig_setTextFont(VALUE self, VALUE val)
{
   MsgConfig &ref = getRef<MsgConfig>(self);
   ref.textFont = StringValuePtr(val);

   return val;
}

VALUE MsgConfig_titleFont(VALUE self)
{
   MsgConfig &ref = getRef<MsgConfig>(self);
   return rb_str_new2(ref.titleFont.c_str());
}

VALUE MsgConfig_setTitleFont(VALUE self, VALUE val)
{
   MsgConfig &ref = getRef<MsgConfig>(self);
   ref.titleFont = StringValuePtr(val);

   return val;
}

VALUE MsgConfig_titlePos(VALUE self)
{
   MsgConfig &ref = getRef<MsgConfig>(self);
   Point p(ref.titleX, ref.titleY);

   return createObject(getClass("Point"), p);
}

VALUE MsgConfig_setTitlePos(VALUE self, VALUE val)
{
   MsgConfig &ref = getRef<MsgConfig>(self);
   Point &p = getRef<Point>(val);

   ref.titleX = p.x;
   ref.titleY = p.y;
   
   return val;
}

void defineMsgConfig()
{
   VALUE cMsgConfig = defClass<MsgConfig>("MsgConfig");
   defMethod(cMsgConfig, "image", MsgConfig_image, 0);
   defMethod(cMsgConfig, "image=", MsgConfig_setImage, 1);

   defMethod(cMsgConfig, "bg", MsgConfig_bg, 0);
   defMethod(cMsgConfig, "bg=", MsgConfig_setBg, 1);

   defMethod(cMsgConfig, "titleColor", MsgConfig_titleColor, 0);
   defMethod(cMsgConfig, "titleColor=", MsgConfig_setTitleColor, 1);

   defMethod(cMsgConfig, "textColor", MsgConfig_textColor, 0);
   defMethod(cMsgConfig, "textColor=", MsgConfig_setTextColor, 1);

   defMethod(cMsgConfig, "bgColor", MsgConfig_bgColor, 0);
   defMethod(cMsgConfig, "bgColor=", MsgConfig_setBgColor, 1);

   defMethod(cMsgConfig, "borderColor", MsgConfig_borderColor, 0);
   defMethod(cMsgConfig, "borderColor=", MsgConfig_setBorderColor, 1);

   defMethod(cMsgConfig, "textFont", MsgConfig_textFont, 0);
   defMethod(cMsgConfig, "textFont=", MsgConfig_setTextFont, 1);

   defMethod(cMsgConfig, "titleFont", MsgConfig_titleFont, 0);
   defMethod(cMsgConfig, "titleFont=", MsgConfig_setTitleFont, 1);
}

void defineMsgSelecter()
{
   defineMsgConfig();
   defineMsgSelecterKlass<VerticalMsgSelecter>("VerticalMsgSelecter");
   defineMsgSelecterKlass<HorizontalMsgSelecter>("HorizontalMsgSelecter");
}
