#include <iostream>
#include <iomanip>		//setw

using namespace std;

int main()
{
	int numb;		//счетчик цикла
	
	for (numb = 1; numb <= 10; numb++)		//цикл от 1 до 10	
	{
		cout << setw(4) << numb;		//вывод 1 столбца
		int cube = numb * numb * numb;	//вычисление куба
		cout << setw(6) << cube << endl;	//вывод 2 столбца
	}

	system("pause");
	return 0;
}