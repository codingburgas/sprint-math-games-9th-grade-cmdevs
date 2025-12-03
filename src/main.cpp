#include <iostream>
#include <vector>
#include <cstdint>
#include <random>
#include <cstring>

#include "../include/ui.h"
#include "../include/wordparser.h"
#include "../include/defines.h"

using namespace std;

// letter points from a to z
const int letterPoints[26] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};
const int doubleWordPoints[17][2] = { {1,1}, {2,2}, {3,3}, {4,4}, {7,7}, {10,10}, {11,11}, {12,12}, {13,13}, {13,1}, {12,2}, {11,3}, {10,4}, {4,10}, {3,11}, {2,12}, {1,13} };
const int tripleWordPoints[8][2] = { {0,0}, {14,0}, {0,14}, {14,14}, {0,7}, {7,0}, {14,7}, {7,14} };
const int doubleLetterPoints[24][2] = { {3,0}, {11,0}, {6,2}, {8,2}, {0,3}, {7,3}, {14,3}, {2,6}, {6,6}, {8,6}, {12,6}, {3,7}, {11,7}, {2,8}, {6,8}, {8,8}, {12,8}, {0,11}, {7,11}, {13,11}, {6,12}, {8,12}, {3,14}, {11,14} };
const int tripleLetterPoints[12][2] = { {5,1}, {9,1}, {1,5}, {5,5}, {9,5}, {13,5}, {1,9}, {5,9}, {9,9}, {13,9}, {5,13}, {9,13} };

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

inline int containsWord(string word, char str[15], int letterindex)
{
	bool containsLetter = 0, found = 0;
	int offset;
	for (int i = 0; i+word.length()<strlen(str); ++i)
	{
		if (word[0]==str[i])
		{
			offset = i;
			found = 1;
			++i;
			for (; i<word.length(); ++i)
			{
				if (str[i]!=word[i-offset])
				{
					found = 0;
					break;
				}	
			}
			if (found)
			{
				return offset;
			}
		}
	}
	return 0;
}

int checkword(int placedx, int placedy, const char board[14][14], vector<string> wordList)
{
	int addedPoints = 0;

	// find word(s) on the current row/column
	
	char wordColumn[15], wordRow[15];
	bool valid = 0;

	int offsetx = 0, offsety = 0;

	// row
	for (int i = 0; i<15; ++i)
	{
		if (board[i][placedy]&&!offsetx) offsetx = i;
		if (offsetx)
		{
			while(i<15&&board[i][placedy])
			{
				if (i==placedx) valid = 1;
				wordRow[i-offsetx] = board[i][placedy];
				++i;
			}
			wordRow[i-offsetx] = 0;
			if (valid) break;
			offsetx = 0;
		}
	}

	// column
	for (int i = 0; i<15; ++i)
	{
		if (board[placedx][i]&&!offsety) offsety = i;
		if (offsety)
		{
			while(i<15&&board[placedx][i])
			{
				if (i==placedy) valid = 1;
				wordColumn[i-offsety] = board[placedx][i];
				++i;
			}
			wordColumn[i-offsety] = 0;
			if (valid) break;
			offsety = 0;
		}
	}

	int offset;

	for (int i = 0; i<wordList.size() && wordList[i].length()<strlen(wordRow); ++i)
	{
		if ((offset=containsWord(wordList[i], wordRow, placedx-offsetx))>0)
		{
			// base points
			for (int b = 0; b<wordList[i].length(); ++i)
				addedPoints += getLetterPoints(wordList[i][b]);

			for (int b = 0; b<wordList[i].length(); ++i)
			{
				for (int x = 0; x<24; ++x)
					if (doubleLetterPoints[x][0] == offsetx+b && doubleLetterPoints[x][1] == placedy)
						addedPoints += getLetterPoints(wordList[i][b]);

				for (int x = 0; x<12; ++x)
					if (tripleLetterPoints[x][0] == offsetx+b && tripleLetterPoints[x][1] == placedy)
						addedPoints += getLetterPoints(wordList[i][b])*2;

				for (int x = 0; x<17; ++x)
					if (doubleWordPoints[x][0] == offsetx+b && doubleWordPoints[x][1] == placedy)
						addedPoints *= 2;

				for (int x = 0; x<8; ++x)
					if (tripleWordPoints[x][0] == offsetx+b && tripleWordPoints[x][1] == placedy)
						addedPoints *= 3;
			}
		}
	}

	for (int i = 0; i<wordList.size() && wordList[i].length()<strlen(wordColumn); ++i)
	{
		if ((offset=containsWord(wordList[i], wordColumn, placedy-offsety))>0)
		{
			// base points
			for (int b = 0; b<wordList[i].length(); ++i)
				addedPoints += getLetterPoints(wordList[i][b]);

			for (int b = 0; b<wordList[i].length(); ++i)
			{
				for (int x = 0; x<24; ++x)
					if (doubleLetterPoints[x][1] == offsety+b && doubleLetterPoints[x][0] == placedx)
						addedPoints += getLetterPoints(wordList[i][b]);

				for (int x = 0; x<12; ++x)
					if (tripleLetterPoints[x][1] == offsety+b && tripleLetterPoints[x][0] == placedx)
						addedPoints += getLetterPoints(wordList[i][b])*2;

				for (int x = 0; x<17; ++x)
					if (doubleWordPoints[x][1] == offsety+b && doubleWordPoints[x][0] == placedx)
						addedPoints *= 2;

				for (int x = 0; x<8; ++x)
					if (tripleWordPoints[x][1] == offsety+b && tripleWordPoints[x][0] == placedx)
						addedPoints *= 3;
			}
		}
	}

	return addedPoints;
}

