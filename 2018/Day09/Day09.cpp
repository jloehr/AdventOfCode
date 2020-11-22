// Day09.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <list>

#include "../../Common/Common.h"

constexpr size_t PlayerNumber = 418;
constexpr size_t LastMarble = 70769;


int main()
{
	std::array<uint64_t, PlayerNumber> PlayerScores { 0 };
	size_t ActivePlayer = 1;

	std::list<uint64_t> Marbles = std::list<uint64_t>({ 0 });
	auto CurrentMarble = Marbles.begin();

	for (uint64_t NextMarble = 1; NextMarble <= LastMarble; NextMarble++)
	{
		if ((NextMarble % 23) == 0)
		{
			for (size_t i = 7; i > 0; i--)
			{
				if (CurrentMarble == Marbles.begin())
					CurrentMarble = Marbles.end();

				CurrentMarble--;
			}

			PlayerScores[ActivePlayer] += NextMarble + *CurrentMarble;
			CurrentMarble = Marbles.erase(CurrentMarble);
			if (CurrentMarble == Marbles.end())
				CurrentMarble = Marbles.begin();
		}
		else
		{
			for (size_t i = 2; i > 0; i--)
			{
				CurrentMarble++;
				if (CurrentMarble == Marbles.end())
					CurrentMarble = Marbles.begin();
			}

			CurrentMarble = Marbles.insert(CurrentMarble, NextMarble);
		}

		++ActivePlayer %= PlayerNumber;
	}

	std::cout << "Part One: " << *std::max_element(PlayerScores.cbegin(), PlayerScores.cend()) << std::endl;
}
