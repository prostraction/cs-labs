﻿#include <iostream>

using namespace std;

//////////////////////////////////////////////////////
int main()
{
	const int DAYS = 7;	// кол-во строк в массиве
	const int MAX = 12;	// максимальная длина каждой из них

	setlocale(LC_ALL, "russian");
	// массив строк
	char star[DAYS][MAX] =
	{
		"Понедельник", "Вторник", "Среда", "Четверг",
		"Пятница", "Суббота", "Воскресенье"
	};
	// вывод всех строк на экран
	for (int j = 0; j < DAYS; j++)
	{
		cout << star[j] << endl;
	}

	system("pause");
	return 0;
}