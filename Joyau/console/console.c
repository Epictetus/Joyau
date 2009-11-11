// ********************
// * ConsoleLib        *
// *  Version 0.4       *
// *   by Geecko, 2009   *
// ************************

// For features requests, mail me to :
// geecko.dev@free.fr

#include <pspkernel.h>
#include <pspctrl.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspgu.h>
#include <time.h>
#include <math.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "./librairies/danzeff.h"
#include "./librairies/intraFont.h"
#include "console.h"

#define DEBUG 0

// Callbacks
int exit_callback(int arg1, int arg2, void *common) {
  sceKernelExitGame();
  return 0; }
int CallbackThread(SceSize args, void *argp) {
  int cbid;
  cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
  sceKernelRegisterExitCallback(cbid);
  sceKernelSleepThreadCB();
  return 0; }
void SetupCallbacks() {
  int thid = 0;
  thid = sceKernelCreateThread("update_thread",CallbackThread,0x11,0xFA0,0,0);
  if(thid >= 0) sceKernelStartThread(thid, 0, 0); }

// Enumerations
enum { OPENING, OPENED, CLOSING, CLOSED };
enum { NOTHING, INPUT, WAITKEY };

// Structures
typedef struct
  {
  char* p; // Pointer to the text
  short len[LINES_PRINT_MAX]; // Length of each line
  short height; // in pixels
  short alpha; // 0-255
  short alpha_state;
  Color color;
  } ConsoleString;

// Defines
#define NBR_STRINGS 100
#define TEXT_WIDTH 1.00f
#define TEXT_COLOR 0xFFFFFFFF
#define BORDER_WIDTH 5
#define Y_START 12
#define LINE_GAP 15
#define SCROLL_SPEED 5
#define MAIN_FADE_SPEED 5
#define DANZEFF_SPEED 9
#define FIXED_WIDTH 9

// Macros
#define COUNTSTRINGS()\
  short i;\
  for (i=0; i!=NBR_STRINGS; i++) if (console[i].p==NULL) break;

// Variables
SceCtrlData pad, oldPad;
intraFont* font;
SceUID mainthread = 0;
ConsoleString console[NBR_STRINGS];
clock_t old_clock;

int   y, main_scroll=0, look_scroll=0;
short fade_alpha=0xFF, wstate=NOTHING, state=OPENING,
      console_init=0, sfx=1, fps=60,
      m_scroll=0, fixed_state=0, cursor=0;
Color text_color=TEXT_COLOR;

// Prototypes for private functions
void consoleStuff();
void consoleRender();
int consoleThread(SceSize args, void *argp);
void consoleComputeString(short nbrstring);
void consoleFreeLine(char number);


/*
* Functions
*/


void consoleInit(short mode)
  {
  if (console_init) return;
  // Invalid parameter
  if (mode<FULL || mode>LITE) return;
  // Controls
  sceCtrlSetSamplingCycle(0);
  sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
  // Danzeff Keyboard
  if (mode == FULL)
    {
    danzeff_load();
    if (!danzeff_isinitialized())
      {
      pspDebugScreenInit();
      pspDebugScreenPrintf("Danzeff load failed !\n\n");
      pspDebugScreenPrintf("Make sure that Danzeff's images are placed\nin the ./graphics folder.\n\n");
      sceKernelDelayThread(5*1000*1000);
      pspDebugScreenPrintf("Exit in 10 seconds.");
      sceKernelDelayThread(10*1000*1000);
      sceKernelExitGame();
      }
    }
  // intraFont
  intraFontInit();
  font = intraFontLoad("flash0:/font/ltn8.pgf",INTRAFONT_CACHE_MED);
  intraFontSetEncoding(font,INTRAFONT_STRING_UTF8);
  // Graphics lib
  initGraphics();
  // Callbacks
  SetupCallbacks();
  // Sets init mode  
  console_init = mode;
  // Inits the array
  short i;
  for (i=0; i!=NBR_STRINGS; i++)
    {
    console[i].p = NULL;
    memset(console[i].len,0,LINES_PRINT_MAX*sizeof(short));
    console[i].height = 0;
    console[i].alpha = 0;
    console[i].alpha_state = CLOSED;
    console[i].color = TEXT_COLOR;
    }
  // Inits the time
  old_clock = clock();
  // Starts the main thread
  mainthread = sceKernelCreateThread("console_thread",consoleThread,0x11,0xFA0,0,NULL);
  if (mainthread>0) sceKernelStartThread(mainthread,0,NULL);
  }


