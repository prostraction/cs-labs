#include <iostream>

using namespace std;

float getavg(float);

int main()
{
	float data = 1, avg;
	setlocale(LC_ALL, "russian");

	while (data != 0)
	{
		cout << "Введите число: ";
		cin >> data;
		avg = getavg(data);
		cout << "Среднее значение: " << avg << endl;
	}
	return 0;
}

float getavg(float newdata)
{
	static float total = 0;	//инициализаци¤ статических
	static int count = 0;	//переменных при первом вызове
	count++;				//увелечение счетчика
	total += newdata;		//добавление нового значение к сумме
	return total / count;	//возврат нового среднего значение
}