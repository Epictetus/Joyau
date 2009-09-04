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

#ifndef JOYAU_KERNEL
#define JOYAU_KERNEL

#include "StdInclude.hpp"

VALUE File_remove(VALUE self, VALUE file);
VALUE File_mkdir(VALUE self, VALUE dir);
VALUE File_rmdir(VALUE self, VALUE dir);
VALUE File_rename(VALUE self, VALUE old, VALUE newName);

VALUE Kernel_cd(VALUE self, VALUE dir);

VALUE Kernel_ExecEboot(VALUE self, VALUE exec);
VALUE Kernel_ExecUpdater(VALUE self, VALUE exec);
VALUE Kernel_DiscExecEboot(VALUE self, VALUE exec);
VALUE Kernel_DiscExecUpdater(VALUE self, VALUE exec);

VALUE Kernel_UmdCheck(VALUE self);
VALUE Kernel_UmdWaitState(VALUE self, VALUE state);
VALUE Kernel_UmdMount(VALUE self);
VALUE Kernel_UmdUmount(VALUE self);
VALUE Kernel_UmdPermitReplace(VALUE self);
VALUE Kernel_UmdProhibitReplace(VALUE self);

VALUE Kernel_Timestamp(VALUE self);

VALUE Kernel_getModel(VALUE self);

VALUE Kernel_getPowerTime(VALUE self);
VALUE Kernel_getPowerPercent(VALUE self);

void defineKernel();

#endif
