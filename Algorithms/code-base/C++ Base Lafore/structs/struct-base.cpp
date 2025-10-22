#include <iostream>

using namespace std;

struct part // объявление структуры
{
	int modelnumber; // номер модели
	int partnumber;	 // номер детали
	float cost;		 // стоимость
};

int main()
{
	part partl;		// определение структурной переменной
	partl.modelnumber = 6244;	// инициализация полей
	partl.partnumber = 373;		// переменной part1
	partl.cost = 217.55;
	// вывод значений на экран
	cout << "Model: " << partl.modelnumber << endl << "Detail: " << partl.partnumber << endl << "Cost: $" << partl.cost << endl;
	cout << "\n";
	system("pause");
	return 0;
}