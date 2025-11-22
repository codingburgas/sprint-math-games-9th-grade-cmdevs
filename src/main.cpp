#include <iostream>
#include <vector>

#include "../include/ui.h"
#include "../include/wordparser.h"

using namespace std;

int main()
{
	bool fileFound;
	vector<string> wordList = parse("words", fileFound);
	if (!fileFound)
	{
		cout << "The word list file wasn't found, exiting...";
	}

	term_init();

	// mainloop

	term_deinit();
}
