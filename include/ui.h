#ifndef UI_H
#define UI_H

#include "defines.h"
#include "keycodes.h"

typedef uint8_t color_t;

void term_init();
void term_deinit();
uint8_t term_getch(); // Get input from user without delay
void term_clear(); // Clear the terminal window
void term_moveCursor(short x, short y); // Move cursor to x and y position specified
uint8_t term_createColorPair(color_t foreground, color_t background); // Create color pair with foreground (text) and background colors specified
void term_enableColorPair(uint8_t colorPairIndex); // Activate color pair with id specified
void term_resetColorPair(); // Set color pair to white text on black background
void term_getTermSize(short& x, short& y); // Get terminal size

#endif
