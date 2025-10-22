#include <iostream>

using namespace std;

//////////////////////////////////////////////////////
class Distance
{
private:
	int feet;
	float inches;
public:
	// получение информации
	void getdist()
	{
		setlocale(LC_ALL, "russian");
		cout << "Введите футы: ";
		cin >> feet;
		cout << "Введите дюймы: ";
		cin >> inches;
	}
	// показ информации
	void showdist()
	{
		cout << feet << "\'-" << inches << "\"" << endl;
	}
};
//////////////////////////////////////////////////////
int main()
{
	Distance dist[100]; // массив переменных
	int n = 0;			// счетчик данных
	char ch;		// ответ пользователя (y/n)
	// получаем и сохраняем длина
	do
	{
		setlocale(LC_ALL, "russian");
		dist[n++].getdist();
		cout << "Продолжить ввод? (y/n): ";
		cin >> ch;
	} while (ch != 'n');
	// показываем все введенное
	for (int j = 0; j < n; j++)
	{
		cout << "Длина номер " << j + 1 << " : ";
		dist[j].showdist();
	}
	cout << endl;
	system("pause");
	return 0;
}