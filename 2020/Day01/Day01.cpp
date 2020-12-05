// Day01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "../../Common/Common.h"

int main()
{
	std::vector<uint16_t> Values = GetValues<uint16_t>("Input.txt");

	std::sort(Values.begin(), Values.end());
	auto LowValue = Values.begin();
	auto HighValue = Values.end() - 1;

	while(HighValue != LowValue)
	{
		uint16_t Sum = *LowValue + *HighValue;

		if (Sum == 2020)
			break;
		else if (Sum > 2020)
			HighValue--;
		else
			LowValue++;
	}

	if (HighValue == LowValue)
		std::cout << "No Solution found!" << std::endl;
	else
		std::cout << "Part One: " << (*HighValue * *LowValue) << std::endl;
}