short fpscount=0, danzeff_angle=0;
void consoleStuff()
  {
  if (!console_init) return;

  int console_height=0, it;
  for (it=0; it!=NBR_STRINGS; it++) console_height += console[it].height;

  // Scrolling stuffs
  if (look_scroll > 0 ) look_scroll = 0;
  else if (look_scroll < main_scroll) look_scroll = main_scroll;

  if (fabs(pad.Ly-127) > 30 && danzeff_angle != 90)
    {
    look_scroll -= (pad.Ly-127)*2*SCROLL_SPEED/127;
    m_scroll = 1;
    }
  else if (!m_scroll)
    {
    if (look_scroll > main_scroll)
      {
      look_scroll -= SCROLL_SPEED;
      if (look_scroll < main_scroll) look_scroll = main_scroll;
      }
    else if (look_scroll < main_scroll)
      {
      look_scroll += SCROLL_SPEED;
      if (look_scroll > main_scroll) look_scroll = main_scroll;
      }
    }

  // Fade Effect stuffs (main)
  if (state==OPENING)
    {
    fade_alpha -= MAIN_FADE_SPEED;
    if (fade_alpha<0) { fade_alpha=0; state=OPENED; }
    }
  else if (state==CLOSING)
    {
    fade_alpha += MAIN_FADE_SPEED;
    if (fade_alpha>255) { fade_alpha=255; state=CLOSED; }
    }

  // Fade Effect stuffs (string)
  short nbr_lines;
  for (it=0; it!=NBR_STRINGS; it++)
    {
    nbr_lines = console[it].height/LINE_GAP;
    // Ameliorates visual rendering of fade effect
    if (nbr_lines<2) nbr_lines = 2;
    if (console[it].alpha_state==OPENING)
      {
      console[it].alpha += 255/(LINE_GAP/SCROLL_SPEED)/nbr_lines;
      // If the value is too small, the alpha is not increased,
      // Then set to the maximum alpha value directly.
      if (!console[it].alpha) console[it].alpha = 255; 
      if (console[it].alpha>255)
        {
        console[it].alpha=255;
        console[it].alpha_state=OPENED;
        }
      }
    }

  // Time & fps
  fpscount++;
  if (clock() > old_clock + CLOCKS_PER_SEC)
    {
    old_clock = clock();
    fps = fpscount;
    fpscount = 0;
    }

  // Danzeff moves
  if (wstate == INPUT && !consoleButton(PSP_CTRL_SELECT,PRESSED))
    {
    danzeff_angle += DANZEFF_SPEED;
    if (danzeff_angle > 90) danzeff_angle = 90;
    }
  else
    {
    danzeff_angle -= DANZEFF_SPEED;
    if (danzeff_angle < 0) danzeff_angle = 0;
    }
  danzeff_moveTo(SCREEN_WIDTH-DANZEFF_WIDTH,
                 SCREEN_HEIGHT-DANZEFF_HEIGHT*sinf(danzeff_angle*3.14159f/180));
  }

#define A_COLOR(color,alpha)\
  RGBA(R(color),G(color),B(color),alpha)
