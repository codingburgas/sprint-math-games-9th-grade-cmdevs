#include <iostream>
#include <fstream>

vector<string> parse(string filename, bool& fileFound)
{
	fstream wordListFile(filename);
	if (!wordListFile.good())
	{
		fileFound = 0;
		return 0;
	}

	vector<string> wordList;

	string word;

	while (!wordListFile.eof())
	{
		getline(wordListFile, word);
		wordList.push_back(word);
		word.clear();
	}

	return wordList;
}
