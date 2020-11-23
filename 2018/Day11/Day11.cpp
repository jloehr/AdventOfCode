// Day11.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "../../Common/Common.h"

constexpr uint64_t Input = 3214;
constexpr size_t GridSize = 300;

int main()
{
	std::array<int16_t, GridSize * GridSize> AggregatedEnergyMap;
	int16_t BestPowerLevel = INT16_MIN;
	size_t BestX = SIZE_MAX;
	size_t BestY = SIZE_MAX;

	for (size_t Y = GridSize; Y > 0; Y--)
	{
		int16_t Previous = 0;
		int16_t SecondPrevious = 0;

		for (size_t X = GridSize; X > 0; X--)
		{
			int16_t CellPowerLevel = ((((((X + 10) * Y) + Input) * (X + 10)) % 1000) / 100) - 5;
			int16_t AggregatedCellPowerLevel = CellPowerLevel + Previous + SecondPrevious;

			AggregatedEnergyMap[(Y - 1) * GridSize + (X - 1)] = AggregatedCellPowerLevel;

			if (Y < GridSize)
				AggregatedCellPowerLevel += AggregatedEnergyMap[Y * GridSize + (X - 1)];

			if ((Y + 1) < GridSize)
				AggregatedCellPowerLevel += AggregatedEnergyMap[(Y + 1) * GridSize + (X - 1)];

			if (AggregatedCellPowerLevel > BestPowerLevel)
			{
				BestPowerLevel = AggregatedCellPowerLevel;
				BestX = X;
				BestY = Y;
			}

			SecondPrevious = Previous;
			Previous = CellPowerLevel;
		}
	}


	std::cout << "Part One: " << BestX << "," << BestY << std::endl;
}
