// Day11.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int main()
{
	const StringVector Lines = GetFileLines("Input.txt");
	const std::string & Input = Lines[0];
	auto Position = std::make_pair( 0, 0 );
	
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
	}

	std::cout << "Steps: " << ((std::abs(Position.first) + std::abs(Position.second)) / 2 ) << std::endl;

	system("pause");
    return 0;
}

