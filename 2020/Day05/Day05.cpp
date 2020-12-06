// Day05.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "../../Common/Common.h"

int main()
{
	StringVector Lines = GetFileLines("Input.txt");
	size_t HighestSeatID = 0;

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

		if (SeatID > HighestSeatID)
			HighestSeatID = SeatID;
	}

	std::cout << "Part One:" << HighestSeatID << std::endl;
}
