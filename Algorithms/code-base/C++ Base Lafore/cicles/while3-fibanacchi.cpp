#include <iostream>

using namespace std;

int main()
{
	const unsigned long limit = 4294967295;	//задаем лимит
	unsigned long next = 0;					//предпоследний член
	unsigned long last = 1;					//последний член

	while (next < limit / 2)				//результат не должен быть огромным
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