// Day22.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

constexpr size_t PartOneIterations = 10000;
constexpr size_t PartTwoIterations = 10000000;

using Position = std::pair<int64_t, int64_t>;
enum class Cell { Clean, Weakened, Infected, Flagged };

size_t Simulate(std::map<Position, Cell> Map, bool MultiState, size_t Iterations)
{
	Position Pos = { 0, 0 };
	Position Direction{ 0, -1 };
	uint64_t InfectionBurst = 0;

	for (auto Burst = 0; Burst < Iterations; ++Burst)
	{
		auto Result = Map.find(Pos);
		if (Result == std::end(Map))
		{
			Result = Map.insert(std::make_pair(Pos, Cell::Clean)).first;
		}

		switch (Result->second)
		{
		case Cell::Clean:
			// turn left
			std::swap(Direction.first, Direction.second);
			Direction.second *= -1;
			Result->second = MultiState ? Cell::Weakened : Cell::Infected;
			break;
		case Cell::Weakened: 
			Result->second = Cell::Infected;
			break;
		case Cell::Infected:
			// turn right
			std::swap(Direction.first, Direction.second);
			Direction.first *= -1;
			Result->second = MultiState ? Cell::Flagged : Cell::Clean;
			break;
		case Cell::Flagged:
			// reverse
			Direction.first *= -1;
			Direction.second *= -1;
			Result->second = Cell::Clean;
			break;
		}

		InfectionBurst += Result->second == Cell::Infected ? 1 : 0;

		Pos.first += Direction.first;
		Pos.second += Direction.second;
	}

	return InfectionBurst;
}

int main()
{
	const StringVector Lines = GetFileLines("Input.txt");
	std::map<Position, Cell> Map;
	Position Pos{ 0 , -1 * (Lines.size()/2) };

	for (const auto & Line : Lines)
	{
		Pos.first = -1 * (Line.size() / 2);
		for (char Cell : Line)
		{
			Map.insert(std::make_pair(Pos, (Cell == '#') ? Cell::Infected : Cell::Clean));
			Pos.first++;
		}
		Pos.second++;
	}

	std::cout << "Burst of Infection part one: " << Simulate(Map, false, PartOneIterations) << std::endl;
	std::cout << "Burst of Infection part two: " << Simulate(Map, true, PartTwoIterations) << std::endl;

	system("pause");
    return 0;
}

