#include <iostream>

using namespace std;

int main()
{
	int n = 99;			//n не должна быть равной 0

	while (n != 0)		//цикл пока значение не равно 0
	{
		cin >> n;		//считывание n с клавы
	}

	cout << endl;		//цикл закончился
	system("pause");
	return 0;
}