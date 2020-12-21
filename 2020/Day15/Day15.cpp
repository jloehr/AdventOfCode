// Day15.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <unordered_map>

#include "../../Common/Common.h"

constexpr uint64_t Rounds = 2020;
const std::vector<uint64_t> Input = { 14,3,1,0,9,5 };

int main()
{
	std::unordered_map<uint64_t, std::pair<uint64_t, uint64_t>> NumbersSpoken;
	uint64_t Round = 1;

	for (uint64_t StartingNumber : Input) NumbersSpoken[StartingNumber] = { Round++, 0 };
	uint64_t LastNumber = *Input.crbegin();
	for (; Round <= Rounds; Round++)
	{
		uint64_t NewNumber = NumbersSpoken[LastNumber].second;
		bool FirstTime = NumbersSpoken.find(NewNumber) == std::cend(NumbersSpoken);

		NumbersSpoken[NewNumber] = { Round, FirstTime ? 0 : (Round - NumbersSpoken[NewNumber].first) };
		LastNumber = NewNumber;
	}

	std::cout << "Part One: " << LastNumber << std::endl;
}
