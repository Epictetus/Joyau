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

#include "StdInclude.hpp"
#include "Sprite.hpp"
#include "Audio.hpp"
#include "Graphics.hpp"
#include "Keys.hpp"
#include "Kernel.hpp"
#include "MessageBox.hpp"
#include "Scrolling.hpp"
#include "Particles.hpp"
#include "GameMap.hpp"
#include "CircleMenu.hpp"
#include "Timer.hpp"
#include "Drawable.hpp"
#include "DrawableRect.hpp"
#include "Circle.hpp"
#include "Line.hpp"
#include "Triangle.hpp"
#include "DrawableText.hpp"
#include "Shape.hpp"
#include "Usb.hpp"
#include "RubyDrawable.hpp"
#include "MultiSelecter.hpp"
#include "Font.hpp"
#include "Console.hpp"

PSP_MODULE_INFO("Joyau", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);
PSP_HEAP_SIZE_KB(-256);

int exit_callback(int arg1, int arg2, void* commons)
{
   sceKernelExitGame();
   return 0;
}

int CallbackThread(SceSize args, void *argp)
{
   int cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
   sceKernelRegisterExitCallback(cbid);
   sceKernelSleepThreadCB();
   return 0;
}

int SetupCallbacks(void)
{
   int thid = sceKernelCreateThread("update_thread", CallbackThread,
                                    0x11, 0xFA0, THREAD_ATTR_USER, 0);
   if(thid >= 0)
      sceKernelStartThread(thid, 0, 0);
   return thid;
}

VALUE debug(VALUE self, VALUE text)
{
   oslDebug(StringValuePtr(text));
   return Qnil;
}

VALUE Joyau_puts(int argc, VALUE *argv, VALUE self)
{
   for (int i = 0; i < argc; ++i)
   {
      pspDebugScreenPrintf("%s\n", StringValuePtr(argv[i]));
      std::cout << StringValuePtr(argv[i]) << std::endl;
   }
   return Qnil;
}

VALUE Joyau_exit(VALUE self)
{
   sceKernelExitGame();
   return Qnil;
}

int main(int argc, char** argv)
{
   pspDebugScreenInit();
   SetupCallbacks();

   ruby_init();

   // We add our include path, in order to allow extension.
   ruby_incpush((std::string("./ruby/") + JOYAU_RB_VERSION).c_str());
   ruby_incpush("./ruby/site_ruby");
   ruby_incpush((std::string("./ruby/site_ruby/") + JOYAU_RB_VERSION).c_str());

   // We'll also allow include at memory stick's root
   ruby_incpush((std::string("ms0:/ruby/") + JOYAU_RB_VERSION).c_str());
   ruby_incpush("ms0:/ruby/site_ruby");
   ruby_incpush((std::string("ms0:/ruby/site_ruby/") + 
		 JOYAU_RB_VERSION).c_str());

   ruby_incpush("./");
   
   VALUE joyau = rb_define_module("Joyau");

   defineManager();
   defineDrawable();
   defineShape();
   defineManager();
   defineSprite();
   defineAudio();
   defineGraphics();
   defineKeys();
   defineKernel();
   defineMessageBox();
   defineScrolling();
   defineParticles();
   defineGameMap();
   defineCircleMenu();
   defineTimer();
   defineDrawableRect();
   defineCircle();
   defineLine();
   defineTriangle();
   defineDrawableText();
   defineUsb();
   defineMsgSelecter();
   defineMultiSelecter();
   defineIntrafont();
   defineConsole();

   Manager::getInstance().setArg(argc, argv);
   Pad::getInstance();

   defModFunc(joyau, "debug", debug, 1);
   defModFunc(joyau, "puts", Joyau_puts, -1); // puts redefined for the psp
   defModFunc(joyau, "exitGame", Joyau_exit, 0);
   
   ruby_init_loadpath();
   ruby_script("joyau");

   try {
      runScript("./script.rb");
   }
   catch (...) { // An error occured from Ruby/
      pspDebugScreenInit();
      pspDebugScreenSetTextColor(RGB(255, 0, 0));
      pspDebugScreenPrintf("Joyau - error manager\n\n\n");
      pspDebugScreenSetTextColor(RGB(255, 255, 255));
      
      pspDebugScreenPrintf("This is Joyau's error manager.An exception ");
      pspDebugScreenPrintf("has been thrown, and the script ");
      pspDebugScreenPrintf("cannot be run. If you're the developper of");
      pspDebugScreenPrintf(" this application,\ncheck yout script.");
      pspDebugScreenPrintf(" If it seems fine to you, consider ");
      pspDebugScreenPrintf("filling a bug report.\n");
      pspDebugScreenPrintf("If you're not the developper, consider");
      pspDebugScreenPrintf(" contacting him.\n\n\n");

      pspDebugScreenSetTextColor(RGB(255, 0, 0));
      pspDebugScreenPrintf("Errors informations :\n\n\n");
      pspDebugScreenSetTextColor(RGB(255, 255, 255));

      // Now, we get the error.
      VALUE error = rb_gv_get("$!");
      
      // And the information we want to have about it.
      VALUE backtrace = rb_funcall(error, getFunc("backtrace"), 0);
      VALUE type      = rb_funcall(error, getFunc("class"), 0);
      VALUE msg       = rb_funcall(error, getFunc("message"), 0);

      type = rb_funcall(type, getFunc("to_s"), 0); // We need a String

      VALUE first = rb_ary_entry(backtrace, 0);

      // It should be printed in a file too.
      std::ofstream errorFile("joyau-error.log");

      pspDebugScreenPrintf("%s: %s: %s\n", StringValuePtr(first),
			                   StringValuePtr(type),
			                   StringValuePtr(msg));
      errorFile << StringValuePtr(first) << ": ";
      errorFile << StringValuePtr(type)  << ": ";
      errorFile << StringValuePtr(msg)   << "\n";

      for (int i = 1; i < RARRAY_LEN(backtrace); ++i) {
	 VALUE entry = rb_ary_entry(backtrace, i);
	 pspDebugScreenPrintf("%s\n", StringValuePtr(entry));
	 errorFile << StringValuePtr(entry) << "\n";
      }
      errorFile.close();

      pspDebugScreenPrintf("\nThese informations can be found in ");
      pspDebugScreenPrintf("joyau-error.log\n\n");

      pspDebugScreenPrintf("Press X in order to quit.");
      
      Pad &pad = Pad::getInstance();
      pad.update();
      while (!pad.pressed(PSP_CTRL_CROSS))
	 pad.update();

      sceKernelExitGame();
      return 0;
   }

   Manager::deleteInstance();
   Pad::deleteInstance();
   return 0;
}
