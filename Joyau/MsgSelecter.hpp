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

/** @addtogroup Drawables **/
/*@{*/

/** 
 * @class MsgConfig
 * Used for Message's configuration
 */
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

   MsgConfig& operator=(const MsgConfig& obj) 
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

   /** Pointer to the image set on a Message **/
   Sprite *image;

   /** Pointer to the image set as background on a Message **/
   Sprite *bg;

   /** Color used for the text **/
   OSL_COLOR textColor;

   /** Color used for the title **/
   OSL_COLOR titleColor;

   /** Color used for the background **/
   OSL_COLOR bgColor;
   
   /** Color used for the border **/
   OSL_COLOR borderColor;

   std::string titleFont;
   std::string textFont;

   /** title's x position**/
   int titleX;

   /** title's y position **/
   int titleY;
};

/** 
 * @class MsgSelecter
 * Allows to select a message. Father should be 
 * HorizontalSelecter<Message>, or VerticalSelecter<Message>.
 */
template<typename Father>
class MsgSelecter: public Father
{
public:
   /** Type representing the content's type. **/
   typedef typename Father::content_t content_t;

   void forFocused(content_t &obj);
   void atLoop(content_t &obj, size_t pos);

   /** Sets the configuration applied to any Message.
    *  @param val a configuration
    */
   void setConf(const MsgConfig &val) { conf = val; }

   /** Sets the configuration applied to the focused Message.
    *  @param val a configuration
    */
   void setFocusConf(const MsgConfig &val) { focusConf = val; }

   /** Returns the configuration applied to any Message. **/
   MsgConfig& getConf() { return conf; }

   /** Returns the configuration applied to the focused Message. **/
   MsgConfig& getFocusConf() { return focusConf; }
protected:
   MsgConfig conf;
   MsgConfig focusConf;

   /** Applies a configuration to a Message. **/
   void applyConf(content_t &obj, MsgConfig &arg);
};

/*@}*/

template<typename T> VALUE MsgSelecter_setConf(VALUE self, VALUE val);
template<typename T> VALUE MsgSelecter_setFocusConf(VALUE self, VALUE val);

template<typename T> VALUE MsgSelecter_conf(VALUE self);
template<typename T> VALUE MsgSelecter_focusConf(VALUE self);

template<typename T> VALUE defineMsgSelecterKlass(const char *name);

#include "MsgSelecterImpl.hpp"

/** @addtogroup Drawables **/
/*@{*/

/** Allows to select message horizontally. **/
typedef MsgSelecter< HorizontalSelecter<Message> > HorizontalMsgSelecter;

/** Allows to select message vertically. **/
typedef MsgSelecter< VerticalSelecter<Message> > VerticalMsgSelecter;

/*@}*/

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
