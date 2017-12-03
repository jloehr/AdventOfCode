// Day03.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

constexpr uint64_t Input = 289326;

uint64_t GetUpperRingBound(const uint64_t Ring)
{
	return ((Ring * 2) + 1) * ((Ring * 2) + 1);
}

uint64_t PartOne()
{
	uint64_t Ring = 0;

	while (Input > GetUpperRingBound(Ring))
		Ring++;

	uint64_t Steps = Ring;
	uint64_t Remaining = Input - GetUpperRingBound(Ring - 1);
	if (Ring > 0)
	{
		Remaining %= (Ring * 2);
		Steps += (Remaining < Ring) ? Ring - Remaining : Remaining - Ring;
	}

	return Steps;
}

uint64_t PartTwo()
{
	enum class CalculationType { Corner, PostCorner, Edge, PreCorner};
	std::vector<uint64_t> Sequence = { 1, 1, 2, 4, 5, 10, 11, 23, 25, 26, 54, 57};
	CalculationType Calculation = CalculationType::Corner;
	size_t Offset = 9;
	size_t EdgeLength = 1;
	size_t EdgePosition = EdgeLength;
	size_t EdgeCounter = 2;

	// Corner: 1, Offset -> Offset++
	// Post Corner: 1, 2, Offset - 1, Offset -> Offset++
	// Edge (EdgeLength Times): 1, Offset - 2, Offset - 1, Offset
	// Pre Corner: 1, Offset - 1, Offset
	while (*Sequence.rbegin() < Input)
	{
		switch (Calculation)
		{
		case CalculationType::Corner:
			Sequence.push_back(*Sequence.rbegin() + *(Sequence.rbegin() + Offset));
			Offset++;
			Calculation = CalculationType::PostCorner;
			break;
		case CalculationType::PostCorner:
			Sequence.push_back(*Sequence.rbegin() + *(Sequence.rbegin() + 1) + *(Sequence.rbegin() + Offset - 1) + *(Sequence.rbegin() + Offset));
			Offset++;
			Calculation = CalculationType::Edge;
			break;
		case CalculationType::Edge:
			Sequence.push_back(*Sequence.rbegin() + *(Sequence.rbegin() + Offset - 2) + *(Sequence.rbegin() + Offset - 1) + *(Sequence.rbegin() + Offset));
			EdgePosition--;
			if (EdgePosition == 0)
			{
				EdgeCounter--;
				if (EdgeCounter == 0)
				{
					EdgeCounter = 2;
					EdgeLength++;
				}
				EdgePosition = EdgeLength;
				Calculation = CalculationType::PreCorner;
			}
			break;
		case CalculationType::PreCorner:
			Sequence.push_back(*Sequence.rbegin() + *(Sequence.rbegin() + Offset - 1) + *(Sequence.rbegin() + Offset));
			Calculation = CalculationType::Corner;
			break;
		}

		std::cout << *Sequence.rbegin() << ", ";
	}
	
	std::cout << std::endl;
	
	return *Sequence.rbegin();
}

int main()
{
	std::cout << "Part One Steps: " << PartOne() << std::endl;
	std::cout << "Part Two Number: " << PartTwo() << std::endl;

	system("pause");
    return 0;
}

