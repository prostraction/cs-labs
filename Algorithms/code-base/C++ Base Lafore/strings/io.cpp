#include <iostream>
#include <string>

using namespace std;
//////////////////////////////////////////////////////
int main()
{
	setlocale(LC_ALL, "russian");
	
	string full_name, nickname, address;
	string greeting = "Здравствуйте, ";

	cout << "Введите Ваше имя: ";
	getline(cin, full_name);
	cout << "Ваше имя: " << full_name << endl;

	cout << "Введите Ваш псевдоним: ";
	cin >> nickname;

	greeting += nickname;
	cout << greeting << endl;

	cout << "Введите Ваш адрес в несколько строк\n";
	getline(cin, address, '$');

	cout << "Ваш адрес: " << address << endl;

	system("pause");
	return 0;
}