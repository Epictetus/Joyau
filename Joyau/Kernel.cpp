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

VALUE Kernel_ExecEboot(VALUE self, VALUE exec)
{
   sceKernelLoadExecVSHMs2(StringValuePtr(exec), NULL);

   return Qnil;
}

VALUE Kernel_ExecUpdater(VALUE self, VALUE exec)
{
   sceKernelLoadExecVSHDiscUpdater(StringValuePtr(exec), NULL);

   return Qnil;
}

VALUE Kernel_DiscExecEboot(VALUE self, VALUE exec)
{
   sceKernelLoadExecVSHDisc(StringValuePtr(exec), NULL);

   return Qnil;
}

VALUE Kernel_DiscExecUpdater(VALUE self, VALUE exec)
{
   sceKernelLoadExecVSHMs1(StringValuePtr(exec), NULL);

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

VALUE Kernel_getModel(VALUE self)
{
   if (sceKernelGetModel() == 0)
      return rb_str_new2("fat");
   else if (sceKernelGetModel() == 1)
      return rb_str_new2("slim");
   else
      return rb_str_new2("brite");
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
   rb_define_method(cDir, "setDir", RPROTO(Dir_setDir), 1);
   rb_define_method(cDir, "ls", RPROTO(Dir_List), 0);

   VALUE cFile = defClass<fstream>("PSPFile");
   rb_define_method(cFile, "open", RPROTO(File_open), 1);
   rb_define_method(cFile, "close", RPROTO(File_close), 0);
   rb_define_method(cFile, "getWord", RPROTO(File_getWord), 0);
   rb_define_method(cFile, "getLine", RPROTO(File_getLine), 0);
   rb_define_method(cFile, "write", RPROTO(File_write), 1);

   rb_define_global_function("cd", RPROTO(Kernel_cd), 1);
   rb_define_global_function("execEboot", RPROTO(Kernel_ExecEboot), 1);
   rb_define_global_function("execUpdater", RPROTO(Kernel_ExecUpdater), 1);
   rb_define_global_function("discExecEboot", RPROTO(Kernel_DiscExecEboot), 1);
   rb_define_global_function("discExecUpdater", RPROTO(Kernel_DiscExecUpdater), 
			     1);
   rb_define_global_function("umdCheck", RPROTO(Kernel_UmdCheck), 0);
   rb_define_global_function("umdWaitState", RPROTO(Kernel_UmdWaitState), 1);
   rb_define_global_function("umdMount", RPROTO(Kernel_UmdMount), 0);
   rb_define_global_function("umdUmount", RPROTO(Kernel_UmdUmount), 0);
   rb_define_global_function("umdType", RPROTO(Kernel_UmdGetType), 0);

   rb_define_global_function("timestamp", RPROTO(Kernel_Timestamp), 0);
   rb_define_global_function("model", RPROTO(Kernel_getModel), 0);

   rb_define_global_function("powerTime", RPROTO(Kernel_getPowerTime), 0);
   rb_define_global_function("powerPercent", RPROTO(Kernel_getPowerPercent), 0);

   rb_define_global_function("mkdir", RPROTO(File_mkdir), 1);
   rb_define_global_function("rmdir", RPROTO(File_rmdir), 1);
   rb_define_global_function("rm", RPROTO(File_remove), 1);
   rb_define_global_function("mv", RPROTO(File_rename), 2);
}
