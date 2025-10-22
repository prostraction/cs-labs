#include <iostream>

using namespace std;

unsigned long factfunc(unsigned long);

int main()
{
	int n;
	unsigned long fact;
	setlocale(LC_ALL, "russian");

	cout << "¬ведите целое число: ";
	cin >> n;

	fact = factfunc(n);

	cout << "‘акториал числа " << n << " равен " << fact << endl;
	system("pause");
	return 0;
}

unsigned long factfunc(unsigned long n)
{
	if (n > 1)
	{
		return n * factfunc(n - 1);
	}
	else
	{
		return 1;
	}
}