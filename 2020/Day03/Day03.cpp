// Day03.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <numeric>

#include "../../Common/Common.h"

constexpr size_t PartOneIndex = 1;

typedef std::pair<size_t, size_t> Coordinate;

size_t Traverse(const StringVector& Map, const Coordinate& Velocity);

int main()
{
	const StringVector Lines = GetFileLines("Input.txt");
	const std::vector<Coordinate> Slopes = {
		{1,1},
		{3,1},
		{5,1},
		{7,1},
		{1,2},
	};

	std::vector<size_t> TreeCounts;
	std::transform(Slopes.cbegin(), Slopes.cend(), std::back_inserter(TreeCounts), [&](const Coordinate& Velocity) -> size_t { return Traverse(Lines, Velocity); });

	std::cout << "Part One: " << TreeCounts[PartOneIndex] << std::endl;
	std::cout << "Part Two: " << std::accumulate(TreeCounts.cbegin(), TreeCounts.cend(), static_cast<size_t>(1), std::multiplies<size_t>()) << std::endl;
}

size_t Traverse(const StringVector& Map, const Coordinate& Velocity)
{
	Coordinate CurrentPosition = { 0, 0 };
	size_t TreeCount = 0;

	while (CurrentPosition.second < Map.size())
	{
		CurrentPosition.first %= Map[CurrentPosition.second].size();

		if (Map[CurrentPosition.second][CurrentPosition.first] == '#')
			TreeCount++;

		CurrentPosition.first += Velocity.first;
		CurrentPosition.second += Velocity.second;
	}

	return TreeCount;
}
