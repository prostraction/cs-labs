#include <iostream>
#include <iomanip>

using namespace std;

//////////////////////////////////////////////////////
int main()
{
	const int MAX = 80;
	char str[MAX];

	setlocale(LC_ALL, "russian");
	cout << "Введите строку: ";
	// ввод не более чем MAX символов
	cin >> setw(MAX) >> str;

	cout << "Вы ввели: " << str << endl;
	system("pause");
	return 0;
}