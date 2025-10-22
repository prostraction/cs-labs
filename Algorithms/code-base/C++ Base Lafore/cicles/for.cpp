#include <iostream>

using namespace std;

int main()
{
	int x;	//определение счетчика цикла

	for (x = 0; x < 15; x++)	//счетчик меняется от 0 до 14
	{
		cout << x * x << "  ";	//квадрат значения x выводится на экран, а после него следуют 2 пробела
		cout << endl;
	}

		system("pause");
		return 0;
}