#include <iostream>

using namespace std;

//////////////////////////////////////////////////////
int main()
{
	const int SIZE = 6;
	double sales[SIZE];

	setlocale(LC_ALL, "russian");

	cout << "Введите объем продаж на каждый из шести дней: " << endl;

	for (int j = 0; j < SIZE; j++)
	{
		cin >> sales[j];
	}

	double total = 0;

	for (int j = 0; j < SIZE; j++)
	{
		total += sales[j];
	}

	double average = total / SIZE;

	cout << "Средний объем: " << average << endl;
	system("pause");
	return 0;
}