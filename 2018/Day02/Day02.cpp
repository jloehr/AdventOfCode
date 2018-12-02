// Day02.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

int main()
{
	StringVector File = GetFileLines("Input.txt");
	unsigned int Doubles = 0;
	unsigned int Triples = 0;

	for (std::string Line : File)
	{
		std::unordered_map<std::string::value_type, unsigned int> Appearances;
		std::for_each(Line.begin(), Line.end(), [&](const std::string::value_type & character) { ++Appearances[character]; });

		bool HasDouble = false;
		bool HasTripple = false;

		for (std::pair<std::string::value_type, unsigned int> Count : Appearances)
		{
			if (Count.second == 2)
				HasDouble = true;

			if (Count.second == 3)
				HasTripple = true;

			if (HasDouble && HasTripple)
				break;
		}

		if (HasDouble)
			++Doubles;
		if (HasTripple)
			++Triples;
	}

	std::cout << "Part One: " << (Doubles * Triples) << std::endl;
}
