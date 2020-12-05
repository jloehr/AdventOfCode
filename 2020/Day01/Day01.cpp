// Day01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "../../Common/Common.h"

int main()
{
	std::vector<uint16_t> Values = GetValues<uint16_t>("Input.txt");
	std::sort(Values.begin(), Values.end());
	auto LowValue = Values.cbegin();
	auto HighValue = Values.cend() - 1;

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
		std::cout << "Part One: No Solution found!" << std::endl;
	else
		std::cout << "Part One: " << (*HighValue * *LowValue) << std::endl;

	auto MidValue = LowValue;
	for(;HighValue != Values.cbegin(); HighValue--)
	{
		while (((*HighValue + *MidValue) < 2020) && (HighValue > MidValue))
			MidValue++;

		while (((*HighValue + *MidValue) > 2020) && (MidValue > Values.cbegin()))
			MidValue--;

		if ((HighValue == MidValue) || (MidValue == Values.cbegin()))	
			continue;

		for(auto MovingMidValue = MidValue; MovingMidValue != Values.cbegin(); MovingMidValue--)
		{
			LowValue = Values.cbegin();
			while ((LowValue != MovingMidValue) && ((*HighValue + *MovingMidValue + *LowValue) < 2020))
				LowValue++;

			if ((*HighValue + *MovingMidValue + *LowValue) == 2020)
			{
				std::cout << "Part Two: " << (*HighValue * *MovingMidValue  * *LowValue) << std::endl;
				return 0;
			}
		}
	}

	std::cout << "Part Two: No Solution found!" << std::endl;
}