void consoleRender()
  {
  // Defines where y starts, with scroll.
  int y = Y_START + ((sfx)?look_scroll:main_scroll),
      tmp_x=-1, tmp_y=-1, it, it2, p;

  // Everything is already precomputed,
  // Just display it.
  sceGuEnable(GU_BLEND);
  for (it=0; it!=NBR_STRINGS; it++)
    {
    if (console[it].p!=NULL)
      {
      p = 0;
      // While the string is not entierely displayed..
      for (it2=0; it2!=console[it].height/LINE_GAP; it2++)
        {
        // Clipping
        if (y > -LINE_GAP && y < SCREEN_HEIGHT+LINE_GAP)
          {
          // Sets the alpha color for each string
          intraFontSetStyle(font,TEXT_WIDTH,
                            (sfx)?A_COLOR(console[it].color,console[it].alpha)
                                 :A_COLOR(console[it].color,0xFF),0,
                            ((fixed_state)?INTRAFONT_WIDTH_FIX|FIXED_WIDTH:0));
          // Then displays the line (p=start, len=length)
          intraFontPrintEx(font,BORDER_WIDTH,y,console[it].p+p,console[it].len[it2]);
          // If we are at the last line, with input mode activated,
          // Draw the blinking '|' (2x per second)
          if (wstate == INPUT && (it == NBR_STRINGS-1 || console[it+1].p == NULL) &&
              ((p <= cursor && !it2) || p < cursor) && p+console[it].len[it2] >= cursor)
            {
            tmp_x = intraFontMeasureTextEx(font,console[it].p+p,cursor-p)+BORDER_WIDTH;
            tmp_y = y-1;
            if (clock() > old_clock+0.5f*CLOCKS_PER_SEC)
              intraFontPrint(font,tmp_x,tmp_y,"|");
            }
          }
        // Line-break
        y += LINE_GAP;
        // Increments the pointer value to display next part of the initial string input
        p += console[it].len[it2];
        }
      }
    else break;
    }

  // Draws the scrolling bar
  int console_height=0, bar_height, bar_y;
  for (it=0; it!=NBR_STRINGS; it++) console_height += console[it].height;
  if (console_height > 272 && sfx)
    {
    bar_height = SCREEN_HEIGHT*(float)SCREEN_HEIGHT/console_height;
    bar_y = SCREEN_HEIGHT-bar_height-(look_scroll-main_scroll)*SCREEN_HEIGHT/console_height;
    drawLineScreen(0xFF8F8F8F,0,bar_y,0,bar_height+bar_y);
    if (bar_height >= 3)
      {
      drawLineScreen(0xFF4F4F4F,1,bar_y+1,1,bar_height+bar_y-1);
      }
    }

  if (DEBUG)
    {
    intraFontSetStyle(font,0.9f,0xFF0000FF,0xFF000000,INTRAFONT_ALIGN_RIGHT);
    intraFontPrintf(font,SCREEN_WIDTH-BORDER_WIDTH,Y_START,"%d fps",fps);
    }

  // Draws mask for fade effect
  if (fade_alpha) fillScreenRect(RGBA(0,0,0,fade_alpha),0,0,SCREEN_WIDTH,SCREEN_HEIGHT);
  }

int consoleThread(SceSize args, void *argp)
  {
  while (1)
    {
    if (console_init)
      {
      // Controls
      oldPad = pad;
      sceCtrlPeekBufferPositive(&pad,1);
      // Other stuffs
      consoleStuff();
      // Display
      guStart();
      clearScreen(0xFF000000);
      consoleRender();
      if (danzeff_angle) danzeff_render();
      sceGuFinish();
      sceGuSync(0,0);
      if (fps >= 58) sceDisplayWaitVblankStart();
      flipScreen();
      }
    }
  return 0;
  }


void consoleExit()
  {
  if (!console_init) return;
  state = CLOSING;
  // Waits until the thread has finished the fade effect
  while (state != CLOSED) sceKernelDelayThread(1000);
  // Unload all...
  if (console_init==FULL) danzeff_free();
  short i;
  for (i=0; i!=NBR_STRINGS; i++) free(console[i].p);
  disableGraphics();
  intraFontUnload(font);
  // ... to exit.
  sceKernelDeleteThread(mainthread);
  sceKernelExitGame();
  }


short consoleSwitchSFX()
  {
  look_scroll = main_scroll;
  sfx = !sfx;
  return sfx;
  }


