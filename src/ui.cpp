// Windows-specific includes for interacing with CMD
#include <conio.h>
#include <windows.h>

#include <cstdint>
#include <cstdarg>
#include <iostream>

#include "../include/keycodes.h"
#include "../include/defines.h"
#include "../include/ui.h"

using namespace std;

color_t colorPairs[2][16];
uint8_t colorPairQuantity;

void term_init()
{
	colorPairQuantity = 0;

	term_clear();

	// set cursor to box

	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
	cursorInfo.dwSize = 100;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

	// Initialise all color pairs to white text on black background (default)

	for (uint8_t i = 0; i<16; ++i)
	{
		colorPairs[FOREGROUND_INDEX][i] = WHITE;
		colorPairs[BACKGROUND_INDEX][i] = BLACK;
	}
}

void term_deinit()
{
	term_resetColorPair();
	term_clear();
	term_moveCursor(0,0);
}

uint8_t term_getch()
{
	uint8_t input = _getch();

	if (input == 224) // handling arrows
	{
		input = _getch();
		switch (input)
		{
			case 72:
			{
				input = KEY_ARROW_UP;
				break;
			}

			case 80:
			{
				input = KEY_ARROW_DOWN;
				break;
			}

			case 77:
			{
				input = KEY_ARROW_RIGHT;
				break;
			}

			case 75:
			{
				input = KEY_ARROW_LEFT;
				break;
			}
		}
	}

	return input;
}

void term_moveCursor(short x, short y)
{
	COORD size = {x, y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), size);
}

uint8_t term_createColorPair(color_t foreground, color_t background)
{
	if (colorPairQuantity < 255)
	{
		colorPairs[FOREGROUND_INDEX][colorPairQuantity] = foreground;
		colorPairs[BACKGROUND_INDEX][colorPairQuantity] = background;
	}

	return colorPairQuantity++;
}

void term_enableColorPair(uint8_t colorPairsIndex)
{
	// Check if trying to access an uninitialised color pair
	if (colorPairsIndex >= colorPairQuantity)
	{
		return;
	}

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorPairs[FOREGROUND_INDEX][colorPairsIndex] + (colorPairs[BACKGROUND_INDEX][colorPairsIndex]<<4));
}

void term_resetColorPair()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE + (BLACK<<4));
}

void term_getTermSize(short& x, short& y)
{
	CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &bufferInfo);
	x = bufferInfo.srWindow.Right - bufferInfo.srWindow.Left+1;
	y = bufferInfo.srWindow.Bottom - bufferInfo.srWindow.Top+1;
}

void term_setCursorVisibility(bool visible)
{
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
	cursorInfo.bVisible = visible;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void term_clear()
{
	short term_maxx, term_maxy;
	term_getTermSize(term_maxx, term_maxy);
	term_resetColorPair();
	term_moveCursor(0,0);

	string fullLine = "";

	for (int x = 0; x<term_maxx; ++x)
		fullLine += ' ';

	for (int y = 0; y<term_maxy; ++y)
	{
		term_moveCursor(0, y);
		cout << fullLine;
	}
	term_moveCursor(0,0);
}
