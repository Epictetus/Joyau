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

VALUE Dir_Wrap(VALUE info)
{
   PSP_Directory *item = new PSP_Directory;
   VALUE tdata = Data_Wrap_Struct(info, 0, Dir_delete, item);
   return tdata;
}

VALUE Dir_setDir(VALUE self, VALUE name)
{
   PSP_Directory *item;
   Data_Get_Struct(self, PSP_Directory, item);

   item->open(StringValuePtr(name));

   return Qnil;
}

void Dir_delete(void *data)
{
   delete (PSP_Directory *)data;
}

VALUE Dir_List(VALUE self)
{
   PSP_Directory *item;
   Data_Get_Struct(self, PSP_Directory, item);

   list<string> aList = item->ls();

   VALUE ret = rb_ary_new();

   for (list<string>::iterator i = aList.begin(); i != aList.end(); ++i)
   {
      VALUE val = rb_str_new2((*i).c_str());
      rb_ary_push(ret, val);
   }

   return ret;
}

VALUE File_Wrap(VALUE info)
{
   fstream *item = new fstream;
   VALUE tdata = Data_Wrap_Struct(info, 0, File_delete, item);
   return tdata;
}

void File_delete(void *data)
{
   delete (fstream *)data;
}

VALUE File_open(VALUE self, VALUE name)
{
   fstream *item;
   Data_Get_Struct(self, fstream, item);

   item->open(StringValuePtr(name));

   return Qnil;
}

VALUE File_close(VALUE self)
{
   fstream *item;
   Data_Get_Struct(self, fstream, item);

   item->close();

   return Qnil;
}

VALUE File_getWord(VALUE self)
{
   fstream *item;
   Data_Get_Struct(self, fstream, item);

   string word;
   (*item) >> word;

   return rb_str_new2(word.c_str());
}

VALUE File_getLine(VALUE self)
{
   fstream *item;
   Data_Get_Struct(self, fstream, item);

   char line[256];
   item->getline(line, 255);

   return rb_str_new2(line);
}

VALUE File_write(VALUE self, VALUE text)
{
   fstream *item;
   Data_Get_Struct(self, fstream, item);

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

void defineKernel()
{
   VALUE cDir = rb_define_class("PSPDir", rb_cObject);

   rb_define_singleton_method(cDir, "new", (VALUE(*)(...))&Dir_Wrap, 0);
   rb_define_method(cDir, "setDir", (VALUE(*)(...))&Dir_setDir, 1);
   rb_define_method(cDir, "ls", (VALUE(*)(...))&Dir_List, 0);

   VALUE cFile = rb_define_class("PSPFile", rb_cObject);
   rb_define_singleton_method(cFile, "new", (VALUE(*)(...))&File_Wrap, 0);
   rb_define_method(cFile, "open", (VALUE(*)(...))&File_open, 1);
   rb_define_method(cFile, "close", (VALUE(*)(...))&File_close, 0);
   rb_define_method(cFile, "getWord", (VALUE(*)(...))&File_getWord, 0);
   rb_define_method(cFile, "getLine", (VALUE(*)(...))&File_getLine, 0);
   rb_define_method(cFile, "write", (VALUE(*)(...))&File_write, 1);

   rb_define_global_function("cd", (VALUE(*)(...))&Kernel_cd, 1);
   rb_define_global_function("execEboot", (VALUE(*)(...))&Kernel_ExecEboot, 1);
   rb_define_global_function("execUpdater", (VALUE(*)(...))&Kernel_ExecUpdater,
			     1);
   rb_define_global_function("discExecEboot", 
			     (VALUE(*)(...))&Kernel_DiscExecEboot, 1);
   rb_define_global_function("discExecUpdater", 
			     (VALUE(*)(...))&Kernel_DiscExecUpdater, 1);

   rb_define_global_function("umdCheck", (VALUE(*)(...))&Kernel_UmdCheck, 0);
   rb_define_global_function("umdWaitState", 
			     (VALUE(*)(...))&Kernel_UmdWaitState, 1);
   rb_define_global_function("umdMount", (VALUE(*)(...))&Kernel_UmdMount, 0);
   rb_define_global_function("umdUmount", (VALUE(*)(...))&Kernel_UmdUmount, 0);
   rb_define_global_function("umdType", (VALUE(*)(...))&Kernel_UmdGetType, 0);

   rb_define_global_function("timestamp", (VALUE(*)(...))&Kernel_Timestamp, 0);
   rb_define_global_function("model", (VALUE(*)(...))&Kernel_getModel, 0);

   rb_define_global_function("mkdir", (VALUE(*)(...))&File_mkdir, 1);
   rb_define_global_function("rmdir", (VALUE(*)(...))&File_rmdir, 1);
   rb_define_global_function("rm", (VALUE(*)(...))&File_remove, 1);
   rb_define_global_function("mv", (VALUE(*)(...))&File_rename, 2);
}
