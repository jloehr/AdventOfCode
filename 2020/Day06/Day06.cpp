// Day06.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <bitset>

#include "../../Common/Common.h"

int main()
{
	StringVector Lines = GetFileLines("Input.txt");
	Lines.push_back("");

	std::bitset<26> GroupAny;
	std::bitset<26> GroupAll;
	size_t SumAny = 0;
	size_t SumAll = 0;

	GroupAll.set();

	for (const std::string& Line : Lines)
	{
		if (Line.size() > 0)
		{
			std::bitset<26> Person;

			for (const char Character : Line)
				Person.set(static_cast<size_t>(Character) - 'a');

			GroupAny |= Person;
			GroupAll &= Person;
		}
		else
		{
			SumAny += GroupAny.count();
			SumAll += GroupAll.count();
			GroupAny.reset();
			GroupAll.set();
		}
	}

	std::cout << "Part One: " << SumAny << std::endl;
	std::cout << "Part Two: " << SumAll << std::endl;
}