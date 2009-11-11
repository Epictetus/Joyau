#ifndef CONSOLE_H
#define CONSOLE_H

#include "./librairies/graphics.h"

// Defines
#define LINES_PRINT_MAX 5

// Enumerations
enum { FULL=1, LITE=2 };
enum { ADD, REPLACE, CAT };
enum { LETTERS, NUMBERS };
enum { PRESSED, JUST_PRESSED, JUST_RELEASED, STILL_PRESSED };

// Functions

/**
 * Inits the library, with a fade effect.
 *
 * @param mode - FULL for Danzeff, or LITE.
 */
void consoleInit(short mode);

/**
 * Deinits the library, then exits, with a fade effect.
 */
void consoleExit();

/**
 * Switches scroll and fade effect to on/off.
 *
 * @returns the actual state.
 */
short consoleSwitchSFX();

/**
 * Prints a text to the console.
 *
 * @param mode - ADD, REPLACE or CAT.
 * @param text - pointer to the text string.
 */
void consolePrintText(short mode, const char* text);

/**
 * Prints a text to the console (with formatting). Use it like printf.
 *
 * @param mode - ADD, REPLACE or CAT.
 * @param text - formatted text string.
 */
void consolePrintfText(short mode, const char* text, ...);

/**
 * Inputs a text, and returns the string.
 *
 * @param mode - LETTERS or NUMBERS (danzeff mode)
 * @returns pointer to the input string.
 */
const char* consoleInputText(short mode);

/**
 * Clears the console array.
 */
void consoleFreeTextArray();

/**
 * Checks if a button is pressed.
 *
 * @param button - for example, PSP_CTRL_CROSS
 * @param mode - PRESSED, JUST_PRESSED, JUST_RELEASED, STILL_PRESSED
 * @returns 1 if true, else 0.
 */
unsigned int consoleButton(unsigned int button, short mode);

/**
 * Waits for a button pressed.
 *
 * @param button - can be PSP_CTRL_CROSS. On 0, waits for any key input.
 * @returns an SceCtrlData var.
 */
unsigned int consoleWaitButton(unsigned int button, const char* text);

/**
 * Waits for a duration.
 *
 * @param delay - Delay to exit the library, in seconds.
 */
void consoleWait(float delay);

/**
 * Changes text color.
 *
 * @param color - Color R-G-B, like 0xFFFFFF (white)
 */
void consoleSetTextColor(Color color);

/**
 * Switches text mode.
 * Variable or fixed width (52 char).
 */
void consoleSwitchFixedMode();

#endif
