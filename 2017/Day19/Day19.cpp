// Day19.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int main()
{
	const StringVector Map = GetFileLines("Input.txt");

	std::pair<size_t, size_t> Position{ 0, Map[0].find('|') };
	std::pair<int, int> Movement{ 1,0 };
	std::string Letters;
	size_t StepCount = 0;
	bool ReachedEnd = false;

	while (!ReachedEnd)
	{
		Position.first += Movement.first;
		Position.second += Movement.second;
		StepCount++;

		const char Symbol = Map[Position.first][Position.second];
		switch (Symbol)
		{
		case'|':
		case'-':
			break;
		case ' ':
			std::cout << "Off the path!" << std::endl;
			ReachedEnd = true;
			break;
		case '+':
			if (Movement.first != 0)
			{
				Movement.first = 0;
				Movement.second = (Map[Position.first][Position.second - 1] == '-') ? -1 : 1;
			}
			else
			{
				Movement.first = (Map[Position.first - 1][Position.second] == '|') ? -1 : 1;
				Movement.second = 0;
			}
			break;
		default:
			Letters.push_back(Symbol);
			break;
		}
	}

	std::cout << "Word along the path: " << Letters << std::endl;
	std::cout << "Steps along the path: " << StepCount << std::endl;

	system("pause");
    return 0;
}

