#include <iostream>

using namespace std;

struct Distance               // длина в английской системе
{
	int feet;			// футы
	float inches;		// дюймы
};

struct Room		// размеры комнаты
{
	Distance length;	// длина
	Distance width;		// ширина
};

int main()
{
	setlocale(LC_ALL, "Russian");
	Room dining;	// переменна¤ dining типа Room
	// задание параметров комнаты
	dining.length.feet = 13;	
	dining.length.inches = 6.5;
	dining.width.feet = 10;
	dining.width.inches = 0.0;
	// преобразование длины и ширины в вещественный формат
	float l = dining.length.feet + dining.length.inches / 12;
	float w = dining.width.feet + dining.width.inches / 12;

	cout << "ѕлощадь комнаты равна " << l * w << " квадратных футов\n";

	system("pause");
	return 0;
}