// This function defines the height of the string,
// The length of each line,
// And the scroll.
void consoleComputeString(short nbrstring)
  {
  if (console[nbrstring].p == NULL) return;

  short p=0, len;
  int console_height=0;

  // Resets the length and the height of the string.
  memset(console[nbrstring].len,0,LINES_PRINT_MAX*sizeof(short));
  console[nbrstring].height = 0;

  while (p < strlen(console[nbrstring].p))
    {
    len = 0;
    intraFontSetStyle(font,TEXT_WIDTH,0,0,
                      ((fixed_state)?INTRAFONT_WIDTH_FIX|FIXED_WIDTH:0));
    // Increases the string length displayed until the line reaches entire string length
    // (Approximative value)
    while (intraFontMeasureTextEx(font,console[nbrstring].p+p,len+1) < SCREEN_WIDTH-BORDER_WIDTH*2
      && (p+len) < strlen(console[nbrstring].p))
      {
      // Explanation: we need more speed when a lot of lines are printed.
      len += LINES_PRINT_MAX*2;
      // The font var is used both by the 2 threads, force the width :
      // Can be changed in by the other thread
      intraFontSetStyle(font,TEXT_WIDTH,0,0,
                        ((fixed_state)?INTRAFONT_WIDTH_FIX|FIXED_WIDTH:0));
      }
    // Then, find the real value (*big* speed gain)
    while (intraFontMeasureTextEx(font,console[nbrstring].p+p,len) >= SCREEN_WIDTH-BORDER_WIDTH*2
      && len > 0)
      {
      len--;
      intraFontSetStyle(font,TEXT_WIDTH,0,0,
                        ((fixed_state)?INTRAFONT_WIDTH_FIX|FIXED_WIDTH:0));
      }
    // Save computed values
    console[nbrstring].len[console[nbrstring].height/LINE_GAP] = len;
    console[nbrstring].height += LINE_GAP;
    // Increments pointer value
    p += len;
    }
  if (!strlen(console[nbrstring].p)) console[nbrstring].height = LINE_GAP;

  // Defines total height of the console array.
  short i;
  for (i=0; i!=NBR_STRINGS; i++) console_height += console[i].height;

  // Scrolling stuffs
  if (Y_START+console_height > SCREEN_HEIGHT)
    {
    while (Y_START+console_height+main_scroll > SCREEN_HEIGHT+LINE_GAP) main_scroll -= LINE_GAP;
    while (Y_START+console_height+main_scroll < SCREEN_HEIGHT) main_scroll += LINE_GAP;
    }
  else main_scroll = 0;
  }


// Free all the array
// So, there is a blank screen.
void consoleFreeTextArray()
  {
  short i;
  for (i=0; i!=NBR_STRINGS; i++)
    {
    free(console[i].p);
    console[i].p = NULL;
    memset(console[i].len,0,LINES_PRINT_MAX*sizeof(short));
    console[i].height = 0;
    console[i].alpha = 0;
    console[i].alpha_state = CLOSED;
    }
  // Reinits the view
  main_scroll = 0;
  look_scroll = 0;
  }


// Make the lines scroll to free some space, deleting the last line (#0)
// This is NOT a usual function.
void consoleFreeLine(char number)
  {
  if (!console_init) return;
  short lines_breaked = console[0].height/LINE_GAP-1, i;
  // Scrolling effect fix
  look_scroll += LINE_GAP*lines_breaked;
  // Removes the #1 line (#0 console array item)
  free(console[0].p);
  // Moves up all lines
  for (i=0; i!=NBR_STRINGS-1; i++)
    {
    console[i].p = console[i+1].p;
    memcpy(console[i].len,console[i+1].len,LINES_PRINT_MAX*sizeof(short));
    console[i].height = console[i+1].height;
    console[i].alpha = console[i+1].alpha;
    console[i].alpha_state = console[i+1].alpha_state;
    }
  // Deletes last line
  console[i].p = NULL;
  memset(console[i].len,0,LINES_PRINT_MAX*sizeof(short));
  console[i].height = 0;
  console[i].alpha = 0;
  console[i].alpha_state = CLOSED;
  // Scrolling
  look_scroll += LINE_GAP;
  if (number!=1) consoleFreeLine(number-1); // Recursive function, not needed
  }


