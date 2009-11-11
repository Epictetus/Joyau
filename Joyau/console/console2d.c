// ******************
// * ConsoleLib 2D   *
// *  by Geecko, 2009 *
// *********************

// For features requests, mail me to :
// geecko.dev@free.fr

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <malloc.h>
#include "console2d.h"

// Defines
#define TEXT_COLOR 0xFFFFFF

// Variables
char array[CONSOLE_HEIGHT][CONSOLE_WIDTH+1];
Color c_array[CONSOLE_HEIGHT];
short console2d_init=0, auto_flip=0;


/*
* Functions
*/


void console2dInit()
  {
  consoleInit(LITE);
  consoleSwitchSFX();
  consoleSwitchFixedMode();
  // Sets the array to 0.
  console2dClear();
  // Inits the color array.
  console2dSetColor(TEXT_COLOR);
  console2d_init = 1;
  }


void console2dExit()
  {
  if (!console2d_init) return;
  consoleExit();
  }


// This function is based on cccStrlenUTF8().
// Needed to have only 1-byte char.
void convertUTF8toASCII(unsigned char* string)
  {
  if (string == NULL) return;

	short i=0, length = 0;
	while (string[i]!='\0')
    {
		if (string[i] <= 0x7F) // ASCII
      {
      string[length] = string[i];
      i++;
      length++;
      }
    else if (string[i] <= 0xC1) // Multi-byte
      {
      // Ignore
      i++;
      }
    else if (string[i] <= 0xDF) // 2-byte
      {
      string[length] = '?';
      i += 2;
      length++;
      }
    else if (string[i] <= 0xEF) // 3-byte
      {
      string[length] = '?';
      i += 3;
      length++;
      }
    else // 4-byte or invalid
      {
      // Ignore
      i++;
      }
    }
  string[length] = '\0';
  }


char *pointer, *tmpstring;
void console2dPrint(short x, short y, short mode, const char* text)
  {
  if (!console2d_init) return;
  if (mode < L || mode > R) return;
  if (text == NULL) return;

  // A copy of the string is needed :
  // This function can modify it
  tmpstring = calloc(strlen(text)+1,sizeof(char));
  strcpy(tmpstring,text);
  // Find the '\n' and replace it
  pointer = strchr(tmpstring,'\n');
  if (pointer != NULL) *pointer = '\0';
  // Fight against the UTF-8 bug
  convertUTF8toASCII((unsigned char*)tmpstring);

  short len = strlen(tmpstring), i;

  // Clipping
  if (y >= 0 && y < CONSOLE_HEIGHT)
    {
    if (mode == L)
      {
      // To-the-right print
      for (i=0; i!=len; i++)
        {
        if (x >= 0 && x+i < CONSOLE_WIDTH)
          array[y][x+i] = tmpstring[i]; 
        }
      }
    else if (mode == R)
      {
      // To-the-left print
      for (i=0; i!=len; i++)
        {
        if (x-i >= 0 && x < CONSOLE_WIDTH)
          array[y][x-i] = tmpstring[len-1-i]; 
        }
      }
    }

  free(tmpstring);
  // Recursive function
  if (pointer != NULL)
    {
    console2dPrint(x,y+1,mode,text+(int)(pointer-tmpstring)+1);
    }
  else if (auto_flip) console2dFlip();
  }


void console2dPrintf(short x, short y, short mode, const char* text, ...)
  {
  char buffer[256];
  va_list ap;  
  va_start(ap, text);
  vsnprintf(buffer, 256, text, ap);
  va_end(ap);
  buffer[255] = '\0';
  console2dPrint(x,y,mode,buffer);
  }


void console2dFillLine(short mode, short i, short advance, const char* text)
  {
  if (!console2d_init) return;
  if (mode < H || mode > V) return;
  if (advance < 0) return;
  if (text == NULL) return;

  // A copy of the string is needed :
  // This function can modify it
  char* string = calloc(strlen(text)+1,sizeof(char));
  strcpy(string,text);
  pointer = strchr(tmpstring,'\n');
  if (pointer != NULL) *pointer = ' ';
  convertUTF8toASCII((unsigned char*)string);
  string = realloc(string,(strlen(string)+1)*sizeof(char));

  short i2=0, i3, len=strlen(string);
  while (advance >= len) advance -= len;
  i3 = advance;
  // Horizontal Mode
  if (mode == H)
    {
    if (i < 0 || i >= CONSOLE_HEIGHT) return;
    while (i2!=CONSOLE_WIDTH)
      {
      array[i][i2] = text[i3];
      i2++;
      i3++;
      // Repeat
      if (i3 >= len) i3 -= len;
      }
    }
  // Vertical mode
  else if (mode == V)
    {
    if (i < 0 || i >= CONSOLE_WIDTH) return;
    while (i2!=CONSOLE_HEIGHT)
      {
      array[i2][i] = text[i3];
      i2++;
      i3++;
      // Repeat
      if (i3 >= len) i3 -= len;
      }
    }

  if (auto_flip) console2dFlip();
  }


void console2dSetLineColor(short y, Color color)
  {
  if (!console2d_init) return;
  if (y<0 || y>=CONSOLE_HEIGHT) return;
  if (color > 0xFFFFFF || color < 0x000000) return;

  c_array[y] = color;

  if (auto_flip) console2dFlip();
  }


void console2dSetColor(Color color)
  {
  // No 'console2d_init' check.
  if (color > 0xFFFFFF || color < 0x000000)
    color = TEXT_COLOR;

  short i;
  for (i=0; i!=CONSOLE_HEIGHT; i++)
    {
    c_array[i] = color;
    }

  if (auto_flip) console2dFlip();
  }


void console2dClear()
  {
  // No 'console2d_init' check again.
  memset(array,' ',sizeof(array));
  short i;
  for (i=0; i!=CONSOLE_HEIGHT; i++)
    {
    array[i][CONSOLE_WIDTH] = '\0';
    }
  if (auto_flip) console2dFlip();
  }


void console2dFlip()
  {
  if (!console2d_init) return;

  short i;
  // First, clear screen
  consoleFreeTextArray();
  // Then, print every line with color.
  for (i=0; i!=CONSOLE_HEIGHT; i++)
    {
    consoleSetTextColor(c_array[i]);
    consolePrintText(ADD,array[i]);
    }
  }


void console2dSwitchAutoFlip()
  {
  auto_flip = !auto_flip;
  }

// EOF
