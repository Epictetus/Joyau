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

bool loadStartModule(const std::string &prx)
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

VALUE Usb_activated(VALUE self)
{
   u32 state = sceUsbGetState();
   return state & PSP_USB_ACTIVATED ? Qtrue : Qfalse;
}

VALUE USb_established(VALUE self)
{
   u32 state = sceUsbGetState();
   return state & PSP_USB_CONNECTION_ESTABLISHED ? Qtrue : Qfalse;
}

VALUE Usb_connected(VALUE self)
{
   u32 state = sceUsbGetState();
   return state & PSP_USB_CABLE_CONNECTED ? Qtrue : Qfalse;
}

void defineUsb()
{
   VALUE mUsb = defModule("Usb");

   defModFunc(mUsb, "init", initUsb, 0);
   defModFunc(mUsb, "stop", stopUsb, 0);
   
   defModFunc(mUsb, "connect", usbConnect, 0);
   defModFunc(mUsb, "disconnect", usbDisconnect, 0);

   defModFunc(mUsb, "state", usbState, 0);

   defModFunc(mUsb, "activated?", Usb_activated, 0);
   defModFunc(mUsb, "established?", USb_established, 0);
   defModFunc(mUsb, "connected?", Usb_connected, 0);
}
