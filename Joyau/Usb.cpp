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
   return mod > 0;
}

/*
  Inits the module used for the USB connection.
*/
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

/*
  Stops the USB connection.
*/
VALUE stopUsb(VALUE self)
{
   sceUsbStop(PSP_USBSTOR_DRIVERNAME, 0, 0);
   sceUsbStop(PSP_USBBUS_DRIVERNAME, 0, 0);
   return Qnil;
}

/*
  Establishes the USB connection.
*/
VALUE usbConnect(VALUE self)
{
   sceUsbActivate(0x1c8);
   return Qnil;
}

/*
  Disconnects from the USB connection.
*/
VALUE usbDisconnect(VALUE self)
{
   sceUsbDeactivate(0x1c8);
   sceIoDevctl("fatms0:", 0x0240D81E, NULL, 0, NULL, 0 );
   return Qnil;
}

/*
  Returns a hash where the different connections states are saved, at
  the following keys: "activated", "connected", and "established".
*/
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

/*
  Returns whether the Usb is activated.
*/
VALUE Usb_activated(VALUE self)
{
   u32 state = sceUsbGetState();
   return state & PSP_USB_ACTIVATED ? Qtrue : Qfalse;
}

/*
  Returns whether the connection has been established.
*/
VALUE USb_established(VALUE self)
{
   u32 state = sceUsbGetState();
   return state & PSP_USB_CONNECTION_ESTABLISHED ? Qtrue : Qfalse;
}

/*
  Returns whether the cable is connected.
 */
VALUE Usb_connected(VALUE self)
{
   u32 state = sceUsbGetState();
   return state & PSP_USB_CABLE_CONNECTED ? Qtrue : Qfalse;
}

/*
  Document-class: Joyau::Usb

  This class allows to start the USB connexion through a Ruby script.
*/

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
