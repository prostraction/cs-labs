#include <iostream>

using namespace std;

inline float lbstokg(float pounds)
{
	return 0.453592 * pounds;
}

int main()
{
	float lbs;
	setlocale(LC_ALL, "russian");

	cout << "Введите вес в фунтах: ";
	cin >> lbs;
	cout << "Вес в килограммах: " << lbstokg(lbs) << endl;

	system("pause");
	return 0;
}