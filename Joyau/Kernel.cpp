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

void PSP_Directory::open(string name)
{
   fd = sceIoDopen(name.c_str());
   _name = name;
}

PSP_Directory::~PSP_Directory()
{
   sceIoDclose(fd);
}

list<string> PSP_Directory::ls()
{
   list<string> theList;
   SceIoDirent item;
   while (sceIoDread(fd, &item) != 0)
   {
      theList.push_back(item.d_name);
   }

   return theList;
}

VALUE Dir_setDir(VALUE self, VALUE name)
{
   PSP_Directory *item;
   Data_Get_Struct(self, PSP_Directory, item);

   item->open(StringValuePtr(name));

   return Qnil;
}

VALUE Dir_List(VALUE self)
{
   PSP_Directory *item = getPtr<PSP_Directory>(self);
    list<string> aList = item->ls();

   VALUE ret = rb_ary_new();

   for (list<string>::iterator i = aList.begin(); i != aList.end(); ++i)
   {
      VALUE val = rb_str_new2((*i).c_str());
      rb_ary_push(ret, val);
   }

   return ret;
}

VALUE File_open(VALUE self, VALUE name)
{
   fstream *item = getPtr<fstream>(self);
   item->open(StringValuePtr(name));

   return Qnil;
}

VALUE File_close(VALUE self)
{
   fstream *item = getPtr<fstream>(self);
   item->close();

   return Qnil;
}

VALUE File_getWord(VALUE self)
{
   fstream *item = getPtr<fstream>(self);

   string word;
   (*item) >> word;

   return rb_str_new2(word.c_str());
}

VALUE File_getLine(VALUE self)
{
   fstream *item = getPtr<fstream>(self);

   char line[256];
   item->getline(line, 255);

   return rb_str_new2(line);
}

VALUE File_write(VALUE self, VALUE text)
{
   fstream *item = getPtr<fstream>(self);

   char *str = StringValuePtr(text);
   (*item) << str;

   return Qnil;
}

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
   VALUE cDir = defClass<PSP_Directory>("PSPDir");
   defMethod(cDir, "setDir", Dir_setDir, 1);
   defMethod(cDir, "ls", Dir_List, 0);

   VALUE cFile = defClass<fstream>("PSPFile");
   defMethod(cFile, "open", File_open, 1);
   defMethod(cFile, "close", File_close, 0);
   defMethod(cFile, "getWord", File_getWord, 0);
   defMethod(cFile, "getLine", File_getLine, 0);
   defMethod(cFile, "write", File_write, 1);

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