char *pointer, *tmpstring;
void consolePrintText(short mode, const char* text)
  {
  if (!console_init) return;
  if (mode < ADD || mode > CAT) return;
  m_scroll = 0;
  // A copy of the string is needed :
  // This function can modify it
  tmpstring = calloc(strlen(text)+1,sizeof(char));
  strcpy(tmpstring,text);
  
  // Get indice of the last string
  COUNTSTRINGS() // i = nbrlines

  if (mode == ADD)
    {
    if (i == NBR_STRINGS) { consoleFreeLine(1); i--; }
    // Change color
    console[i].color = text_color;
    // Find the '\n' and replace it
    pointer = strchr(tmpstring,'\n');
    if (pointer != NULL) *pointer = '\0';

    // Copy the string to the console array
    console[i].p = calloc(strlen(tmpstring)+1,sizeof(char));
    strcpy(console[i].p,tmpstring);
    console[i].alpha_state = OPENING;
    // Free the tmp string
    free(tmpstring); // not set to NULL
    // Compute it
    consoleComputeString(i);
    // If the height of the new string is bigger than LINES_PRINT_MAX
    if (console[i].height/LINE_GAP > LINES_PRINT_MAX)
      consolePrintText(REPLACE,"Text too long !");
    if (pointer != NULL)
      {
      // For the scroll 
      if (sfx) sceKernelDelayThread(50000*console[i].height/LINE_GAP);
      // Recall the function with another part of string
      // (after the '\0', which was '\n')
      consolePrintText(ADD,text+(int)(pointer-tmpstring)+1);
      }
    }

  else if (mode==REPLACE)
    {
    if (!i) { i=1; console[i-1].alpha_state=OPENING; }
    // Change color
    console[i].color = text_color;
    // Find the '\n' and delete it
    pointer = strchr(tmpstring,'\n');
    if (pointer != NULL) *pointer = '\0';
    // Resize the string
    console[i-1].p = realloc(console[i-1].p,(strlen(tmpstring)+1)*sizeof(char));
    // Copy the string
    strcpy(console[i-1].p,tmpstring);
    free(tmpstring);
    // Compute it
    consoleComputeString(i-1);
    if (console[i-1].height/LINE_GAP > LINES_PRINT_MAX)
      consolePrintText(REPLACE,"Text too long !");
    }

  else if (mode==CAT)
    {
    if (!i)
      {
      i = 1;
      console[i-1].alpha_state = OPENING;
      console[i-1].p = malloc(sizeof(char));
      *console[i-1].p = '\0';
      }
    pointer = strchr(tmpstring,'\n');
    if (pointer != NULL) *pointer = '\0';
    console[i-1].p = realloc(console[i-1].p,
                             (strlen(tmpstring)+strlen(console[i-1].p)+1)
                             *sizeof(char));
    // Copy the string at the end of the last char
    strcpy(strchr(console[i-1].p,'\0'),tmpstring);
    free(tmpstring);
    consoleComputeString(i-1);
    if (console[i-1].height/LINE_GAP > LINES_PRINT_MAX)
      consolePrintText(REPLACE,"Text too long !");
    // Here again, no support for '\n'.
    }
  }


void consolePrintfText(short mode, const char* text, ...)
  {
  char buffer[256];
  va_list ap;  
  va_start(ap, text);
  vsnprintf(buffer, 256, text, ap);
  va_end(ap);
  buffer[255] = '\0';
  consolePrintText(mode,buffer);
  }


