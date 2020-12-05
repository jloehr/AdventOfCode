// Day11.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "../../Common/Common.h"

constexpr uint64_t Input = 3214;
constexpr size_t GridSize = 300;

typedef std::array<int32_t, GridSize* GridSize> GridArray;

struct Square
{
	int32_t PowerLevel;
	size_t X;
	size_t Y;
	size_t Size;
};

GridArray SummedPowerMap;

int main()
{
	Square Best3x3 = { std::numeric_limits<size_t>::max(), std::numeric_limits<size_t>::max(), 3, std::numeric_limits<int32_t>::min() };
	Square BestDynamic = { std::numeric_limits<size_t>::max(), std::numeric_limits<size_t>::max(), std::numeric_limits<size_t>::max(), std::numeric_limits<int32_t>::min() };
	auto GetIndex = [&](size_t X, size_t Y) -> size_t { return (Y - 1) * GridSize + (X - 1); };
	auto GetPower = [&](GridArray::difference_type X, GridArray::difference_type Y) -> int32_t { return ((X >= 1) && (Y >= 1)) ? SummedPowerMap[GetIndex(X, Y)] : 0; };

	for (size_t Y = 1; Y <= GridSize; Y++)
	{
		for (size_t X = 1; X <= GridSize; X++)
		{
			int32_t CellPowerLevel = ((((((X + 10) * Y) + Input) * (X + 10)) % 1000) / 100) - 5;
			SummedPowerMap[GetIndex(X,Y)] = CellPowerLevel + GetPower(X - 1, Y ) + GetPower(X, Y - 1) - GetPower(X - 1, Y - 1);

			for (size_t Size = 1; Size <= std::min(Y, X); Size++)
			{
				int32_t PowerLevel = GetPower(X,Y) - GetPower(X - Size, Y) - GetPower(X, Y - Size) + GetPower(X - Size, Y - Size);

				if (PowerLevel > BestDynamic.PowerLevel)
				{
					BestDynamic.PowerLevel = PowerLevel;
					BestDynamic.X = X - Size + 1;
					BestDynamic.Y = Y - Size + 1;
					BestDynamic.Size = Size;
				}

				if ((Size == 3) && (PowerLevel > Best3x3.PowerLevel))
				{
					Best3x3.PowerLevel = PowerLevel;
					Best3x3.X = X - Size + 1;
					Best3x3.Y = Y - Size + 1;
				}
			}
		}
	}

	std::cout << "Part One: " << Best3x3.X << "," << Best3x3.Y << std::endl;
	std::cout << "Part Two: " << BestDynamic.X << "," << BestDynamic.Y << "," << BestDynamic.Size << std::endl;
}
