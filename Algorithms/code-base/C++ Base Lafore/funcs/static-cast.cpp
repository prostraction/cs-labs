#include <iostream>

using namespace std;

int main()
{
	void intfrac(float, float&, float&);	// прототип
	float number, intpart, fracpart;

	setlocale(LC_ALL, "russian");

	do
	{
		cout << "\nВведите вещественное число: ";
		cin >> number;

		intfrac(number, intpart, fracpart);	// нахождение целов и дробной части

		cout << "Целая часть равна " << intpart;
		cout << ", дробная часть равна " << fracpart << endl;
	} while (number != 0);
	return 0;
}

void intfrac(float number, float& intpart, float& fracpart)
{
	long temp = static_cast<long>(number); // преобразование к типу long
	intpart = static_cast<float>(temp);	   // и обратно во float
	fracpart = number - intpart;		   // вычитаем целую часть
}