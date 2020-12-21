// Day15.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <unordered_map>

#include "../../Common/Common.h"

constexpr uint64_t PartOneRounds = 2020;
constexpr uint64_t PartTwoRounds = 30000000;
const std::vector<uint64_t> Input = { 14,3,1,0,9,5 };

std::array<std::pair<uint64_t, uint64_t>, std::max(PartOneRounds, PartTwoRounds)> NumbersSpoken;

int main()
{
	uint64_t Round = 1;
	uint64_t PartOne = 0;

	for (uint64_t StartingNumber : Input) NumbersSpoken[StartingNumber] = { Round++, 0 };
	uint64_t LastNumber = *Input.crbegin();
	for (; Round <= std::max(PartOneRounds, PartTwoRounds); Round++)
	{
		uint64_t NewNumber = NumbersSpoken[LastNumber].second;
		bool FirstTime = NumbersSpoken[NewNumber].first == 0;

		NumbersSpoken[NewNumber] = { Round, FirstTime ? 0 : (Round - NumbersSpoken[NewNumber].first) };
		LastNumber = NewNumber;

		if (Round == PartOneRounds) PartOne = LastNumber;
	}

	std::cout << "Part One: " << PartOne << std::endl;
	std::cout << "Part Two: " << LastNumber << std::endl;
}
