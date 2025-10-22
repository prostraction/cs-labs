#include <iostream>

using namespace std;

//////////////////////////////////////////////////////
int main()
{
	const int MAX = 80;	// максимальная дллина строки
	char str[MAX];		// сама строка

	setlocale(LC_ALL, "russian");
	cout << "Введите строку : ";
	cin.get(str, MAX,'#');
	cout << "Вы ввели: " << str << endl;

	system("pause");
	return 0;
}