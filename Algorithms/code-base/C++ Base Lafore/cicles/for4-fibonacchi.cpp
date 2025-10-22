#include <iostream>

using namespace std;

int main()
{
	unsigned long next = 0;	//предпосдений член
	unsigned long last = 1; //последний член
	int z,x;
	setlocale(LC_ALL, "Russian"),

	//z - число, до какого пойдет цикл
	//x - переменна¤ дл¤ цикла

	cout << "”кажите целое число, до какого пойдет цикл: ";
	cin >> z;

	for (x = 0; x < z; x++)
	{
		cout << last << ", ";				//вывод последнего члена
		long sum = next + last;				//сложение двух последних членов
		next = last;						//обновление предпоследнего
		last = sum;							//и последнего членов
	}

	cout << endl;
	system("pause");
	return 0;
}