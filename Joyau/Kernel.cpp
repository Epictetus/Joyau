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

/*
  call-seq: cd(dirname)

  Changes the current directory.
*/
VALUE Kernel_cd(VALUE self, VALUE dir)
{
   sceIoChdir(StringValuePtr(dir));

   return Qnil;
}

/*
  Returns whether a UMD is inside the PSP.
*/
VALUE Kernel_UmdCheck(VALUE self)
{
   if (sceUmdCheckMedium() == 0)
      return Qfalse;
   return Qtrue;
}

/*
  call-seq: waitState(state)

  Wait for a state of the UMD drive.
*/
VALUE Kernel_UmdWaitState(VALUE self, VALUE state)
{
   sceUmdWaitDriveStat(FIX2INT(state));
   return Qnil;
}

/*
  Mounts the UMD.
*/
VALUE Kernel_UmdMount(VALUE self)
{
   sceUmdActivate(1, "disc0:");
   sceUmdWaitDriveStat(PSP_UMD_READY);

   return Qnil;
}

/*
  Unmount the UMD.
*/
VALUE Kernel_UmdUmount(VALUE self)
{
   sceUmdDeactivate(1, "discO:");
   return Qnil;
}

/*
  Allows to change the actual UMD.
*/
VALUE Kernel_UmdPermitReplace(VALUE self)
{
   sceUmdReplacePermit();
   return Qnil;
}

/*
  Prohibits to change the actual UMD.
*/
VALUE Kernel_UmdProhibitReplace(VALUE self)
{
   sceUmdReplaceProhibit();
   return Qnil;
}

/*
  Returns the actual timestamp.
*/
VALUE Kernel_Timestamp(VALUE self)
{
   time_t time;
   sceKernelLibcTime(&time);

   return rb_uint2big(time);
}

/*
  call-seq: rm(file)

  Removes a file.
*/
VALUE File_remove(VALUE self, VALUE file)
{
   sceIoRemove(StringValuePtr(file));
   return Qnil;
}


/*
  call-seq: mkdir(dir)

  Creates a directory.
*/
VALUE File_mkdir(VALUE self, VALUE dir)
{
   sceIoMkdir(StringValuePtr(dir), 0777);

   return Qnil;
}

/*
  call-seq: rmdir(dir)

  Removes a directory.
*/
VALUE File_rmdir(VALUE self, VALUE dir)
{
   sceIoRmdir(StringValuePtr(dir));
   return Qnil;
}

/*
  call-seq: rename(old, new)

  Renames a file.
*/
VALUE File_rename(VALUE self, VALUE old, VALUE newName)
{
   sceIoRename(StringValuePtr(old), StringValuePtr(newName));
   return Qnil;
}

/*
  Returns the battery lifetime.
*/
VALUE Kernel_getPowerTime(VALUE self)
{
   return INT2FIX(scePowerGetBatteryLifeTime());
}

/*
  Returns the battery life percentage.
*/
VALUE Kernel_getPowerPercent(VALUE self)
{
   return INT2FIX(scePowerGetBatteryLifePercent());
}

void defineKernel()
{
   VALUE joyau = JOYAU_MOD;
   defModFunc(joyau, "cd", Kernel_cd, 1);

   VALUE mUmd = defModule("Umd");
   defModFunc(mUmd, "present?", Kernel_UmdCheck, 0);
   defModFunc(mUmd, "waitState", Kernel_UmdWaitState, 1);
   
   defModFunc(mUmd, "mount", Kernel_UmdMount, 0);
   defModFunc(mUmd, "umount", Kernel_UmdUmount, 0);

   defModFunc(mUmd, "permitReplace", Kernel_UmdPermitReplace, 0);
   defModFunc(mUmd, "prohibitReplace", Kernel_UmdProhibitReplace, 0);

   /*
     Document-const: NOT_PRESENT
     PSP_UMD_NOT_PRESENT: No UMD inside the drive.
   */

   /*
     Document-const: PRESENT
     PSP_UMD_PRESENT: A UMD is inside the drive.
   */

   /*
     Document-const: CHANGED
     PSP_UMD_CHANGED: Changed the UMD present in the driver.
   */

   /*
     Document-const: INITING
     PSP_UMD_INITING: UMD initing.
   */

   /*
     Document-const: INITED
     PSP_UMD_INITED: UMD inited.
   */
   
   /*
     Document-const: READY
     PSP_UMD_READY: UMD ready.
   */

   defConst(mUmd, "NOT_PRESENT", INT2FIX(PSP_UMD_NOT_PRESENT));
   defConst(mUmd, "PRESENT", INT2FIX(PSP_UMD_PRESENT));
   defConst(mUmd, "CHANGED", INT2FIX(PSP_UMD_CHANGED));
   defConst(mUmd, "INITING", INT2FIX(PSP_UMD_INITING));
   defConst(mUmd, "INITED", INT2FIX(PSP_UMD_INITED));
   defConst(mUmd, "READY", INT2FIX(PSP_UMD_READY));
   
   defModFunc(joyau, "timestamp", Kernel_Timestamp, 0);

   defModFunc(joyau, "powerTime", Kernel_getPowerTime, 0);
   defModFunc(joyau, "powerPercent", Kernel_getPowerPercent, 0);

   defModFunc(joyau, "mkdir", File_mkdir, 1);
   defModFunc(joyau, "rmdir", File_rmdir, 1);
   defModFunc(joyau, "rm", File_remove, 1);
   defModFunc(joyau, "mv", File_rename, 2);
}
