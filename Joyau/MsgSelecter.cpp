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

/*
  Document-class: Joyau::HorizontalMsgSelecter
  
  This drawable allows to select between one or more messages, disposed
  horizontally.
*/

template<>
/*
  call-seq: new()

  Creates a new message selecter. 
*/
VALUE wrap<HorizontalMsgSelecter>(int argc, VALUE *argv, VALUE info)
{
   HorizontalMsgSelecter *ptr = new HorizontalMsgSelecter;
   ptr->setClass("HorizontalMsgSelecter");
   VALUE tdata = Data_Wrap_Struct(info, 0, 
				  wrapped_free<HorizontalMsgSelecter>, ptr);
   return tdata;
}

template<>
/*
  call-seq: new()

  Creates a new message selecter. 
*/
VALUE wrap<VerticalMsgSelecter>(int argc, VALUE *argv, VALUE info)
{
   VerticalMsgSelecter *ptr = new VerticalMsgSelecter;
   ptr->setClass("VerticalMsgSelecter");
   VALUE tdata = Data_Wrap_Struct(info, 0, 
				  wrapped_free<VerticalMsgSelecter>, ptr);
   return tdata;
}

/*
  Document-class: Joyau::MsgConfig

  MsgConfig is used with MsgSelecter, which use them in order
 */

/*
  Returns the image shown beside the text.
*/
VALUE MsgConfig_image(VALUE self)
{
   MsgConfig &ref = getRef<MsgConfig>(self);
   if (ref.image)
      return ref.image->toRuby();
   return Qnil;
}

/*
  call-seq: image=(sprite)

  Sets the sprite drawn beside the text.
*/
VALUE MsgConfig_setImage(VALUE self, VALUE val)
{
   MsgConfig &ref = getRef<MsgConfig>(self);
   ref.image = getPtr<Sprite>(val);

   return val;
}

/*
  Returns the message's background.
*/
VALUE MsgConfig_bg(VALUE self)
{
   MsgConfig &ref = getRef<MsgConfig>(self);
   if (ref.bg)
      return ref.bg->toRuby();
   return Qnil;
}

/*
  call-seq: bg=(val)

  Sets the background's picture.
*/
VALUE MsgConfig_setBg(VALUE self, VALUE val)
{
   MsgConfig &ref = getRef<MsgConfig>(self);
   ref.bg = getPtr<Sprite>(val);

   return val;
}

/*
  Returns the text's color.
*/
VALUE MsgConfig_textColor(VALUE self)
{
   MsgConfig &ref = getRef<MsgConfig>(self);
   return col2hash(ref.textColor);
}

/*
  call-seq: text=(val)

  Sets the text's color.
*/
VALUE MsgConfig_setTextColor(VALUE self, VALUE val)
{
   MsgConfig &ref = getRef<MsgConfig>(self);
   ref.textColor = hash2col(val);

   return val;
}

/*
  Returns the title's color.
*/
VALUE MsgConfig_titleColor(VALUE self)
{
   MsgConfig &ref = getRef<MsgConfig>(self);
   return col2hash(ref.titleColor);
}

/*
  call-seq: titleColor=(val)

  Sets the title's color.
*/
VALUE MsgConfig_setTitleColor(VALUE self, VALUE val)
{
   MsgConfig &ref = getRef<MsgConfig>(self);
   ref.titleColor = hash2col(val);

   return val;
}

/*
  Returns the background's color.
*/
VALUE MsgConfig_bgColor(VALUE self)
{
   MsgConfig &ref = getRef<MsgConfig>(self);
   return col2hash(ref.bgColor);
}

/*
  call-seq: bgColor=(val)

  Sets the background's color.
*/
VALUE MsgConfig_setBgColor(VALUE self, VALUE val)
{
   MsgConfig &ref = getRef<MsgConfig>(self);
   ref.bgColor = hash2col(val);

   return val;
}

/*
  Returns the border's color.
*/
VALUE MsgConfig_borderColor(VALUE self)
{
   MsgConfig &ref = getRef<MsgConfig>(self);
   return col2hash(ref.borderColor);
}

/*
  call-seq: borderColor=(val)

  Sets the border's color.
*/
VALUE MsgConfig_setBorderColor(VALUE self, VALUE val)
{
   MsgConfig &ref = getRef<MsgConfig>(self);
   ref.borderColor = hash2col(val);

   return val;
}

/*
  Returns the text's font.
*/
VALUE MsgConfig_textFont(VALUE self)
{
   MsgConfig &ref = getRef<MsgConfig>(self);
   return rb_str_new2(ref.textFont.c_str());
}

/*
  call-seq: textFont=(val)

  Sets the text's font.
*/
VALUE MsgConfig_setTextFont(VALUE self, VALUE val)
{
   MsgConfig &ref = getRef<MsgConfig>(self);
   ref.textFont = StringValuePtr(val);

   return val;
}

/*
  Returns the title's font.
*/
VALUE MsgConfig_titleFont(VALUE self)
{
   MsgConfig &ref = getRef<MsgConfig>(self);
   return rb_str_new2(ref.titleFont.c_str());
}

/*
  call-seq: titleFont=(val)

  Sets the title's font.
*/
VALUE MsgConfig_setTitleFont(VALUE self, VALUE val)
{
   MsgConfig &ref = getRef<MsgConfig>(self);
   ref.titleFont = StringValuePtr(val);

   return val;
}

/*
  Returns the title's position
*/
VALUE MsgConfig_titlePos(VALUE self)
{
   MsgConfig &ref = getRef<MsgConfig>(self);
   Point p(ref.titleX, ref.titleY);

   return createObject(getClass("Point"), p);
}

/*
  call-seq: titlePos=(val)

  Sets the title's position.
*/
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
