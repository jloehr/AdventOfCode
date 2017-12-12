// Day11.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int StepsAway(const std::pair<int, int> & Position)
{
	return ((std::abs(Position.first) + std::abs(Position.second)) / 2);
}

int main()
{
	const StringVector Lines = GetFileLines("Input.txt");
	const std::string & Input = Lines[0];
	auto Position = std::make_pair( 0, 0 );
	int MaxSteps = 0;
	
	
	for (auto It = std::begin(Input); It != std::end(Input);)
	{
		int X = 0;
		int Y = (*(It++) == 'n') ? 2 : -2;

		if(It != std::end(Input) && (*It != ','))
		{
			X = (*(It++) == 'e') ? 1 : -1;
			Y /= 2;
		}

		if (It != std::end(Input))
			++It;

		Position.first += X;
		Position.second += Y;

		MaxSteps = std::max(MaxSteps, StepsAway(Position));
	}

	std::cout << "Steps: " << StepsAway(Position) << std::endl;
	std::cout << "Max steps away: " << MaxSteps << std::endl;

	system("pause");
    return 0;
}

