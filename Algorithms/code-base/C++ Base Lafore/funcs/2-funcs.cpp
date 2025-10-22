#include <iostream>

using namespace std;

struct Distance
{
	int feet;
	float inches;
};

Distance addengl(Distance, Distance); // прототипы
void engldisp(Distance);

int main()
{
	Distance d1, d2, d3;
	
	setlocale(LC_ALL, "russian");

	cout << "Введите число футов: ";
	cin >> d1.feet;
	
	cout << "Ввелите число дюймов: ";
	cin >> d1.inches;

	cout << "\nВведите число футов: ";
	cin >> d2.feet;

	cout << "Введите число дюймов: ";
	cin >> d2.inches;

	d3 = addengl(d1, d2);

	cout << endl;

	engldisp(d1); cout << " + "; engldisp(d2); cout << " = "; engldisp(d3); cout << endl;
	system("pause");
	return 0;

}

// функция addengl
// складывает два значение тима Distance и возвращает сумму

Distance addengl(Distance dd1, Distance dd2)
{
	Distance dd3;	// переменная для хранения результата
	dd3.inches = dd1.inches + dd2.inches;	// сложение дюймов
	dd3.feet = 0;
	if (dd3.inches >= 12.0)
	{
		dd3.inches -= 12.0;
		dd3.feet++;
	}
	dd3.feet += dd1.feet + dd2.feet; // сложение футов
	return dd3;
}

// функция engldisp
// отображает поля структурной переменной Distance

void engldisp(Distance dd)
{
	cout << dd.feet << "\'-" << dd.inches << "\"";
}