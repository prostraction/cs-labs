﻿#include <iostream>
using namespace std;

struct Distance               // длина в английской системе
{
	int feet;
	float inches;
};

int main()
{
	Distance d1, d3;            // определени¤ двух переменных
	Distance d2 = { 11, 6.25 }; // определение с инициализацией
	setlocale(LC_ALL, "Russian");
	// ввод полей переменной d1
	cout << "\n¬ведите число футов: ";
	cin >> d1.feet;
	cout << "¬ведите число дюймов: ";
	cin >> d1.inches;

	// сложение длин d1 и d2 и присвоение результата d3
	d3.inches = d1.inches + d2.inches; // сложение дюймов 
	d3.feet = 0;                       // с возможным заемом
	if (d3.inches >= 12.0)            // если сумма больше 12.0,
	{                                  // то уменьшаем  
		d3.inches -= 12.0;              // число дюймов на 12.0 и
		d3.feet++;               // увеличиваем число футов на 1
	}
	d3.feet += d1.feet + d2.feet;      // сложение футов 

	// вывод всех длин на экран
	cout << d1.feet << "\'-" << d1.inches << "\" + ";
	cout << d2.feet << "\'-" << d2.inches << "\" = ";
	cout << d3.feet << "\'-" << d3.inches << "\"\n";

	system("pause");
	return 0;
}