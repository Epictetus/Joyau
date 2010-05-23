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

#ifndef JOYAU_MULTI_SELECTER
#define JOYAU_MULTI_SELECTER

#include "MsgSelecter.hpp"

/** @addtogroup Drawables **/
/*@{*/

/** 
 * @class MultiSelecter
 * A selecter's selecter
 */
template<typename Father>
class MultiSelecter: public Father
{
public:
   /** Type representing the content's type. **/
   typedef typename Father::content_t content_t;

   void atLoop(content_t &obj, size_t pos)
   {
      Father::atLoop(obj, pos);
      obj.setFocus(false);
   }

   void forFocused(content_t &obj)
   {
      obj.setFocus(true);
   }
};

/** A VerticalMsgSelecter's selecter **/
typedef MultiSelecter< HorizontalSelecter<VerticalMsgSelecter> > 
MultiVerticalMsgSelecter;

/** An HorizontalMsgSelecter's selecter **/
typedef MultiSelecter< VerticalSelecter<HorizontalMsgSelecter> > 
MultiHorizontalMsgSelecter;

/*@}*/

void defineMultiSelecter();

#endif
