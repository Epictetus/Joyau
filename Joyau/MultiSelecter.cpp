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

#include "MultiSelecter.hpp"

template<>
/*
  call-seq: new()

  Creates a new selecter.
*/
VALUE wrap<MultiHorizontalMsgSelecter>(int argc, VALUE *argv, VALUE info)
{
   MultiHorizontalMsgSelecter *ptr = new MultiHorizontalMsgSelecter;
   ptr->setClass("MultiHorizontalMsgSelecter");
   VALUE tdata = Data_Wrap_Struct(info, 0, 
				  wrapped_free<MultiHorizontalMsgSelecter>, ptr);
   return tdata;
}

template<>
/*
  call-seq: new()

  Creates a new selecter.
*/
VALUE wrap<MultiVerticalMsgSelecter>(int argc, VALUE *argv, VALUE info)
{
   MultiVerticalMsgSelecter *ptr = new MultiVerticalMsgSelecter;
   ptr->setClass("MultiVerticalMsgSelecter");
   VALUE tdata = Data_Wrap_Struct(info, 0, 
				  wrapped_free<MultiVerticalMsgSelecter>, ptr);
   return tdata;
}

/*
  Document-class: Joyau::MultiVerticalMsgSelecter

  Class which allows to select between some VerticalMsgSelecter.
*/

/*
  Document-class: Joyau::MultiHorizontalMsgSelecter

  Class which allows to select between some HorizontalMsgSelecter.
*/

void defineMultiSelecter()
{
 defineGenericSelecter<MultiVerticalMsgSelecter>("MultiVerticalMsgSelecter");
 defineGenericSelecter<MultiHorizontalMsgSelecter>("MultiHorizontalMsgSelecter");
}
