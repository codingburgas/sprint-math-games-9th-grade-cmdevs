#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

bool parse(string filename, vector<string>& wordList)
{
	fstream wordListFile(filename);

	if (!wordListFile.good()) return 1;

	string word;

	while (!wordListFile.eof())
	{
		getline(wordListFile, word);
		wordList.push_back(word);
		word.clear();
	}

	return 0;
}
