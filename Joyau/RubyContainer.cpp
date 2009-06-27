#include "RubyContainer.hpp"

RubyContainer::RubyContainer():
   Container(NULL) // No parent by default
{}

void RubyContainer::setSelf(VALUE val) { self = val; }

void RubyContainer::play()
{
   // The Ruby play function would be called by this. Thus,
   // yeah, a_rb_container.play isn't *really* what we call in playChild.
   ID playFunc = rb_intern("play");
   rb_funcall(self, playFunc, 0);

   playChild();
}

void RubyContainer::draw()
{
   ID drawFunc = rb_intern("draw");
   rb_funcall(self, drawFunc, 0);

   drawChild();
}

VALUE wrapContainer(VALUE info)
{
   RubyContainer *item = new RubyContainer;
   VALUE tdata = Data_Wrap_Struct(info, 0, containerFree, item);
   return tdata;
}

void containerFree(void *info)
{
   RubyContainer *p = (RubyContainer*)info;
   delete p;
   p = NULL; // See how are NULL items managed in Container.cpp
}

VALUE Container_initialize(VALUE self)
{
   RubyContainer *self_p;
   Data_Get_Struct(self, RubyContainer, self_p);

   self_p->setSelf(self);

   return self;
}

VALUE Container_addContent(VALUE self, VALUE item)
{
   RubyContainer *self_p;
   Data_Get_Struct(self, RubyContainer, self_p);

   RubyContainer *item_p;
   Data_Get_Struct(item, RubyContainer, item_p);

   self_p->addContent(item_p);

   return Qnil;
}

VALUE Container_setParent(VALUE self, VALUE parent)
{
   RubyContainer *self_p;
   Data_Get_Struct(self, RubyContainer, self_p);

   RubyContainer *item_p;
   Data_Get_Struct(parent, RubyContainer, item_p);

   self_p->changeParent(item_p);

   return Qnil;
}

VALUE Container_getX(VALUE self)
{
   RubyContainer *self_p;
   Data_Get_Struct(self, RubyContainer, self_p);

   int x = self_p->getX();
   return INT2FIX(x);
}

VALUE Container_getY(VALUE self)
{
   RubyContainer *self_p;
   Data_Get_Struct(self, RubyContainer, self_p);

   int y = self_p->getY();
   return INT2FIX(y);
}

VALUE Container_getW(VALUE self)
{
   RubyContainer *self_p;
   Data_Get_Struct(self, RubyContainer, self_p);

   int w = self_p->getW();
   return INT2FIX(w);
}

VALUE Container_getH(VALUE self)
{
   RubyContainer *self_p;
   Data_Get_Struct(self, RubyContainer, self_p);

   int h = self_p->getH();
   return INT2FIX(h);
}

VALUE Container_move(VALUE self, VALUE x, VALUE y)
{
   RubyContainer *self_p;
   Data_Get_Struct(self, RubyContainer, self_p);

   int _x = FIX2INT(x);
   int _y = FIX2INT(y);

   self_p->move(_x, _y);

   return Qnil;
}

VALUE Container_setPos(VALUE self, VALUE x, VALUE y)
{
   RubyContainer *self_p;
   Data_Get_Struct(self, RubyContainer, self_p);

   int _x = FIX2INT(x);
   int _y = FIX2INT(y);

   self_p->setPos(_x, _y);

   return Qnil;
}

VALUE Container_playChild(VALUE self)
{
   RubyContainer *self_p;
   Data_Get_Struct(self, RubyContainer, self_p);

   self_p->playChild();
   return Qnil;
}

VALUE Container_drawChild(VALUE self)
{
   RubyContainer *self_p;
   Data_Get_Struct(self, RubyContainer, self_p);

   self_p->drawChild();
   return Qnil;
}

void defineContainer()
{
   VALUE cContainer = rb_define_class("Container", rb_cObject);
   
   rb_define_singleton_method(cContainer, "new", 
			      (VALUE(*)(...))&wrapContainer, 0);
   rb_define_method(cContainer, "initialize", 
		    (VALUE(*)(...))&Container_initialize, 0);
   rb_define_method(cContainer, "addContent", 
		    (VALUE(*)(...))&Container_addContent, 1);
   rb_define_method(cContainer, "setParent", 
		    (VALUE(*)(...))&Container_setParent, 1);
   rb_define_method(cContainer, "getX", 
			      (VALUE(*)(...))&Container_getX, 0);
   rb_define_method(cContainer, "getY", 
			      (VALUE(*)(...))&Container_getY, 0);
   rb_define_method(cContainer, "getW", 
			      (VALUE(*)(...))&Container_getW, 0);
   rb_define_method(cContainer, "getH", 
			      (VALUE(*)(...))&Container_getH, 0);
   rb_define_method(cContainer, "move", 
			      (VALUE(*)(...))&Container_move, 2);
   rb_define_method(cContainer, "setPos", 
			      (VALUE(*)(...))&Container_setPos, 2);
   rb_define_method(cContainer, "playChild", 
			      (VALUE(*)(...))&Container_playChild, 0);
}
