#include <iostream>
#include <vector>
#include <cstdint>
#include <cstdlib>

#include "../include/ui.h"
#include "../include/wordparser.h"
#include "../include/defines.h"

using namespace std;

// letter points from a to z
const int letterPoints[26] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

void putLetters(vector<char>& letters)
{
	// BEGIN 0 points
	for (int i = 0; i<2; ++i) letters.push_back(' '); // 2 blanks
	// END 0 points

	// BEGIN 1 points
	for (int i = 0; i<12; ++i) letters.push_back('e'); // 12 e's
	for (int i = 0; i<9; ++i)
	{
		letters.push_back('a');
		letters.push_back('i'); 
		letters.push_back('o');
	}
	letters.pop_back(); // remove last 'o'
	for (int i = 0; i<6; ++i)
	{
		letters.push_back('n');
		letters.push_back('r');
		letters.push_back('t');
	}
	for (int i = 0; i<4; ++i)
	{
		letters.push_back('l'); 
		letters.push_back('s');
		letters.push_back('u');
	}
	// END 1 points
	
	// BEGIN 2 points
	for (int i = 0; i<4; ++i)
	{
		letters.push_back('d');
		letters.push_back('g');
	}
	letters.pop_back(); // remove last 'g'
	// END 2 points
	
	// BEGIN 3 points
	for (int i = 0; i<2; ++i)
	{
		letters.push_back('b');
		letters.push_back('c');
		letters.push_back('m');
		letters.push_back('p');
	// END 3 points
	
	// BEGIN 4 points
		letters.push_back('f');
		letters.push_back('h');
		letters.push_back('v');
		letters.push_back('w');
		letters.push_back('y');
	}
	// END 4 points
	
	// BEGIN 5 points
	letters.push_back('k');
	// END 5 points
	
	// BEGIN 8 points
	letters.push_back('j');
	letters.push_back('x');
	// END 8 points

	// BEGIN 10 points
	letters.push_back('q');
	letters.push_back('z');
	// END 10 points
}

int findLetter(char letter, char letters[6])
{
	for (int i = 0; i<7; ++i)
	{
		if (letters[i]==letter) return i;
	}
	return -1;
}

int checkword(int placedx, int placedy, const char board[14][14], vector<string> wordList)
{
	int addedPoints = 0;

	// find word(s) on the current row/column
	
	char wordColumn[15], wordRow[15];
	bool valid = 0;

	// row
	for (int i = 0, offset = 0; i<15; ++i)
	{
		if (board[i][placedy]&&!offset) offset = i;
		if (offset)
		{
			while(i<15&&board[i][placedy])
			{
				if (i==placedx) valid = 1;
				wordRow[i-offset] = board[i][placedy];
				++i;
			}
			wordRow[i-offset] = 0;
			offset = 0;
			if (valid) break;
		}
	}

	// todo: word check from wordList and special tiles
	
	// column
	for (int i = 0, offset = 0; i<15; ++i)
	{
		if (board[placedx][i]&&!offset) offset = i;
		if (offset)
		{
			while(i<15&&board[placedx][i])
			{
				if (i==placedy) valid = 1;
				wordColumn[i-offset] = board[placedx][i];
				++i;
			}
			wordColumn[i-offset] = 0;
			offset = 0;
			if (valid) break;
		}
	}

	return addedPoints;
}

int main()
{
	uint8_t input, x = 0, y = 0;

	int p1Score = 0, pointsToWin; // define pointsToWin (from menu/argument)

	char playfield[14][14];
	
	vector<char> letters;
	putLetters(letters);

	// randomise letters for players
	
	char p1Letters[6];
	for (int i = 0, id; i<7; ++i)
	{
		id = getLetterId();
		p1Letters[i] = letters.at(id);
		letters.erase(letters.begin()+id);
	}

	// clear field

	for (int x = 0; x<15; ++x)
	{
		for (int y = 0; y<15; ++y)
		{
			playfield[x][y] = 0;
		}
	}

	vector<string> wordList;
	if (parse("words", wordList))
	{
		cout << "Word list not found, exiting...\n";
		return 1;
	}

	term_init();

	short term_maxx, term_maxy;

	term_getTermSize(term_maxx, term_maxy);

	int colorPairIds[11];
	colorPairIds[0] = term_createColorPair(WHITE, BLACK);
	colorPairIds[1] = term_createColorPair(RED, BLACK);
	colorPairIds[2] = term_createColorPair(YELLOW, BLACK);
	colorPairIds[3] = term_createColorPair(GREEN, BLACK);
	colorPairIds[4] = term_createColorPair(PINK, BLACK);
	colorPairIds[5] = term_createColorPair(CYAN, BLACK);
	colorPairIds[8] = term_createColorPair(BLUE, BLACK);
	colorPairIds[10] = term_createColorPair(LIGHTGREEN, BLACK);


	term_moveCursor(term_maxx/2+12, term_maxy/2-4);
	term_enableColorPair(colorPairIds[1]);
	cout << "1 point";
	term_moveCursor(term_maxx/2+12, term_maxy/2-3);
	term_enableColorPair(colorPairIds[2]);
	cout << "2 points";
	term_moveCursor(term_maxx/2+12, term_maxy/2-2);
	term_enableColorPair(colorPairIds[3]);
	cout << "3 points";
	term_moveCursor(term_maxx/2+12, term_maxy/2-1);
	term_enableColorPair(colorPairIds[4]);
	cout << "4 points";
	term_moveCursor(term_maxx/2+12, term_maxy/2);
	term_enableColorPair(colorPairIds[5]);
	cout << "5 points";
	term_moveCursor(term_maxx/2+12, term_maxy/2+1);
	term_enableColorPair(colorPairIds[8]);
	cout << "8 points";
	term_moveCursor(term_maxx/2+12, term_maxy/2+2);
	term_enableColorPair(colorPairIds[10]);
	cout << "10 points";

	// print letters for p1

	for (int i = 0; i<7; ++i)
	{
		term_moveCursor(term_maxx/2-17+i*2, term_maxy/2+10);
		COLORPAIR(p1Letters[i]);
		cout << p1Letters[i];
		term_resetColorPair();
	}

	// move cursor to top left of the playfield

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
				if (findLetter(input, p1Letters)!=-1)
				{
					playfield[x][y] = input;
					p1Score += checkword(x, y, playfield, wordList);
				}
				break;
			}
			default: break;
		}
		term_moveCursor(COORDSX(x), COORDSY(y));
	}

	term_deinit();
}
