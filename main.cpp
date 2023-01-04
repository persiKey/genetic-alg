#include <iostream>
#include <array>
#include <time.h>
#include <random>
#include "main.h"
#include "fstream"

using std::cin;
using std::cout;

int main()
{
	srand(time(NULL));

	Population p1;
	std::ofstream file("data.csv");
	file << "Iteration , Max F Value\n";
	for (size_t i = 0; i <= 1000; i++)
	{
		if (!(i % 20))
		{
			file << i << " , " << p1.GetMaxWorth() << '\n';
			cout << p1.GetMaxWorth() << '\n';
		}
		p1.Iterate();
	}
	p1.PrintMax();
	file.close();
	char key;
	int num;
	do
	{
		cin >> key;
		switch (key)
		{
		case 'i':
		case 'I': p1.Iterate(); p1.PrintMax(); break;
		case 'o':
		case 'O': cout << "Enter object number:"; cin >> num; p1.PrintObject(num); break;
		case 'p':
		case 'P': cout << "Enter individ number:"; cin >> num; p1.PrintIndivid(num); break;
		case 'a':
		case 'A': p1.PrintWorthValue(); break;
		default:
			break;
		}
	} while (key != 'q' && key != 'Q');
	return 0;
}
