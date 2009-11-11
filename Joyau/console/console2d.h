#ifndef CONSOLE_2D_H
#define CONSOLE_2D_H

#include "console.h"

// Defines
#define CONSOLE_WIDTH 52
#define CONSOLE_HEIGHT 18

// Enumerations
enum { L, R };
enum { H, V };

// Functions

/**
 * Inits the 2D library.
 * Calls consoleInit();
 */
void console2dInit();

/**
 * Exits the 2D library.
 * Calls consoleExit();
 */
void console2dExit();

/**
 * Prints a text to the array.
 *
 * @param x - X position on the array.
 * @param y - Y position on the array.
 * @param mode - L or R. Defines the alignment.
 * @param text - ASCII text to draw (UTF-8 accepted).
 */
void console2dPrint(short x, short y, short mode, const char* text);

/**
 * Prints a text to the array, with formatting (like printf).
 *
 * @param x - X position on the array.
 * @param y - Y position on the array.
 * @param mode - L or R. Defines the alignment.
 * @param text - ASCII text to draw (UTF-8 accepted).
 */
void console2dPrintf(short x, short y, short mode, const char* text, ...);

/**
 * Fills a line with a text.
 *
 * @param mode - H or V. Horizontal or vertical line.
 * @param i - X or Y position, depends on '@param mode'.
 * @param advance - Which char to start (typically 0). 
 * @param text - ASCII text to draw (UTF-8 accepted).
 */
void console2dFillLine(short mode, short i, short advance, const char* text);

/**
 * Sets line color.
 *
 * @param y - Y position of the line.
 * @param color - Text color.
 */
void console2dSetLineColor(short y, Color color);

/**
 * Sets array color.
 *
 * @param color - Text color.
 */
void console2dSetColor(Color color);

/**
 * Clears the array.
 */
void console2dClear();

/**
 * Prints the array to the console.
 */
void console2dFlip();

/**
 * Switches automatic flip when a print function is called.
 */
void console2dSwitchAutoFlip();

#endif
