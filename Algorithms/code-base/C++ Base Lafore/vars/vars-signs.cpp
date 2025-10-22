#include <iostream>

using namespace std;

int main()
{
		setlocale(LC_CTYPE, "rus");				//русская cmd
			int signedVar = 1500000000;				//знаковая переменная
			unsigned int unsignVar = 1500000000;	//беззнаковая переменная
			signedVar = (signedVar * 2) / 3;		//выход за границы диапозона
			unsignVar = (unsignVar * 2) / 3;		//вычисления внутри диапозона
			cout << "Знаковая переменная равна  " << signedVar << endl;		//вывод правильного результата
			cout << "Беззнаковая переменная равна  " << unsignVar << endl;	//вывод неправильного результата
			system("pause");						//пауза
			return 0;								//0
}
