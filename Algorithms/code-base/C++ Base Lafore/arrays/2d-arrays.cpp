#include <iostream>
#include <iomanip>

using namespace std;

const int DISTRICTS = 4;
const int MONTHS = 3;
//////////////////////////////////////////////////////
int main()
{
	int d, m;
	double sales [DISTRICTS][MONTHS];

	for (d = 0; d < DISTRICTS; d++)
	{
		for (m = 0; m < MONTHS; m++)
		{
			setlocale(LC_ALL, "russian");

			cout << "גוהטעו מבתול ןנמהאז הכ מעהוכא " << d + 1;
			cout << ", חא לוסצ " << m + 1 << ": ";
			cin >> sales[d][m];
		}
	}

	cout << "\n\n";
	cout << "\nוסצ	      1         2	  3";

	for (d = 0; d < DISTRICTS; d++)
	{
		cout << "\nעהוכ " << d + 1;
		for (m = 0; m < MONTHS; m++)
		{
			cout << setiosflags(ios::fixed)
				 << setiosflags(ios::showpoint)
				 << setprecision(2)
				 << setw(10)
				 << sales[d][m];
		}
	}
	cout << endl;
	system("pause");
	return 0;
}