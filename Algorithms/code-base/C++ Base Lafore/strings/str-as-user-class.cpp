#include <iostream>
#include <cstring>

using namespace std;

//////////////////////////////////////////////////////
class String
{
private:
	enum { SZ = 80 };	// максимальный размер строки
	char str[SZ];		// сама строка
public:
	// конструктор без параметров
	String()
	{
		str[0] = '\0';
	}
	// конструктор с одним параметром
	String(char s[])
	{
		strcpy_s(str, s);
	}
	// показ строки
	void display()
	{
		cout << str;
	}
	// сложение строк
	void concat(String s2)
	{
		if (strlen(str) + strlen(s2.str) < SZ)
		{
			strcat_s(str, s2.str);
		}
		else
		{
			setlocale(LC_ALL, "russian");
			cout << "\nПереполнение!";
		}
	}
};
//////////////////////////////////////////////////////
int main()
{
	setlocale(LC_ALL, "russian");

	String s1("С новым Годом!");	// конструктор с одним параметром
	String s2 = "С новым счастьем";	// аналогично, но в другой форме
	String s3;						// конструктор без аргументов

	cout << "\ns1 = "; s1.display();
	cout << "\ns2 = "; s2.display();
	cout << "\ns3 = "; s3.display();

	s3 = s1;

	cout << "\ns3 = "; s3.display();

	s3.concat(s2);
	cout << "\ns3 = "; s3.display();
	cout << endl;

	system("pause");
	return 0;
}	