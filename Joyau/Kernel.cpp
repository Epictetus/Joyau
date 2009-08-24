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

#include "Kernel.hpp"

using namespace std;

VALUE Kernel_cd(VALUE self, VALUE dir)
{
   sceIoChdir(StringValuePtr(dir));

   return Qnil;
}

VALUE Kernel_UmdCheck(VALUE self)
{
   if (sceUmdCheckMedium() == 0)
      return Qfalse;
   return Qtrue;
}

VALUE Kernel_UmdWaitState(VALUE self, VALUE state)
{
   string str = StringValuePtr(state);

   if (str == "PRESENT")
      sceUmdWaitDriveStat(PSP_UMD_PRESENT);
   else if (str == "NOT_PRESENT")
      sceUmdWaitDriveStat(PSP_UMD_NOT_PRESENT);
   if (str == "CHANGED")
      sceUmdWaitDriveStat(PSP_UMD_CHANGED);
   if (str == "INITING")
      sceUmdWaitDriveStat(PSP_UMD_INITING);
   if (str == "INITED")
      sceUmdWaitDriveStat(PSP_UMD_INITED);
   if (str == "READY")
      sceUmdWaitDriveStat(PSP_UMD_READY);

   return Qnil;
}

VALUE Kernel_UmdMount(VALUE self)
{
   sceUmdActivate(1, "disc0:");
   sceUmdWaitDriveStat(PSP_UMD_READY);

   return Qnil;
}

VALUE Kernel_UmdUmount(VALUE self)
{
   sceUmdDeactivate(1, "discO:");
   return Qnil;
}

VALUE Kernel_UmdGetType(VALUE self)
{
   pspUmdInfo info;
   sceUmdGetDiscInfo(&info);
   string val;
   if (info.type == PSP_UMD_TYPE_GAME)
      val = "GAME";
   else if (info.type == PSP_UMD_TYPE_VIDEO)
      val = "VIDEO";
   else
      val = "AUDIO";
   return rb_str_new2(val.c_str());
}

VALUE Kernel_Timestamp(VALUE self)
{
   time_t time;
   sceKernelLibcTime(&time);

   return rb_uint2big(time);
}

VALUE File_remove(VALUE self, VALUE file)
{
   sceIoRemove(StringValuePtr(file));
   return Qnil;
}

VALUE File_mkdir(VALUE self, VALUE dir)
{
   sceIoMkdir(StringValuePtr(dir), 0777);

   return Qnil;
}

VALUE File_rmdir(VALUE self, VALUE dir)
{
   sceIoRmdir(StringValuePtr(dir));
   return Qnil;
}

VALUE File_rename(VALUE self, VALUE old, VALUE newName)
{
   sceIoRename(StringValuePtr(old), StringValuePtr(newName));
   return Qnil;
}

VALUE Kernel_getPowerTime(VALUE self)
{
   return INT2FIX(scePowerGetBatteryLifeTime());
}

VALUE Kernel_getPowerPercent(VALUE self)
{
   return INT2FIX(scePowerGetBatteryLifePercent());
}

void defineKernel()
{
   defFunc("cd", Kernel_cd, 1);
   defFunc("umdCheck", Kernel_UmdCheck, 0);
   defFunc("umdWaitState", Kernel_UmdWaitState, 1);
   defFunc("umdMount", Kernel_UmdMount, 0);
   defFunc("umdUmount", Kernel_UmdUmount, 0);
   defFunc("umdType", Kernel_UmdGetType, 0);

   defFunc("timestamp", Kernel_Timestamp, 0);

   defFunc("powerTime", Kernel_getPowerTime, 0);
   defFunc("powerPercent", Kernel_getPowerPercent, 0);

   defFunc("mkdir", File_mkdir, 1);
   defFunc("rmdir", File_rmdir, 1);
   defFunc("rm", File_remove, 1);
   defFunc("mv", File_rename, 2);
}
