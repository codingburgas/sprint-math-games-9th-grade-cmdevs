#include <iostream>
#include <vector>
#include <cstdint>

#include "../include/ui.h"
#include "../include/wordparser.h"
#include "../include/defines.h"

using namespace std;

int main()
{
	bool fileFound;

	uint8_t input, x = 0, y = 0;

	int points = 0, pointsToWin; // define pointsToWin

	char playfield[15][15];

	for (int x = 0; x<15; ++x)
	{
		for (int y = 0; y<15; ++y)
		{
			playfield[x][y] = ' ';
		}
	}

	vector<string> wordList = parse("words", fileFound);
	if (!fileFound)
	{
		cout << "The word list file wasn't found, exiting...";
	}

	term_init();

	term_moveCursor(COORDSX(0), COORDSY(0));

	while ((input = term_getch())!=CONTROL('C'))
	{
		switch (input)
		{
			case KEY_ARROW_UP:
			{
				if (y>0) --y;
				break;
			}
			case KEY_ARROW_DOWN:
			{
				if (y<14) ++y;
				break;
			}
			case KEY_ARROW_LEFT:
			{
				if (x>0) --x;
				break;
			}
			case KEY_ARROW_RIGHT:
			{
				if (x<14) ++x;
				break;
			}
			case 'A'...'Z': tolower(input);
			case 'a'...'z':
			{
				playfield[x][y] = input;
				// do word check
				break;
			}
			default: break;
		}
		
		term_moveCursor(COORDSX(x), COORDSY(y));
	}

	term_deinit();
}
