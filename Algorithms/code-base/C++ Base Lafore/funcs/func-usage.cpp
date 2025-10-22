#include <iostream>

using namespace std;

struct Distance
{
	int feet;
	float inches;
};

void func(Distance);

int main()
{
	Distance d1, d2;
	setlocale(LC_ALL, "Russian");

	cout << "¬ведите число футов: ";
	cin >> d1.feet;
	cout << "¬ведите количество дюймов: ";
	cin >> d1.inches;

	cout << "\n¬ведите число футов (2): ";
	cin >> d2.feet;
	cout << "¬ведите число дюймов (2): ";
	cin >> d2.inches;

	cout << "\nn1 = ";
	func(d1);
	cout << "\nn2 = ";
	func(d2);

	cout << endl;
	system("pause");
	return 0;
}

void func (Distance dd)
{
	cout << dd.feet << "\'-" << dd.inches << "\"";
}