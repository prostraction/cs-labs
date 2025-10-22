#include <iostream>

using namespace std;

int main()
{
	long dividend, divisor;
	char ch;

	setlocale(LC_ALL, "Rus");

	do
	{
		cout << "Введите делимое   ";
			cin >> dividend;
		cout << "Введите делитель  ";
			cin >> divisor;
		if (divisor == 0)
		{
			cout << "Некорректный делитель!\n";
			continue;
		}
		cout << "Частное равно " << dividend / divisor;
		cout << ", остаток равен " << dividend % divisor << endl;
		cout << "\n";
		cout << "Еще раз?(y/n): ";
		cin >> ch;
		cout << "\n";
	}
	while (ch != 'n');
	return 0;
}