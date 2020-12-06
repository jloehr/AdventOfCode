// Day05.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <set>

#include "../../Common/Common.h"

int main()
{
	StringVector Lines = GetFileLines("Input.txt");
	size_t HighestSeatID = 0;
	size_t YourSeatID = 0;
	std::set<size_t> TakenSeats;

	for (const std::string& Line : Lines)
	{
		size_t SeatID = 0;
		for (const char Character : Line)
		{
			SeatID <<= 1;
			switch (Character)
			{
			case 'B':
			case 'R':
				SeatID++;
			}
		}

		TakenSeats.insert(SeatID);

		if (SeatID > HighestSeatID)
			HighestSeatID = SeatID;
	}

	for (auto It = TakenSeats.cbegin(); It != TakenSeats.cend();)
	{
		size_t SeatID = *It;
		size_t NextSeatID = *(++It);

		if (SeatID == (NextSeatID - 2))
		{	
			YourSeatID = SeatID + 1;
			break;
		}
	}

	std::cout << "Part One:" << HighestSeatID << std::endl;
	std::cout << "Part To:" << YourSeatID << std::endl;
}
