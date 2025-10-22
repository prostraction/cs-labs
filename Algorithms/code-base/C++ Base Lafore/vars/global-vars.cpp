#include <iostream>
#include <conio.h>

using namespace std;

char ch = 'a';

void getachar();
void putachar();

int main()
{
	setlocale(LC_ALL, "russian");
	while (ch != '/r')
	{
		getachar();
		putachar();
	}
	cout << endl;
	return 0;
}

void getachar()
{
	ch = _getch();
}

void putachar()
{
	cout << ch;
}