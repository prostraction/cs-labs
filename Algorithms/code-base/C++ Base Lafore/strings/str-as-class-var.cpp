#include <iostream>
#include <cstring>

using namespace std;

//////////////////////////////////////////////////////
class part
{
private:
	char partname[30];	// название
	int partnumber;		// номер
	double cost;		// цена
public:
	void setpart(char pname[], int pn, double c)
	{
		strcpy_s(partname, pname);
		partnumber = pn;
		cost = c;
	}
	void showpart()
	{
		setlocale(LC_ALL, "russian");
		cout << "Название " << partname;
		cout << ", номер " << partnumber;
		cout << ", цена $" << cost;
	}
};
//////////////////////////////////////////////////////
int main()
{
	part part1, part2;
	setlocale(LC_ALL, "russian");

	part1.setpart("муфта", 4473, 217.55);
	part2.setpart("вороток", 9924, 419.25);

	cout << "Первая деталь: "; part1.showpart();
	cout << "\nВторая деталь: "; part2.showpart();
	
	cout << endl;
	system("pause");
	return 0;
}