const char* consoleInputText(short mode)
  {
  if (console_init != FULL) return NULL;
  if (mode<LETTERS || mode>NUMBERS) return NULL;
  // Lets create a new string.
  consolePrintText(ADD,"\0");
  // Letters or numbers.
  danzeff_set_disposition(mode);
  wstate = INPUT;
  m_scroll = 0;
  COUNTSTRINGS()
  cursor = 0;
  short pointer;
  int i2;
  char c;
  while (wstate == INPUT)
    {
    sceKernelDelayThread(1000);
    c = danzeff_readInput(pad);
    switch (c)
      {
      // Key Blacklist start
      case ' ':
      case 0:
      case DANZEFF_SELECT:
      break;
      // End
      case DANZEFF_LEFT:
        if (cursor > 0) cursor--;
      break;
      case DANZEFF_RIGHT:
        if (console[i-1].p[cursor] != '\0') cursor++;
      break;
      case DANZEFF_START:
        if (!strlen(console[i-1].p)) break;
        wstate = NOTHING;
      break;
      case '\10':
        // The "suppr" button was pressed :
        if (cursor>0) cursor--;
        // Move following chars..
        i2 = cursor;
        while (console[i-1].p[i2] != '\0')
          {
          console[i-1].p[i2] = console[i-1].p[i2+1];
          i2++; 
          }
        // Resize the string 
        console[i-1].p = realloc(console[i-1].p,(strlen(console[i-1].p)+1)*sizeof(char));
        // Then compute on it.
        consoleComputeString(i-1);
      break;
      default:
        // Checks if the string is not too long :
        // The size of the string should not be greater than LINES_PRINT_MAX.
        // We let space for the blinking cursor.
        pointer = 0;
        // The pointer is set to the last line.
        for (i2=0; i2!=(console[i-1].height/LINE_GAP)-1; i2++)
          pointer += console[i-1].len[i2];
        font->size = TEXT_WIDTH;
        // If a new char can be printed.. 
        if (!(intraFontMeasureText(font,console[i-1].p+pointer)
           > SCREEN_WIDTH-BORDER_WIDTH*2-FIXED_WIDTH*2
           && console[i-1].height/LINE_GAP==LINES_PRINT_MAX))
          {
          // Resizes string..
          console[i-1].p = realloc(console[i-1].p,(strlen(console[i-1].p)+2)*sizeof(char));
          // Move following chars..
          i2 = strchr(console[i-1].p,'\0')-console[i-1].p;
          while (i2 != cursor-1)
            {
            console[i-1].p[i2+1] = console[i-1].p[i2];
            i2--; 
            }
          // Then put it.
          console[i-1].p[cursor] = c;
          cursor++;
          // Then compute on it.
          consoleComputeString(i-1);
          }
      break;
      }
    }
  // Returns the string. 
  return console[i-1].p;
  }


unsigned int consoleButton(unsigned int button, short mode)
  {
  if (!console_init) return 0;
  unsigned int tmp=0;
  if      (mode==0) tmp = pad.Buttons & button && 1;
  else if (mode==1) tmp = pad.Buttons & button && !(oldPad.Buttons & button);
  else if (mode==2) tmp = !(pad.Buttons & button) && oldPad.Buttons & button;
  else if (mode==3) tmp = pad.Buttons & button && oldPad.Buttons & button; 
  return tmp;
  }


unsigned int consoleWaitButton(unsigned int button, const char* text)
  {
  if (!console_init) return 0;
  if (text!=NULL) consolePrintText(ADD,(char*)text);
  wstate = WAITKEY;
  m_scroll = 0;
  oldPad = pad;
  while (1)
    {
    sceKernelDelayThread(1000);
    sceCtrlPeekBufferPositive(&pad,1);
    if (pad.Buttons != oldPad.Buttons)
      {
      // If pressed keys have changed
      if (((oldPad.Buttons ^ pad.Buttons) & pad.Buttons) != 0)
        {
        if (!button) break;
        else if (pad.Buttons & button) break;
        }
      else oldPad = pad;
      }
    }
  m_scroll = 0;
  return pad.Buttons;
  }


void consoleWait(float delay)
  {
  if (!console_init) return;
  sceKernelDelayThread(fabs(delay)*1000000);
  }


void consoleSetTextColor(Color color)
  {
  if (color > 0xFFFFFF || color < 0x000000) return;
  text_color = RGBA(B(color),G(color),R(color),0xFF);
  }


void consoleSwitchFixedMode()
  {
  // Switch the state
  fixed_state = !fixed_state;
  // Then, recompute the array
  short i;
  for (i=0; i!=NBR_STRINGS; i++)
    consoleComputeString(i);
  // Reset the view
  look_scroll = main_scroll;
  }

// EOF
