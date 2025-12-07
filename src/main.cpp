#include <iostream>
#include <vector>
#include <cstdint>
#include <random>
#include <cstring>
#include <ctime>
#include <windows.h>

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

int findLetter(char letter, char letters[7])
{
	for (int i = 0; i<7; ++i)
	{
		if (letters[i]==letter) return i;
	}
	return -1;
}

int containsWord(string word, char str[15])
{
	bool found;
	int offset;
	for (int i = 0; str[i]; ++i)
	{
		found = 0;
		if (word[0]==str[i])
		{
			offset = i;
			found = 1;
			++i;
			for (; i-offset<word.length(); ++i)
			{
				if (str[i]!=word[i-offset])
				{
					found = 0;
					--i;
					break;
				}	
			}
			if (found)
			{
				return offset;
			}
		}
	}
	return -1;
}

int checkWordRow(int placedx, int placedy, const char board[15][15], vector<string> wordList)
{
	int addedPoints = 0;

	// find word(s) on the current row/column
	
	char wordRow[15];
	bool valid = 0;

	int offsetx = 0;

	// row
	for (int i = 0; i<15; ++i)
	{
		valid = 0;
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

	int offset;
	// Check for words on current row
	for (int i = wordList.size()-1; i>0; --i)
	{
		if (wordList[i].length()>strlen(wordRow)) continue;
		if ((offset = containsWord(wordList[i], wordRow))!=-1)
		{
			if (placedx<offset+offsetx || placedx>offset+wordList[i].length()+offsetx) continue; // if found word doesn't connect to the letter at placedx,placedy
			// base points
			for (int b = 0; b<wordList[i].length(); ++b)
				addedPoints += getLetterPoints(wordList[i][b]);

			for (int b = 0; b<wordList[i].length(); ++b)
			{
				for (int x = 0; x<24; ++x)
				{
					if ((doubleLetterPoints[x][0] == (offsetx+offset+b)) && (doubleLetterPoints[x][1] == placedy))
					{
						addedPoints += getLetterPoints(wordList[i][b]);
					}
				}

				for (int x = 0; x<12; ++x)
				{
					if ((tripleLetterPoints[x][0] == (offsetx+offset+b)) && (tripleLetterPoints[x][1] == placedy))
					{
						addedPoints += (getLetterPoints(wordList[i][b]))*2;
					}
				}

				for (int x = 0; x<17; ++x)
				{
					if ((doubleWordPoints[x][0] == (offsetx+offset+b)) && (doubleWordPoints[x][1] == placedy))
					{
						addedPoints *= 2;
					}
				}

				for (int x = 0; x<8; ++x)
				{
					if ((tripleWordPoints[x][0] == (offsetx+offset+b)) && (tripleWordPoints[x][1] == placedy))
					{
						addedPoints *= 3;
					}
				}
			}
			break;
		}
	}
	return addedPoints;
}

int checkWordColumn(int placedx, int placedy, const char board[15][15], vector<string> wordList)
{
	int offsety = 0, addedPoints = 0;
	char wordColumn[15];
	bool valid = 0;
	for (int i = 0; i<15; ++i)
	{
		valid = 0;
		if (board[placedx][i])
		{
			offsety = i;
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

	for (int i = wordList.size()-1; i>0; --i)
	{
		if (wordList[i].length()>strlen(wordColumn)) continue;
		if ((offset = containsWord(wordList[i], wordColumn))!=-1)
		{
			if (placedy<offset+offsety || placedy>offset+wordList[i].length()+offsety) continue; // if found word doesn't connect to the letter at placedx,placedy
			// base points
			for (int b = 0; b<wordList[i].length(); ++b)
				addedPoints += getLetterPoints(wordList[i][b]);

			for (int b = 0; b<wordList[i].length(); ++b)
			{
				for (int x = 0; x<24; ++x)
				{
					if ((doubleLetterPoints[x][1] == (offsety+offset+b)) && (doubleLetterPoints[x][0] == placedx))
					{
						addedPoints += getLetterPoints(wordList[i][b]);
					}
				}

				for (int x = 0; x<12; ++x)
				{
					if ((tripleLetterPoints[x][1] == (offsety+offset+b)) && (tripleLetterPoints[x][0] == placedx))
					{
						addedPoints += getLetterPoints(wordList[i][b])*2;
					}
				}

				for (int x = 0; x<17; ++x)
				{
					if ((doubleWordPoints[x][1] == (offsety+offset+b)) && (doubleWordPoints[x][0] == placedx))
					{
						addedPoints *= 2;
					}
				}

				for (int x = 0; x<8; ++x)
				{
					if ((tripleWordPoints[x][1] == (offsety+offset+b)) && (tripleWordPoints[x][0] == placedx))
					{
						addedPoints *= 3;
					}
				}
			}
			break;
		}
	}

	return addedPoints;
}

void redrawSpecialTiles(short term_maxx, short term_maxy, uint8_t specialColorPairIds[4])
{
	term_enableColorPair(specialColorPairIds[DOUBLEWORDPOINTSPAIR]);
	for (int i = 0; i<17; ++i)
	{
		term_moveCursor(COORDSX(doubleWordPoints[i][0]), COORDSY(doubleWordPoints[i][1]));
		cout << ' ';
	}

	term_enableColorPair(specialColorPairIds[TRIPLEWORDPOINTSPAIR]);
	for (int i = 0; i<8; ++i)
	{
		term_moveCursor(COORDSX(tripleWordPoints[i][0]), COORDSY(tripleWordPoints[i][1]));
		cout << ' ';
	}

	term_enableColorPair(specialColorPairIds[DOUBLELETTERPOINTSPAIR]);
	for (int i = 0; i<24; ++i)
	{
		term_moveCursor(COORDSX(doubleLetterPoints[i][0]), COORDSY(doubleLetterPoints[i][1]));
		cout << ' ';
	}

	term_enableColorPair(specialColorPairIds[TRIPLELETTERPOINTSPAIR]);
	for (int i = 0; i<12; ++i)
	{
		term_moveCursor(COORDSX(tripleLetterPoints[i][0]), COORDSY(tripleLetterPoints[i][1]));
		cout << ' ';
	}

	term_resetColorPair();
}

void redrawLetters(short term_maxx, short term_maxy, char playerLetters[7], uint8_t tileColorPairIds[11])
{
	for (int i = 0; i<7; ++i)
	{
		term_moveCursor(term_maxx/2-7+i*2, term_maxy/2+10);
		COLORPAIR(playerLetters[i]);
		cout << (playerLetters[i]?playerLetters[i]:' ');
	}
	term_resetColorPair();
}

void fillPlayerLetters(vector<char>& letters, char playerLetters[7])
{
	static mt19937 mt(time(nullptr));
	for (int i = 0, id; i<7; ++i)
	{
		if (!letters.size()) return;
		if (!playerLetters[i])
		{
			id = (((double)mt())/(UINT32_MAX))*(letters.size());
			playerLetters[i] = letters[id];
			letters.erase(letters.begin()+id);
		}
	}
}

int emptyLettersOnHand(char letters[7])
{
	int ret = 0;
	for (int i = 0; i<7; ++i)
		ret += (letters[i]==0);

	return ret;
}

int game()
{
	term_clear();

	term_setCursorVisibility(1);

	uint8_t input, x = 7, y = 7, direction = 0;
	// direction 0 - undecided, 1-x, 2-y

	int playerScore[2] = {0,0};

	vector<uint8_t> positionsx{}, positionsy{};

	char playfield[15][15];

	// clear field

	for (int x = 0; x<15; ++x)
	{
		for (int y = 0; y<15; ++y)
		{
			playfield[x][y] = 0;
		}
	}

	vector<char> letters{};
	putLetters(letters);

	char playerLetters[2][7] = { {0,0,0,0,0,0,0}, {0,0,0,0,0,0,0} };
	uint8_t turn = 0;

	fillPlayerLetters(letters, playerLetters[0]);
	fillPlayerLetters(letters, playerLetters[1]);

	vector<string> wordList;
	if (parse("words", wordList))
	{
		term_deinit();
		cout << "Word list not found, exiting...\n";
		term_getch();
		exit(1);
	}

	short term_maxx, term_maxy;

	term_getTermSize(term_maxx, term_maxy);

	uint8_t tileColorPairIds[11];
	tileColorPairIds[0] = term_createColorPair(WHITE, BLACK);
	tileColorPairIds[1] = term_createColorPair(RED, BLACK);
	tileColorPairIds[2] = term_createColorPair(YELLOW, BLACK);
	tileColorPairIds[3] = term_createColorPair(GREEN, BLACK);
	tileColorPairIds[4] = term_createColorPair(PINK, BLACK);
	tileColorPairIds[5] = term_createColorPair(CYAN, BLACK);
	tileColorPairIds[8] = term_createColorPair(BLUE, BLACK);
	tileColorPairIds[10] = term_createColorPair(LIGHTGREEN, BLACK);

	uint8_t specialTileColorPairIds[4];
	specialTileColorPairIds[DOUBLEWORDPOINTSPAIR] = term_createColorPair(BLUE, BLUE);
	specialTileColorPairIds[TRIPLEWORDPOINTSPAIR] = term_createColorPair(RED, RED);
	specialTileColorPairIds[DOUBLELETTERPOINTSPAIR] = term_createColorPair(CYAN, CYAN);
	specialTileColorPairIds[TRIPLELETTERPOINTSPAIR] = term_createColorPair(PINK, PINK);

	term_moveCursor(0,0);
	cout << "Player 1: 0 points\nPlayer 2: 0 points"; // temp

	term_moveCursor(term_maxx/2+12, term_maxy/2-5);
	term_enableColorPair(tileColorPairIds[1]);
	cout << "1 point";
	term_moveCursor(term_maxx/2+12, term_maxy/2-4);
	term_enableColorPair(tileColorPairIds[2]);
	cout << "2 points";
	term_moveCursor(term_maxx/2+12, term_maxy/2-3);
	term_enableColorPair(tileColorPairIds[3]);
	cout << "3 points";
	term_moveCursor(term_maxx/2+12, term_maxy/2-2);
	term_enableColorPair(tileColorPairIds[4]);
	cout << "4 points";
	term_moveCursor(term_maxx/2+12, term_maxy/2-1);
	term_enableColorPair(tileColorPairIds[5]);
	cout << "5 points";
	term_moveCursor(term_maxx/2+12, term_maxy/2);
	term_enableColorPair(tileColorPairIds[8]);
	cout << "8 points";
	term_moveCursor(term_maxx/2+12, term_maxy/2+1);
	term_enableColorPair(tileColorPairIds[10]);
	cout << "10 points";
	term_moveCursor(term_maxx/2+12, term_maxy/2+2);
	term_enableColorPair(specialTileColorPairIds[DOUBLEWORDPOINTSPAIR]);
	cout << ' ';
	term_resetColorPair();
	cout << " - 2x word points";
	term_moveCursor(term_maxx/2+12, term_maxy/2+3);
	term_enableColorPair(specialTileColorPairIds[TRIPLEWORDPOINTSPAIR]);
	cout << ' ';
	term_resetColorPair();
	cout << " - 3x word points";
	term_moveCursor(term_maxx/2+12, term_maxy/2+4);
	term_enableColorPair(specialTileColorPairIds[DOUBLELETTERPOINTSPAIR]);
	cout << ' ';
	term_resetColorPair();
	cout << " - 2x letter points";
	term_moveCursor(term_maxx/2+12, term_maxy/2+5);
	term_enableColorPair(specialTileColorPairIds[TRIPLELETTERPOINTSPAIR]);
	cout << ' ';
	term_resetColorPair();
	cout << " - 3x letter points";
	term_resetColorPair();

	// print letters for p1

	redrawLetters(term_maxx, term_maxy, playerLetters[0], tileColorPairIds);

	// print special tiles

	redrawSpecialTiles(term_maxx, term_maxy, specialTileColorPairIds);

	// print turn

	term_moveCursor(COORDSX(1), COORDSY(-3));
	cout << "Player " << turn+1 << "'s turn";

	// move cursor to the center of the playfield

	term_moveCursor(COORDSX(x), COORDSY(y));

	while ((input = term_getch())!=CONTROL('C'))
	{
		switch (input)
		{
			case KEY_ARROW_UP:
			{
				if (y>0 && direction!=1)
				{
					if (!positionsx.size()&&(playfield[x][y]||playfield[x][y-1]))
					{
						--y;
						break;
					}
					if ((playfield[x][y]||playfield[x][y-1]))
					{
						--y;
						break;
					}
				}
				break;
			}
			case KEY_ARROW_DOWN:
			{
				if (y<14 && direction!=1)
				{
					if (!positionsx.size()&&(playfield[x][y]||playfield[x][y+1]))
					{
						++y;
						break;
					}
					if ((playfield[x][y]||playfield[x][y+1]))
					{
						++y;
						break;
					}
				}
				break;
			}
			case KEY_ARROW_LEFT:
			{
				if (x>0 && direction!=2)
				{
					if (!positionsx.size()&&(playfield[x][y]||playfield[x-1][y]))
					{
						--x;
						break;
					}
					if ((playfield[x][y]||playfield[x-1][y]))
					{
						--x;
						break;
					}
				}
				break;
			}
			case KEY_ARROW_RIGHT:
			{
				if (x>0 && direction!=2)
				{
					if (!positionsx.size()&&(playfield[x][y]||playfield[x+1][y]))
					{
						++x;
						break;
					}
					if (playfield[x][y]||playfield[x+1][y])
					{
						++x;
						break;
					}
				}
				break;
			}
			case 'A'...'Z': input = tolower(input);
			case 'a'...'z':
			{
				if (playfield[x][y]) break;
				int8_t letterId;
				if ((letterId = findLetter(input, playerLetters[turn]))!=-1)
				{
					if (positionsx.size()==1)
					{
						direction = 1+(positionsx[0]==x);
					}
					COLORPAIR(playerLetters[turn][letterId]);
					cout << playerLetters[turn][letterId];
					term_resetColorPair();
					playerLetters[turn][letterId] = 0;
					playfield[x][y] = input;
					positionsx.push_back(x);
					positionsy.push_back(y);
					redrawLetters(term_maxx, term_maxy, playerLetters[turn], tileColorPairIds);
				}
				break;
			}
			case KEY_BACKSPACE:
			{
				for (int i = 0; i<positionsx.size(); ++i)
				{
					if (positionsx[i]==x&&positionsy[i]==y)
					{
						// print special tile color if present
						for (int b = 0; b<24; ++b)
						{
							if (doubleLetterPoints[b][0]==x&&doubleLetterPoints[b][1]==y)
							{
								term_enableColorPair(specialTileColorPairIds[DOUBLELETTERPOINTSPAIR]);
								break;
							}
						}

						for (int b = 0; b<12; ++b)
						{
							if (tripleLetterPoints[b][0]==x&&tripleLetterPoints[b][1]==y)
							{
								term_enableColorPair(specialTileColorPairIds[TRIPLELETTERPOINTSPAIR]);
								break;
							}
						}

						for (int b = 0; b<17; ++b)
						{
							if (doubleWordPoints[b][0]==x&&doubleWordPoints[b][1]==y)
							{
								term_enableColorPair(specialTileColorPairIds[DOUBLEWORDPOINTSPAIR]);
								break;
							}
						}

						for (int b = 0; b<8; ++b)
						{
							if (tripleWordPoints[b][0]==x&&tripleWordPoints[b][1]==y)
							{
								term_enableColorPair(specialTileColorPairIds[TRIPLEWORDPOINTSPAIR]);
								break;
							}
						}

						positionsx.erase(positionsx.begin()+i);
						positionsy.erase(positionsy.begin()+i);
						cout << ' ';
						for (int8_t b = 0; b<7; ++b)
						{
							if (playerLetters[turn][b]==0)
							{
								playerLetters[turn][b] = playfield[x][y];
								redrawLetters(term_maxx, term_maxy, playerLetters[turn], tileColorPairIds);
								playfield[x][y] = 0;
								break;
							}
						}

						if (!positionsx.size()&&!playfield[7][7]) // return player to 7:7 if deleted all tiles while being outside of 7:7 on the first turn
						{
							x = 7;
							y = 7;
							break;
						}
						else if (positionsx.size()==1) direction = 0;

						break;
					}
				}
				break;
			}
			case 32: // pass
			{
				// if any letters have been placed
				if (positionsx.size()) break;

				turn = !turn;
				redrawLetters(term_maxx, term_maxy, playerLetters[turn], tileColorPairIds);

				// print turn
				term_moveCursor(COORDSX(1), COORDSY(-3));
				cout << "Player " << turn+1 << "'s turn";
				break;
			}
			case 10: case 13: // enter and numpad enter
			{	// count points
				if (!positionsx.size()) break;
				int addScore = 0, index = positionsx.size()-1;
				if (index==6) addScore += 50;
				if (direction==1) // x
				{
					addScore += checkWordRow(positionsx[positionsx.size()-1], positionsy[positionsy.size()-1], playfield, wordList);
					while (index)
					{
						addScore += checkWordColumn(positionsx[positionsx.size()-1], positionsy[positionsy.size()-1], playfield, wordList);
						--index;
					}	
				}
				else // y
				{
					addScore += checkWordColumn(positionsx[positionsx.size()-1], positionsy[positionsy.size()-1], playfield, wordList);
					while (index)
					{
						addScore += checkWordRow(positionsx[positionsx.size()-1], positionsy[positionsy.size()-1], playfield, wordList);
						--index;
					}
				}
				// no words found
				if (!addScore) break;
				playerScore[turn] += addScore;
				term_moveCursor(0, turn);
				cout << "Player " << turn+1 << ": " << playerScore[turn] << " point" << (playerScore[turn]%100==1?' ':'s');
				positionsx.clear();
				positionsy.clear();
				fillPlayerLetters(letters, playerLetters[turn]);

				if ((letters.size()+emptyLettersOnHand(playerLetters[turn]))==0)
				{
					// if all letters are used
					return (playerScore[0]!=playerScore[1]?(playerScore[0]<playerScore[1])+1:0);
				}

				turn = !turn;

				redrawLetters(term_maxx, term_maxy, playerLetters[turn], tileColorPairIds);

				// print turn
				term_moveCursor(COORDSX(1), COORDSY(-3));
				cout << "Player " << turn+1 << "'s turn";

				direction = 0;
				break;
			}
			case CONTROL('R'): // replace some or all letters
			{
				if (positionsx.size()) break;
				term_moveCursor(COORDSX(-2), COORDSY(-5));
				cout << "Select letters to replace";
				term_moveCursor(COORDSX(x), COORDSY(y));
				while ((input = term_getch()) && input!=10 && input!=13)
				{
					switch (input)
					{
						case 'A'...'Z': input = tolower(input);
						case 'a'...'z':
						{
							for (int i = 0; i<7; ++i)
							{
								if (playerLetters[turn][i]==input)
								{
									playerLetters[turn][i] = 0;
									redrawLetters(term_maxx, term_maxy, playerLetters[turn], tileColorPairIds);
									term_moveCursor(COORDSX(x), COORDSY(y));
									break;
								}
							}
							break;
						}
					}
				}

				fillPlayerLetters(letters, playerLetters[turn]);

				term_moveCursor(20, 3);
				cout << (letters.size()+emptyLettersOnHand(playerLetters[turn]));
				if ((letters.size()+emptyLettersOnHand(playerLetters[turn]))==0)
				{
					// if all letters are used
					return (playerScore[0]!=playerScore[1]?(playerScore[0]<playerScore[1])+1:0);
				}

				term_moveCursor(COORDSX(-2), COORDSY(-5));
				cout << "                         ";

				turn = !turn;

				redrawLetters(term_maxx, term_maxy, playerLetters[turn], tileColorPairIds);

				// print turn
				term_moveCursor(COORDSX(1), COORDSY(-3));
				cout << "Player " << turn+1 << "'s turn";

				break;
			}
			default: break;
		}
		term_moveCursor(COORDSX(x), COORDSY(y));
	}
	return (playerScore[0]!=playerScore[1]?(playerScore[0]<playerScore[1])+1:0);
}

int main() // menu
{
	term_setCursorVisibility(0);
init:
	term_init();

	short term_maxx, term_maxy;

	term_getTermSize(term_maxx, term_maxy);

	if (term_maxx<60||term_maxy<25) return -1;

	term_moveCursor(term_maxx/2-20, term_maxy/2-10);
	cout << " _______   _______   _\n";
	Sleep(200);
	term_moveCursor(term_maxx/2-20, term_maxy/2-9);
	cout << "(       ) (  ____ \\ ( (    /| |\\     /|\n";
	Sleep(200);
	term_moveCursor(term_maxx/2-20, term_maxy/2-8);
	cout << "| () () | | (    \\/ |  \\  ( | | )   ( |\n";
	Sleep(200);
	term_moveCursor(term_maxx/2-20, term_maxy/2-7);
	cout << "| || || | | (__     |   \\ | | | |   | |\n";
	Sleep(200);
	term_moveCursor(term_maxx/2-20, term_maxy/2-6);
	cout << "| |(_)| | |  __)    | (\\ \\) | | |   | |\n";
	Sleep(200);
	term_moveCursor(term_maxx/2-20, term_maxy/2-5);
	cout << "| |   | | | (       | | \\   | | |   | |\n";
	Sleep(200);
	term_moveCursor(term_maxx/2-20, term_maxy/2-4);
	cout << "| )   ( | | (____/\\ | )  \\  | | (___) |\n";
	Sleep(200);
	term_moveCursor(term_maxx/2-20, term_maxy/2-3);
	cout << "|/     \\| (_______/ |/    )_) (_______)\n";

	Sleep(200);
	term_moveCursor(term_maxx/2-31, term_maxy/2);
	cout << "==============================================================\n";
	Sleep(200);
	term_moveCursor(term_maxx/2-4, term_maxy/2+2);
	cout << "1) Start";
	Sleep(200);
	term_moveCursor(term_maxx/2-4, term_maxy/2+3);
	cout << "2) Quit";
	Sleep(200);
	term_moveCursor(term_maxx/2-4, term_maxy/2+4);
	cout << "3) Rules";
	Sleep(200);
	term_moveCursor(term_maxx/2-4, term_maxy/2+5);
	cout << "4) Controls";
	Sleep(200);
	term_moveCursor(term_maxx/2-4, term_maxy/2+6);
	cout << "5) Creators";
	Sleep(200);
	term_moveCursor(term_maxx/2-31, term_maxy/2+8);
	cout << "==============================================================\n";

	int result;
	switch(term_getch())
	{
		case '1':
		{
			result = game();
			term_setCursorVisibility(0);
			break;
		}
		case '3':
		{
			term_clear();
			Sleep(100);
			term_moveCursor(term_maxx/2-4, term_maxy/2-10);
			cout << "Scrabble";
			Sleep(100);
			term_moveCursor(term_maxx/2-20, term_maxy/2-8);
			cout << "The goal of the game is to get points";
			Sleep(100);
			term_moveCursor(term_maxx/2-20, term_maxy/2-7);
			cout << "by using the letters you have. You win";
			Sleep(100);
			term_moveCursor(term_maxx/2-20, term_maxy/2-6);
			cout << "when there are no more letters left.";
			Sleep(100);
			term_moveCursor(term_maxx/2-20, term_maxy/2-5);
			cout << "You can move only to tiles with letters";
			Sleep(100);
			term_moveCursor(term_maxx/2-20, term_maxy/2-4);
			cout << "in or next to them. You can see your";
			Sleep(100);
			term_moveCursor(term_maxx/2-20, term_maxy/2-3);
			cout << "hand below the playfield. During your";
			Sleep(100);
			term_moveCursor(term_maxx/2-20, term_maxy/2-2);
			cout << "turn, you can do either:";
			Sleep(100);
			term_moveCursor(term_maxx/2-20, term_maxy/2-1);
			cout << "1. Place one or more letters to form";
			Sleep(100);
			term_moveCursor(term_maxx/2-20, term_maxy/2);
			cout << "one or more words";
			Sleep(100);
			term_moveCursor(term_maxx/2-20, term_maxy/2+1);
			cout << "2. Pass the turn and get zero points";
			Sleep(100);
			term_moveCursor(term_maxx/2-20, term_maxy/2+2);
			cout << "3. Exchange some letters for new ones";
			Sleep(100);
			term_moveCursor(term_maxx/2-20, term_maxy/2+3);
			cout << "Good luck and have fun!";
			term_getch();
			goto init;
		}
		case '4':
		{
			term_clear();
			term_moveCursor(term_maxx/2-15, term_maxy/2-3);
			cout << "Arrow keys - move on the field";
			Sleep(100);
			term_moveCursor(term_maxx/2-15, term_maxy/2-2);
			cout << "Letter - choose / place a letter";
			Sleep(100);
			term_moveCursor(term_maxx/2-15, term_maxy/2-1);
			cout << "Enter - calculate score";
			Sleep(100);
			term_moveCursor(term_maxx/2-15, term_maxy/2);
			cout << "Space - pass your turn";
			Sleep(100);
			term_moveCursor(term_maxx/2-15, term_maxy/2+1);
			cout << "Ctrl-R - exchange letters";
			Sleep(100);
			term_moveCursor(term_maxx/2-15, term_maxy/2+2);
			cout << "Backspace - return a letter";
			Sleep(100);
			term_moveCursor(term_maxx/2-15, term_maxy/2+3);
			cout << "Ctrl-C - finish the game";
			term_getch();
			goto init;
		}
		case '5':
		{
			term_clear();
			term_moveCursor(term_maxx/2-20, term_maxy/2-2);
			cout << "Scrum Leader       -       SAKostin";
			Sleep(200);
			term_moveCursor(term_maxx/2-20, term_maxy/2-1);
			cout << "Backend Developer  -       SVRomanchenko";
			Sleep(200);

			term_moveCursor(term_maxx/2-20, term_maxy/2+1);
			cout << "Frontend Developer -       OVBondarenko";
			Sleep(200);
			term_moveCursor(term_maxx/2-20, term_maxy/2+2);
			cout << "Designer           -       SAChapkina";
			term_getch();
			goto init;
		}
		default:
		{
			term_deinit();
			term_setCursorVisibility(1);
			return 0;
		}
	}

	term_deinit();
	term_setCursorVisibility(1);

	if (result) cout << "Player " << result << " won!\n";
	else cout << "Tie!\n";
	term_getch();
	return 0;
}
