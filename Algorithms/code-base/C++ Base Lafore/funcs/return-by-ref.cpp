#include <iostream>

using namespace std;

int x;
int& setx();

int main()
{
	setx() = 92;
	cout << "x = " << x << endl;
	system("pause");
	return 0;
}

int& setx()
{
	return x;
}