void game()
{
	uint8_t input, x = 0, y = 0, lettersRemaining = 100, lettersEntered = 0;

	int p1Score = 0; // TODO: define pointsToWin (from menu/argument)

	vector<int> positions;

	char playfield[14][14];
	
	vector<char> letters;
	putLetters(letters);

	// randomise letters for players
	
	// mt19937 mt{}; //_// segfaults with random numbers !_! FIX !_! :(X)D
	char p1Letters[7];
	for (int i = 0, id; i<7; ++i)
	{
		//id = (double)mt()/(~0)*(lettersRemaining--);
		id = 93;
		p1Letters[i] = letters[id];
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
		return;
	}

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
		term_moveCursor(term_maxx/2-7+i*2, term_maxy/2+10);
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
			case 'A'...'Z': input = tolower(input);
			case 'a'...'z':
			{
				if (findLetter(input, p1Letters)!=-1)
				{
					++lettersEntered;
					playfield[x][y] = input;
				}
				break;
			}
			case 10: case 13: //enter
			{
				for (int i = 0; i<lettersEntered; ++i)
					// get x and y coordinates
					p1Score += checkword(/* add x and y coordinates */, playfield, wordList);
				break;
			}
			default: break;
		}
		term_moveCursor(COORDSX(x), COORDSY(y));
	}

}

int main() // menu
{
	term_init();


	short term_maxx, term_maxy;

	term_getTermSize(term_maxx, term_maxy);

	// menu things

	cout << "     ███╗   ███╗ ███████╗ ███╗   ██╗ ██╗   ██╗" << endl;
	cout << "     ████╗ ████║ ██╔════╝ ████╗  ██║ ██║   ██║" << endl;
	cout << "     ██╔████╔██║ █████╗   ██╔██╗ ██║ ██║   ██║" << endl;
	cout << "     ██║╚██╔╝██║ ██╔══╝   ██║╚██╗██║ ██║   ██║" << endl;
	cout << "     ██║ ╚═╝ ██║ ███████╗ ██║ ╚████║ ╚██████╔╝" << endl;
	cout << "     ╚═╝     ╚═╝ ╚══════╝ ╚═╝  ╚═══╝  ╚═════╝" << endl;


	Sleep(100);
	cout << "==============================================================" << endl;
	sleep(1);
	cout << "                       1) еуые1🙋‍♂️" << endl;
	sleep(1);
	cout << "                       2) еуые2🙋‍♂️" << endl;
	sleep(1);
	cout << "                       3) еуые3🙋‍♂️"<< endl;
	Sleep(100);
	cout << "==============================================================" << endl;



	game();

	term_deinit();
	return 0;
}
