#include <iostream>

using namespace std;

enum days_of_week {Mon, Tue, Wed, Thu, Fri, Sat, Sun};

int main()
{
	setlocale(LC_ALL, "Russian");
	
	days_of_week day1, day2;

	day1 = Mon;
	day2 = Thu;

	int diff = day2 - day1;
	cout << "Разница в днях: " << diff << endl;

	if (day1 < day2)
		cout << "day1 наступит раньше чем day2\n";

	system("pause");
	return 0;
}