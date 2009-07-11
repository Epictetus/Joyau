#include "MessageBox.hpp"

Message::Message()
{
   image = NULL;
   bg = NULL;

   text = "";
   title = "";

   _x = 0;
   _y = 0;
   marge = 3;

   textColor = RGBA(0, 0, 0, 255);
   titleColor = RGBA(0, 0, 0, 255);
   bgColor = RGBA(0, 0, 255, 127);
   borderColor = RGBA(255, 255, 255, 255);

   bg_pic = false;

   titleFont = "";
   textFont = "";

   titleX = 0;
   titleY = 0;
}

void Message::draw()
{
   Manager *manager = Manager::getInstance(); // Needed for getFont
   
   // Firstly, let's draw the border.
   oslDrawRect(_x, _y, _x + _w, _y + _h, borderColor);

   // Then, the background.
   // If it's a picture :
   if (bg_pic)
   {
      bg->setTile(0, 0, _w - 1, _h - 1);
      bg->setPos(_x + 1, _y + 1);
      bg->Draw();
   }
   else
   {
      // We'll draw it in the border. We don't want to overwrit it.
      oslDrawFillRect(_x + 1, _y +1, _x + _w - 1, _y + _h - 1, bgColor);
   }

   int offset = 0;
   if (image != NULL)
   {
      image->setPos(_x + marge, _y + marge);
      image->Draw();
      offset = image->getW();
   }

   // Now, the title should be drawn.
   // Since we'll draw text, let's stop drawing background for it :
   oslSetBkColor(RGBA(0, 0, 0, 0));
   if (title != "") // Don't lose time if there is nothing to draw...
   {
      if (titleFont != "")
	 oslSetFont(manager->getFont(titleFont.c_str()));
      oslSetTextColor(titleColor);
      oslDrawString(_x + titleX, _y + titleY, title.c_str());
   }

   // Let's finish with the text :
   if (textFont != "")
      oslSetFont(manager->getFont(textFont.c_str()));
   oslSetTextColor(textColor);
   oslDrawTextBox(_x + marge + offset, _y + marge, _x + _w - marge + offset,
		  _y + _h - marge, text.c_str(), 0);
}

void Message::setTitle(string txt) { title = txt; }
void Message::setText(string txt) { text = txt; }

void Message::setImage(Sprite *pic) { image = pic; }

void Message::setBackground(Sprite *pic)
{
   bg_pic = true;
   bg = pic;
}

void Message::setBgColor(OSL_COLOR col) { bgColor = col; }
void Message::setTitleColor(OSL_COLOR col) { titleColor = col; }
void Message::setTextColor(OSL_COLOR col) { textColor = col; }
void Message::setBorderColor(OSL_COLOR col) { borderColor = col; }

void Message::setTextFont(string f) { textFont = f; }
void Message::setTitleFont(string f) { titleFont = f; }

void Message::setPos(int x, int y)
{
   _x = x;
   _y = y;
}

void Message::setTitlePos(int x, int y)
{
   titleX = x;
   titleY = y;
}

void Message::resize(int w, int h)
{
   _w = w;
   _h = h;
}

VALUE Message_draw(VALUE self)
{
   Message *item = getPtr<Message>(self);
   item->draw();

   return Qnil;
}

VALUE Message_setTitle(VALUE self, VALUE txt)
{
   Message *item = getPtr<Message>(self);
   string str = StringValuePtr(txt);

   item->setTitle(str);
   return Qnil;
}


VALUE Message_setText(VALUE self, VALUE txt)
{
   Message *item = getPtr<Message>(self);
   string str = StringValuePtr(txt);

   item->setText(str);
   return Qnil;
}

VALUE Message_setImage(VALUE self, VALUE pic)
{
   Message *item  = getPtr<Message>(self);
   Sprite *spr = getPtr<Sprite>(pic);
   
   item->setImage(spr);
   return Qnil;
}

VALUE Message_setBackground(VALUE self, VALUE pic)
{
   Message *item = getPtr<Message>(self);
   Sprite *spr = getPtr<Sprite>(pic);
 
   item->setBackground(spr);
   return Qnil;
}

VALUE Message_setBgColor(VALUE self, VALUE col)
{
   Message *item = getPtr<Message>(self);
   OSL_COLOR color = hash2col(col);

   item->setBgColor(color);
   return Qnil;
}

VALUE Message_setTitleColor(VALUE self, VALUE col)
{
   Message *item = getPtr<Message>(self);
   OSL_COLOR color = hash2col(col);

   item->setTitleColor(color);
   return Qnil;
}

VALUE Message_setTextColor(VALUE self, VALUE col)
{
   Message *item = getPtr<Message>(self);
   OSL_COLOR color = hash2col(col);

   item->setTextColor(color);
   return Qnil;
}

VALUE Message_setBorderColor(VALUE self, VALUE col)
{
   Message *item = getPtr<Message>(self);
   OSL_COLOR color = hash2col(col);

   item->setBorderColor(color);
   return Qnil;
}

VALUE Message_setTextFont(VALUE self, VALUE f)
{
   Message *item = getPtr<Message>(self);
   string str = StringValuePtr(f);

   item->setTextFont(str);
   return Qnil;
}

VALUE Message_setTitleFont(VALUE self, VALUE f)
{
   Message *item = getPtr<Message>(self);
   string str = StringValuePtr(f);

   item->setTitleFont(str);
   return Qnil;
}

VALUE Message_setPos(VALUE self, VALUE x, VALUE y)
{
   Message *item = getPtr<Message>(self);
   int _x = FIX2INT(x);
   int _y = FIX2INT(y);

   item->setPos(_x, _y);
   return Qnil;
}

VALUE Message_setTitlePos(VALUE self, VALUE x, VALUE y)
{
   Message *item = getPtr<Message>(self);
   int _x = FIX2INT(x);
   int _y = FIX2INT(y);

   item->setTitlePos(_x, _y);
   return Qnil;
}

VALUE Message_resize(VALUE self, VALUE w, VALUE h)
{
   Message *item = getPtr<Message>(self);
   int _w = FIX2INT(w);
   int _h = FIX2INT(h);

   item->resize(_w, _h);
   return Qnil;
}

void defineMessageBox()
{
   VALUE cMessage = defClass<Message>("Message");
   defMethod(cMessage, "draw", Message_draw, 0);
   defMethod(cMessage, "setTitle", Message_setTitle, 1);
   defMethod(cMessage, "setText", Message_setText, 1);
   defMethod(cMessage, "setImage", Message_setImage, 1);
   defMethod(cMessage, "setBackground", Message_setBackground, 1);
   defMethod(cMessage, "setBgColor", Message_setBgColor, 1);
   defMethod(cMessage, "setTitleColor", Message_setTitleColor, 1);
   defMethod(cMessage, "setTextColor", Message_setTextColor, 1);
   defMethod(cMessage, "setBorderColor", Message_setBorderColor, 1);
   defMethod(cMessage, "setTextFont", Message_setTextFont, 1);
   defMethod(cMessage, "setTitleFont", Message_setTitleFont, 1);
   defMethod(cMessage, "setPos", Message_setPos, 2);
   defMethod(cMessage, "setTitlePos", Message_setTitlePos, 2);
   defMethod(cMessage, "resize", Message_resize, 2);
}
