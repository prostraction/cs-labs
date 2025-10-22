#include <iostream>	// std
#include <conio.h>	// getche

using namespace std;

enum itsaWord { NO, YES }; // NO = 0, YES = 1

int main()
{
	setlocale(LC_ALL, "Russian");
	
	itsaWord isWord = NO;	// isWord = YES, 

	char ch = 'a';
	int wordcount = 0;

	cout << "¬ведите предложение: \n";
	do
	{
		ch = _getche();
		if (ch == ' ' || ch == '\r')
		{
			if (isWord == YES)
			{
				wordcount++;
				isWord = NO;
			}
		}
		else
			if (isWord == NO)
			{
				isWord = YES;
			}
	} 
	while (ch != '\r');
	cout << "\n---„исло слов: " << wordcount << "---\n";
	system("pause");
	return 0;
}