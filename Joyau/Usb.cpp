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

bool loadStartModule(string prx)
{
   SceUID mod = pspSdkLoadStartModule(prx.c_str(), PSP_MEMORY_PARTITION_KERNEL);
   if (mod < 0)
      return false;
   return true;
}

VALUE initUsb(VALUE self)
{
   if (!loadStartModule("flash0:/kd/chkreg.prx") ||   
       !loadStartModule("flash0:/kd/npdrm.prx") ||
       !loadStartModule("flash0:/kd/semawm.prx") ||
       !loadStartModule("flash0:/kd/usbstor.prx") ||
       !loadStartModule("flash0:/kd/usbstormgr.prx") ||
       !loadStartModule("flash0:/kd/usbstorms.prx") ||
       !loadStartModule("flash0:/kd/usbstorboot.prx"))
      return Qfalse;

   if (sceUsbStart(PSP_USBBUS_DRIVERNAME, 0, 0) != 0)
      return Qfalse;
   if (sceUsbStart(PSP_USBSTOR_DRIVERNAME, 0, 0) != 0)
      return Qfalse;
   if (sceUsbstorBootSetCapacity(0x800000) != 0)
      return Qfalse;

   return Qtrue;
}

VALUE stopUsb(VALUE self)
{
   sceUsbStop(PSP_USBSTOR_DRIVERNAME, 0, 0);
   sceUsbStop(PSP_USBBUS_DRIVERNAME, 0, 0);
   return Qnil;
}

VALUE usbConnect(VALUE self)
{
   sceUsbActivate(0x1c8);
   return Qnil;
}

VALUE usbDisconnect(VALUE self)
{
   sceUsbDeactivate(0x1c8);
   sceIoDevctl("fatms0:", 0x0240D81E, NULL, 0, NULL, 0 );
   return Qnil;
}

VALUE usbState(VALUE self)
{
   VALUE ret = rb_hash_new();
   u32 state = sceUsbGetState();
   rb_hash_aset(ret, rb_str_new2("activated"), 
		state & PSP_USB_ACTIVATED ? Qtrue : Qfalse);
   rb_hash_aset(ret, rb_str_new2("connected"), 
		state & PSP_USB_CABLE_CONNECTED ? Qtrue : Qfalse);
   rb_hash_aset(ret, rb_str_new2("established"), 
		state & PSP_USB_CONNECTION_ESTABLISHED ? Qtrue : Qfalse);
   
   return ret;
}

void defineUsb()
{
   defFunc("initUsb", initUsb, 0);
   defFunc("stopUsb", stopUsb, 0);

   defFunc("usbConnect", usbConnect, 0);
   defFunc("usbDisconnect", usbDisconnect, 0);

   defFunc("usbState", usbState, 0);
}
