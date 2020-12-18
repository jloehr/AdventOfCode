// Day10.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "../../Common/Common.h"

int main()
{
	std::vector<uint16_t> Values = GetValues<uint16_t>("Input.txt");
	std::sort(std::begin(Values), std::end(Values));
	Values.push_back((*Values.rbegin()) + 3);

	std::array<uint16_t, 4> JoltDifferences = { 0, 0, 0, 1 };
	std::array<uint16_t, 6> OneDiffCombinations = { 1, 2, 4, 7, 13, 24 };
	uint16_t Joltage = 0;
	size_t OneDiffSeries = 0;
	size_t Combinations = 1;

	for (uint16_t Adapter : Values)
	{
		uint16_t Difference = Adapter - Joltage;
		JoltDifferences[Difference]++;
		Joltage = Adapter;

		if (Difference == 1)
		{
			OneDiffSeries++;
		} 
		else if ((Difference == 3) && (OneDiffSeries > 0))
		{
			if (OneDiffSeries > OneDiffCombinations.size())
			{
				std::cout << "Greater series of one-diffs than hardcoded!" << std::endl;
				return -1;
			}

			Combinations *= OneDiffCombinations[OneDiffSeries - 1];
			OneDiffSeries = 0;
		}
	}

	std::cout << "Part One: " << JoltDifferences[1] * (JoltDifferences[3] -1) << std::endl;
	std::cout << "Part Two: " << Combinations << std::endl;
}

