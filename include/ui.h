#ifndef UI_H
#define UI_H

#include "defines.h"

typedef uint8_t color_t;

void term_init();
void term_deinit();
uint8_t term_getch();
void term_clear();
void term_moveCursor(short x, short y);
uint8_t term_createColorPair(color_t foreground, color_t background);
void term_enableColorPair(uint8_t colorPairIndex);
void term_resetColorPair();
void term_getTermSize(short *x, short *y);

#endif
