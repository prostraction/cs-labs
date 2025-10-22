#include <iostream>

using namespace std;

int main()
{
	int x, z;
	unsigned long y = 1;
	setlocale(LC_ALL, "Russian");

	//y = factorial
	//x = значение юзверя
	//z - переменная для цикла

	do
	{
		cout << "Введите целое число - <12 или =12: ";
		cin >> x;

		for (z = x; z > 0; z--)	{
			y = y * z;
		}
		cout << "Факториал числа равен " << y << endl;
		y = 1;
		z = 0;
	}

	while (x != 0);
	return 0;
}