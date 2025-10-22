#include <iostream>
#include <conio.h> //дл¤ getche

using namespace std;

int main()
{
	int chcount = 0; //число непробельных символов
	int wdcount = 1; //число пробельных символов
	char ch = 'a';	 //ch должна иметь определенное значение
	setlocale(LC_ALL, "rus");

	cout << "¬ведите строку (русские буквы не поддерживаютс¤)  ";
	while (ch=_getche() != '\r')	//цикл до нажати¤ Enter
	{
		//ch = _getche();	//считывание символа
		if (ch == ' ')	//если символ ¤вл¤етс¤ пробелом,
			wdcount++;	//то инкрементируем число слов
		else			//в противном случае
			chcount++;	//инкрементируем число слов
	}					//вывод результатов
	cout << "\n—лов: " << wdcount << endl;
	cout << "Ѕукв: " << (chcount - 1) << endl;

	system("pause");
	return 0;
}