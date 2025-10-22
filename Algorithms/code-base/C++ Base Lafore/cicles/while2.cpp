#include <iostream>
#include <iomanip>				//setw

using namespace std;

int main()
{
	int pow = 1;			//первое возмодимое число равно 1
	int numb = 1;			//1 в 4-й степени равна 1

	while (pow < 10000)		//цикл пока в степени не более 4 цифр
	{
		cout << setw(2) << numb;			//вывод числа
		cout << setw(5) << pow << endl;		//и его 4 степени
		++numb;								//инкремент текущего числа
		pow = numb*numb*numb*numb;			//вычисление 4-й степени
	}

	cout << endl;
	system("pause");
	return 0;
}