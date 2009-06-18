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

#ifndef __KERNEL_RUBY_PSP__
#define __KERNEL_RUBY_PSP__

#include "StdInclude.hpp"

class PSP_Directory
{
public:
   ~PSP_Directory();

   void open(string name);

   list<string> ls();
private:
   SceUID fd;

   string _name;
};

VALUE Dir_Wrap(VALUE info);
VALUE Dir_setDir(VALUE self, VALUE name);
void Dir_delete(void *data);
VALUE Dir_List(VALUE self);

VALUE Kernel_cd(VALUE self, VALUE dir);

VALUE Kernel_ExecEboot(VALUE self, VALUE exec);
VALUE Kernel_ExecUpdater(VALUE self, VALUE exec);
VALUE Kernel_DiscExecEboot(VALUE self, VALUE exec);
VALUE Kernel_DiscExecUpdater(VALUE self, VALUE exec);

VALUE Kernel_UmdCheck(VALUE self);
VALUE Kernel_UmdWaitState(VALUE self, VALUE state);
VALUE Kernel_UmdMount(VALUE self);
VALUE Kernel_UmdUmount(VALUE self);
VALUE Kernel_UmdGetType(VALUE self);

VALUE Kernel_Timestamp(VALUE self);

VALUE Kernel_getModel(VALUE self);

void defineKernel();

#endif
