#include <iostream>
#include <iomanip>

using namespace std;

const int DISTRICTS = 4;
const int MONTHS = 3;

//////////////////////////////////////////////////////

int main()
{
	setlocale(LC_ALL, "russian");
	
	int d, m;

	double sales[DISTRICTS][MONTHS] = 
	{
		{ 1432.07, 234.50, 654.01 },
		{ 322.00, 13838.32, 17589.88 },
		{ 9328.32, 934.00, 4492.30 },
		{ 12838.29, 2332.63, 32.93 }
	};
	
	cout << "וסצ	      1         2	  3";

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