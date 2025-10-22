#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

enum Suit { clubs, diamonds, hearts, spades };
//////////////////////////////////////////////////////
// именованные достоинства карт
const int jack = 11;
const int queen = 12;
const int king = 13;
const int ace = 14;
//////////////////////////////////////////////////////
class card
{
private:
	int number;	// достоинства карт
	Suit suit;	// масть
public:
	card()
	{
		// конструктор без аргументов
	}
	void set(int n, Suit s)
	{
		suit = s;
		number = n;
	}
	void display();		// вывод карты на экран
};
//----------------------------------------------------
void card::display()	// вывод карты на экран
{
	if (number >= 2 && number <= 10)
	{
		cout << number;

		switch (suit)
		{
		case clubs:		cout << "Т"; break;
		case diamonds:  cout << "Б"; break;
		case hearts:	cout << "Ч"; break;
		case spades:	cout << "П"; break;
		}
	}
	else
	{
		switch (number)
		{
		case jack:  cout << "J";  break;
		case queen: cout << "Q";  break;
		case king:  cout << "K";  break;
		case ace:	cout << "A";  break;
		}

		switch (suit)
		{
		case clubs:		cout << "Т"; break;
		case diamonds:  cout << "Б"; break;
		case hearts:	cout << "Ч"; break;
		case spades:	cout << "П"; break;
		}
	}
}
//////////////////////////////////////////////////////
int main()
{
	card deck[52];
	int j;
	
	setlocale(LC_ALL, "russian");
	// создаем упорядаченную колоду карт
	for (j = 0; j < 52; j++)
	{
		int num = (j % 13) + 2;
		Suit su = Suit(j / 13);
		deck[j].set(num, su);
	}
	// показываем исходную колоду
	cout << "Исходная колода: " << endl;
	for (j = 0; j < 52; j++)
	{
		deck[j].display();
		cout << "  ";
		if (!((j + 1) % 13))	// начинаем новую строку после каждой 13-й карты
		{
			cout << endl;
		}
	}
	cout << endl;
	// инициализируем генератор случайных чисел
	srand(time(NULL));
	for (j = 0; j < 52; j++)
	{
		int k = rand() % 52;	// выбираем случайную карту
		card temp = deck[j];	// и меняем ее с текущей
		deck[j] = deck[k];
		deck[k] = temp;
	}
	// показываем перемешанную колоду
	cout << "Перемешанная колода: " << endl;
	for (j = 0; j < 52; j++)
	{
		deck[j].display();
		cout << "  ";
		if (!((j + 1) % 13))	// начинаем новую строку после каждой 13-й карты
		{
			cout << endl;
		}
	}
	cout << endl;
	system("pause");
	return 0;
}