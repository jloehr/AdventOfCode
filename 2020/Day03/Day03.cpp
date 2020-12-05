// Day03.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "../../Common/Common.h"

int main()
{
	StringVector Lines = GetFileLines("Input.txt");
	std::pair<size_t, size_t> CurrentPosition = { 0, 0 };
	std::pair<size_t, size_t> Velocity = { 3, 1 };
	size_t TreeCount = 0;

	while (CurrentPosition.second < Lines.size())
	{
		CurrentPosition.first %= Lines[CurrentPosition.second].size();

		if (Lines[CurrentPosition.second][CurrentPosition.first] == '#')
			TreeCount++;

		CurrentPosition.first += Velocity.first;
		CurrentPosition.second += Velocity.second;
	}

	std::cout << "Part One: " << TreeCount << std::endl;
}

