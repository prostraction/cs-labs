#include <iostream>

using namespace std;

void repchar();
void repchar(char);
void repchar(char, int);
//MAIN
int main()
{
	repchar();
	repchar('=');
	repchar('+', 30);
	cout << endl;
	system("pause");
}
//выводит на экран 45 '*'
void repchar()
{
	for (int j = 0; j < 45; j++)
	{
		cout << "*";
	}
	cout << endl;
}
//выводит 45 заданных символов
void repchar(char ch)
{
	for (int j = 0; j < 45; j++)
	{
		cout << ch;
	}
	cout << endl;
}
//выводит заданный символ заданное число раз
void repchar(char ch, int n)
{
	for (int j = 0; j < n; j++)
	{
		cout << ch;
	}
	cout << endl;
}