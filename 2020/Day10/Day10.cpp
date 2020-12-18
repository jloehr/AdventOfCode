// Day10.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "../../Common/Common.h"

int main()
{
	std::vector<uint16_t> Values = GetValues<uint16_t>("Input.txt");
	std::sort(std::begin(Values), std::end(Values));

	std::array<uint16_t, 4> JoltDifferences = { 0, 0, 0, 1 };
	uint16_t Joltage = 0;

	for (uint16_t Adapter : Values)
	{
		uint16_t Difference = Adapter - Joltage;
		JoltDifferences[Difference]++;
		Joltage = Adapter;
	}

	std::cout << "Part One: " << JoltDifferences[1] * JoltDifferences[3] << std::endl;
}

