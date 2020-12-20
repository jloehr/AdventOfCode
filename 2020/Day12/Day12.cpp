// Day12.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "../../Common/Common.h"

typedef std::pair<int64_t, int64_t> Coordinate;

const std::array<Coordinate, 4> Directions = { {{0, 1}, {1, 0}, {0, -1},{-1, 0}} };

uint64_t RunPartOne(const StringVector& Lines);
uint64_t RunPartTwo(const StringVector& Lines);

int main()
{
	const StringVector Lines = GetFileLines("Input.txt");

	std::cout << "Part One: " << RunPartOne(Lines) << std::endl;
	std::cout << "Part Two: " << RunPartTwo(Lines) << std::endl;
}

uint64_t RunPartOne(const StringVector& Lines)
{
	Coordinate Position = { 0, 0 };
	auto CurrentDirection = std::cbegin(Directions) + 1;

	for (const auto& Line : Lines)
	{
		int64_t Distance = std::stoll(Line.substr(1));
		Coordinate Direction;
		switch (Line[0])
		{
		case 'N':
			Direction = Directions[0];
			break;
		case 'E':
			Direction = Directions[1];
			break;
		case 'S':
			Direction = Directions[2];
			break;
		case 'W':
			Direction = Directions[3];
			break;
		case 'F':
			Direction = *CurrentDirection;
			break;
		case 'R':
			for (; Distance > 0; Distance -= 90) CurrentDirection = ((CurrentDirection + 1) == std::cend(Directions)) ? std::cbegin(Directions) : (CurrentDirection + 1);
			if (Distance != 0) std::cout << "Error: None 90° Rotation found!" << std::endl;
			Distance = 0;
			break;
		case 'L':
			for (; Distance > 0; Distance -= 90) CurrentDirection = (CurrentDirection == std::cbegin(Directions)) ? (std::cend(Directions) - 1) : CurrentDirection - 1;
			if (Distance != 0) std::cout << "Error: None 90° Rotation found!" << std::endl;
			Distance = 0;
			break;
		}

		Position.first += Direction.first * Distance;
		Position.second += Direction.second * Distance;
	}

	return (std::abs(Position.first) + std::abs(Position.second));
}

uint64_t RunPartTwo(const StringVector& Lines)
{
	Coordinate Position = { 0, 0 };
	Coordinate Waypoint = { 10, 1 };

	for (const auto& Line : Lines)
	{
		int64_t Distance = std::stoll(Line.substr(1));
		Coordinate Direction;

		switch (Line[0])
		{
		case 'N':
			Direction = Directions[0];
			break;
		case 'E':
			Direction = Directions[1];
			break;
		case 'S':
			Direction = Directions[2];
			break;
		case 'W':
			Direction = Directions[3];
			break;
		case 'F':
			Position.first += Waypoint.first * Distance;
			Position.second += Waypoint.second * Distance;
			break;
		case 'R':
			for (; Distance > 0; Distance -= 90)
			{
				std::swap(Waypoint.first, Waypoint.second);
				Waypoint.second *= -1;
			}
			if (Distance != 0) std::cout << "Error: None 90° Rotation found!" << std::endl;
			Distance = 0;
			break;
		case 'L':
			for (; Distance > 0; Distance -= 90)
			{
				std::swap(Waypoint.first, Waypoint.second);
				Waypoint.first *= -1;
			}
			if (Distance != 0) std::cout << "Error: None 90° Rotation found!" << std::endl;
			Distance = 0;
			break;
		}

		Waypoint.first += Direction.first * Distance;
		Waypoint.second += Direction.second * Distance;
	}

	return (std::abs(Position.first) + std::abs(Position.second));
}