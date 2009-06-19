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
#include "Usb.hpp"

PSP_MODULE_INFO("Joyau", 0x1000, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_VFPU);

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

void exceptionHandler(PspDebugRegBlock *regs)
{
   pspDebugScreenInit();
   pspDebugScreenSetBackColor(0x00FF0000);
   pspDebugScreenSetTextColor(0xFFFFFFFF);
   pspDebugScreenClear();
   pspDebugScreenPrintf("Exception Details:");
   pspDebugDumpException(regs);
}

__attribute__((constructor)) void stdoutInit()
{
   pspKernelSetKernelPC();
   pspDebugInstallStdoutHandler(pspDebugScreenPrintData);
   pspDebugInstallStderrHandler(pspDebugScreenPrintData);
   pspDebugInstallErrorHandler(exceptionHandler);
}

int main(int argc, char** argv)
{
   const char* scriptName = "script.rb";
   char* scriptFilename = new char[strlen(argv[0]) + strlen(scriptName)];
   strcpy(scriptFilename, argv[0]);
   char* end = strrchr(scriptFilename, '/');
   end++;
   *end = 0;
   strcat(scriptFilename, scriptName);

   pspDebugScreenInit();
   SetupCallbacks();

   ruby_init();

   defineSprite();
   defineAudio();
   defineGraphics();
   defineKeys();
   defineKernel();
   defineUsb();

   ruby_init_loadpath();
   ruby_script("embeded");

   rb_load_file(scriptFilename);
   ruby_run();

   Manager::deleteInstance();

   return 0;
}

