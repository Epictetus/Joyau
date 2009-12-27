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

#ifndef JOYAU_MESSAGE
#define JOYAU_MESSAGE

#include "Drawable.hpp"

class Sprite;

/** @addtogroup Drawables **/
/*@{*/

/** 
 * @class Message
 * Draws text in a message box
 */
class Message: public Drawable
{
public:
   Message();

   void draw();

   /** Changes the message box title.
    *  @param txt new title.
    */
   void setTitle(const std::string &txt);

   /** Changes the message box text.
    *  @param txt new text.
    */
   void setText(const std::string &txt);

   /** Sets the image which will be drawed on the message box.
    *  ( NULL will remove it )
    *  @param pic a pointer to the picture which should be added.
    */
   void setImage(Sprite *pic);

   /** Sets the image which will be drawed as background (instead of a color)
    *  @param pic a pointer to the picture which should be used.
    */
   void setBackground(Sprite *pic);

   /** Sets the background's color.
    *  @param col new color.
    */
   void setBgColor(OSL_COLOR col);

   /** Sets the title's color.
    *  @param col new color.
    */
   void setTitleColor(OSL_COLOR col);

   /** Sets the text's color.
    *  @param col new color.
    */
   void setTextColor(OSL_COLOR col);

   /** Sets the border's color.
    *  @param col new color.
    */
   void setBorderColor(OSL_COLOR col);

   /** Changes the font used for the text.
    *  @param f font name
    */
   void setTextFont(const std::string &f);

   /** Changes the font used for the title.
    *  @param f font name
    */
   void setTitleFont(const std::string &f);

   /** Sets the title position.
    *  @param x title's x position, relatively to the message box.
    *  @param y title's y position, relatively to the message box.
    */
   void setTitlePos(int x, int y);
   
   /** Sets the title position.
    *  @param p title's position, relatively to the message box.
    */
   void setTitlePos(const Point &p);

   /** Returns the title **/
   std::string getTitle() const { return title; }

   /** Returns the text **/
   std::string getText() const { return text; }

   /** Returns a reference to the image draw on the Message **/
   Sprite *getImage() const { return image; }

   /** Returns a reference to the Message's background **/
   Sprite *getBackground() const { return bg; }

   /** Returns the background's color **/
   OSL_COLOR getBgColor() const { return bgColor; }

   /** Returns the title's color **/
   OSL_COLOR getTitleColor() const { return titleColor; }

   /** Returns the text's color **/
   OSL_COLOR getTextColor() const { return textColor; }

   /** Returns the border's color **/
   OSL_COLOR getBorderColor() const { return borderColor; }

   /** Returns the title's font **/
   std::string getTitleFont() const { return titleFont; }
   
   /** Returns the text's font **/
   std::string getTextFont() const { return textFont; }

   /** Returns the title's position **/
   Point getTitlePos() const { return Point(titleX, titleY); }
   
   /** Resizes the Message.
    *  @param w new width.
    *  @param h new height.
    */
   void resize(int w, int h);

   /** Changes the Message's width.
    *  @param w new width.
    */
   void setW(int w) { _w = w; }

   /** Changes the Message's height.
    *  @param h new height.
    */
   void setH(int h) { _h = h; }
private:
   Sprite *image;
   Sprite *bg;

   std::string text;
   std::string title;

   int marge;

   OSL_COLOR textColor;
   OSL_COLOR titleColor;
   OSL_COLOR bgColor;
   OSL_COLOR borderColor;

   bool bg_pic;

   std::string titleFont;
   std::string textFont;

   int titleX, titleY; //relatively to _x and _y.
};

/*@}*/

VALUE Message_setTitle(VALUE self, VALUE txt);
VALUE Message_setText(VALUE self, VALUE txt);

VALUE Message_setImage(VALUE self, VALUE pic);
VALUE Message_setBackground(VALUE self, VALUE pic);

VALUE Message_setBgColor(VALUE self, VALUE col);
VALUE Message_setTitleColor(VALUE self, VALUE col);
VALUE Message_setTextColor(VALUE self, VALUE col);
VALUE Message_setBorderColor(VALUE self, VALUE col);

VALUE Message_setTextFont(VALUE self, VALUE f);
VALUE Message_setTitleFont(VALUE self, VALUE f);

VALUE Message_setTitlePos(VALUE self, VALUE x, VALUE y);
VALUE Message_setTitlePoint(VALUE self, VALUE p);

VALUE Message_title(VALUE self);
VALUE Message_text(VALUE self);

VALUE Message_image(VALUE self);
VALUE Message_background(VALUE self);

VALUE Message_bgColor(VALUE self);
VALUE Message_titleColor(VALUE self);
VALUE Message_textColor(VALUE self);
VALUE Message_borderColor(VALUE self);

VALUE Message_textFont(VALUE self);
VALUE Message_titleFont(VALUE self);

VALUE Message_titlePos(VALUE self);

VALUE Message_resize(VALUE self, VALUE w, VALUE h);
VALUE Message_setW(VALUE self, VALUE w);
VALUE Message_setH(VALUE self, VALUE h);

void defineMessageBox();

#endif
