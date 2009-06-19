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

#include "Usb.hpp"

VALUE Usb_Init(VALUE self)
{
   int res = sceKernelLoadModule("flash0:/kd/semawm.prx", 0, NULL);
   sceKernelStartModule(res , 0, NULL, NULL, NULL);

   res = sceKernelLoadModule("flash0:/kd/usbstor.prx", 0, NULL);
   sceKernelStartModule(res , 0, NULL, NULL, NULL);

   res = sceKernelLoadModule("flash0:/kd/usbstormgr.prx", 0, NULL);
   sceKernelStartModule(res , 0, NULL, NULL, NULL);

   res = sceKernelLoadModule("flash0:/usbstorms.prx", 0, NULL);
   sceKernelStartModule(res , 0, NULL, NULL, NULL);

   res = sceKernelLoadModule("flash0:/kd/usbstorboot.prx", 0, NULL);
   sceKernelStartModule(res , 0, NULL, NULL, NULL);

   sceUsbStart(PSP_USBBUS_DRIVERNAME, 0, 0);
   sceUsbStart(PSP_USBSTOR_DRIVERNAME, 0, 0);
   sceUsbstorBootSetCapacity(0x800000);

   return Qnil;
}

VALUE Usb_mount(VALUE self)
{
   sceUsbActivate(0x1c8);
   return Qnil;
}

VALUE Usb_umount(VALUE self)
{
   sceUsbDeactivate(0x1c8);
   return Qnil;
}

VALUE Usb_state(VALUE self)
{
   int state = sceUsbGetState();
   VALUE ret = rb_hash_new();
   if (state & PSP_USB_ACTIVATED)
      rb_hash_aset(ret, rb_str_new2("activated"), Qtrue);
   else
      rb_hash_aset(ret, rb_str_new2("activated"), Qfalse);
   if (state & PSP_USB_CABLE_CONNECTED)
      rb_hash_aset(ret, rb_str_new2("connected"), Qtrue);
   else
      rb_hash_aset(ret, rb_str_new2("connected"), Qfalse);
   if (state & PSP_USB_CONNECTION_ESTABLISHED)
      rb_hash_aset(ret, rb_str_new2("established"), Qtrue);
   else
      rb_hash_aset(ret, rb_str_new2("established"), Qfalse);

   return ret;
}

void defineUsb()
{
   rb_define_global_function("initUsb", (VALUE(*)(...))&Usb_Init, 0);
   rb_define_global_function("activateUsb", (VALUE(*)(...))&Usb_mount, 0);
   rb_define_global_function("deactivateUsb", (VALUE(*)(...))&Usb_umount, 0);
   rb_define_global_function("usbState", (VALUE(*)(...))&Usb_state, 0);
}
