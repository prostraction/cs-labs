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
	part partl = {6244, 373, 217.55};		// инициализация переменной
	part part2;

	// вывод значений на экран

	cout << "Model: " << partl.modelnumber << endl << "Detail: " << partl.partnumber << endl << "Cost: $" << partl.cost << endl << "\n";
		part2 = partl;  // присваивание структурных переменных
	cout << "Model: " << part2.modelnumber << endl << "Detail: " << part2.partnumber << endl << "Cost: $" << part2.cost << endl; 
						// вывод полей второй переменной
	cout << "\n";
	system("pause");
	return 0;
}