/*Copyright (C) 2009-2010 Verhetsel Kilian

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

#include "Wlan.hpp"

#include <pspwlan.h>

#include <pspnet.h>
#include <pspnet_inet.h>
#include <pspnet_resolver.h>
#include <pspnet_apctl.h>

#include <psputility.h>

static VALUE rb_eTimeoutError;

/*
  Inits the wlan connection. You have to call this before calling the socket
  function.
*/
VALUE Wlan_init(VALUE self) {
    int err;
    err = sceUtilityLoadNetModule(PSP_NET_MODULE_COMMON);
    if (err < 0)
       rb_raise(rb_eRuntimeError, "Failled to load net module");

    err = sceUtilityLoadNetModule(PSP_NET_MODULE_INET);
    if (err < 0)
       rb_raise(rb_eRuntimeError, "Failled to load inet module");

    err = sceNetInit(0x20000, 0x20, 0x1000, 0x20, 0x1000);
    if (err < 0)
        rb_raise(rb_eRuntimeError, "Failled to init net module");

    err = sceNetInetInit();
    if (err < 0)
       rb_raise(rb_eRuntimeError, "Failled to init inet module");

    err = sceNetResolverInit();
    if (err < 0)
        rb_raise(rb_eRuntimeError, "Failled to init resolver");
      
    err = sceNetApctlInit(0x1400, 0x42);
    if (err < 0)
        rb_raise(rb_eRuntimeError, "Failled to init apctl");
 
    return Qnil;
}

/*
  call-seq: connect(access_point, timeout)

  Connects to a given acces point. You can specify a timeout in seconds.
  If the connection isn't established after the specified timeout,
  a TimeoutError is raised.
*/
VALUE Wlan_connect(VALUE self, VALUE config, VALUE timeout) {
   if (sceNetApctlConnect(FIX2INT(config)))
      rb_raise(rb_eRuntimeError, "Failled to connect to acces point %d",
	       FIX2INT(config));
   
   time_t startTime, currTime;
   time(&startTime);

   int last_state = -1;

   while (true) {
      time(&currTime);
      if (currTime - startTime >= FIX2INT(timeout)) {
	 sceNetApctlDisconnect();
	 rb_raise(rb_eTimeoutError, "Connection timeouted after %d seconds",
		  FIX2INT(timeout));
	 break;
      }

      int state;
      if (sceNetApctlGetState(&state)) {
	 rb_raise(rb_eRuntimeError, 
		  "Error occured while getting connection state");
	 break;
      }

      if (state > last_state) {
	 last_state = state;
      }

      if (state == PSP_NET_APCTL_STATE_GOT_IP)
	 break;

      sceKernelDelayThread(50 * 1000);
   }
   
   return Qnil;
}

/*
  Ends a Wlan connection.
*/
VALUE Wlan_disconnect(VALUE self) {
   sceNetApctlDisconnect();
   return Qnil;
}

/*
  Unlods net modules.
*/
VALUE Wlan_stop(VALUE self) {
   sceNetApctlTerm();

   sceUtilityUnloadNetModule(PSP_NET_MODULE_INET);
   sceUtilityUnloadNetModule(PSP_NET_MODULE_COMMON);

   return Qnil;
}

/*
  Returns whether the wlan button is enabled.
*/
VALUE Wlan_button_enabled(VALUE self) {
   return sceWlanDevIsPowerOn() ? Qtrue : Qfalse;
}

/*
  Returns whether we're connected to an acces point.
*/
VALUE Wlan_is_connected(VALUE self) {
   union SceNetApctlInfo apctlInfo;  
   return sceNetApctlGetInfo(PSP_NET_APCTL_INFO_IP, &apctlInfo) == 0 ? 
      Qtrue : 
      Qfalse;
}

/*
  Returns the wlan access points in an array of arrays.
  It returns something in the following form :
    [
      ["ip", "name"], # Access point at 1
      ["ip", "name"], # Access point at 2
      ["ip", "name"], # Access point at 3
      ...
    ]
*/
VALUE Wlan_configs(VALUE self) {
   VALUE ret = rb_ary_new();

   for (int i = 1; i < 20; ++i) {
      if (sceUtilityCheckNetParam(i))
          break;

      VALUE entry = rb_ary_new();

      netData data;
      sceUtilityGetNetParam(i, PSP_NETPARAM_IP, &data);
      rb_ary_push(entry, rb_str_new2(data.asString));

      sceUtilityGetNetParam(i, PSP_NETPARAM_NAME, &data);
      rb_ary_push(entry, rb_str_new2(data.asString));

      rb_ary_push(ret, entry);
   }

   return ret;
}

/*
  Returns our IP.
*/
VALUE Wlan_ip(VALUE self) {
   union SceNetApctlInfo apctlInfo;
   sceNetApctlGetInfo(PSP_NET_APCTL_INFO_IP, &apctlInfo);

   return rb_str_new2(apctlInfo.ip);
}

/*
  Document-class: Joyau::Wlan

  Ruby's stdlib includes a Sock class, as well as classes easying
  the use of HTTP and FTP protocols. These classes can be used,
  but the PSP requires some initialisation before being able to use them.

  This module contains the basic methods which can allow you to use
  theme. This means :
  
  * Loading net modules
  * Connecting to one of the access points
  * Checking the access points
  
  Once you're connected, everything is transparent: you can use sockets as
  usual.
*/
void defineWlan() {
   VALUE joyau = JOYAU_MOD;
   rb_eTimeoutError = rb_define_class_under(joyau, "TimeoutError", 
					    rb_eStandardError);

   VALUE mWlan = defModule("Wlan");
   defModFunc(mWlan, "init", Wlan_init, 0);
   defModFunc(mWlan, "stop", Wlan_stop, 0);
   defModFunc(mWlan, "connect", Wlan_connect, 2);
   defModFunc(mWlan, "disconnect", Wlan_disconnect, 0);
   defModFunc(mWlan, "button_enabled?", Wlan_button_enabled, 0);
   defModFunc(mWlan, "configs", Wlan_configs, 0);
   defModFunc(mWlan, "connected?", Wlan_is_connected, 0);
   defModFunc(mWlan, "ip", Wlan_ip, 0);
}
