#include <iostream>

using namespace std;

float lbstokg(float);

int main()
{
	float lbs;
	setlocale(LC_ALL, "russian");

	cout << "Введите вес в фунтах: ";
	cin >> lbs;
	cout << "Вес в килограммах равен " << lbstokg(lbs) << endl;
	system("pause");
	return 0;
}

float lbstokg(float pounds)
{
	return (0.453592 * pounds);
}