#include <iostream>
#include <cstring>

using namespace std;

//////////////////////////////////////////////////////
int main()
{
	// исходна¤ строка
	setlocale(LC_ALL, "russian");
	char str1[] = "Привет строка 1";

	const int MAX = 80;
	char str2[MAX];
	
	strcpy_s(str2, str1);

	cout << str2 << endl;

	system("pause");
	return 0;
}