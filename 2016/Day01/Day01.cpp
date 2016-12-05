// Day01.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

enum class Direction { North, East, South, West };
typedef std::pair<signed, signed> Coordinate;
typedef std::set<Coordinate> CoordinateSet;
typedef std::vector<Coordinate> CoordinateVector;

void Rotate(char Instruction, Direction & Heading);
void RotateRight(Direction & Heading);
void RotateLeft(Direction & Heading);
void Advance(Coordinate & Position, const Direction Heading, int Steps, CoordinateSet & History, CoordinateVector & Crossings);

int main()
{
	StringVectorVector File = GetFileLineParts("Input.txt");

	for (const StringVector & Line : File)
	{
		Direction Heading = Direction::North;
		Coordinate Position(0, 0);
		CoordinateSet History({ Position });
		CoordinateVector Crossings;

		for (std::string Instruction : Line)
		{
			Rotate(Instruction[0], Heading);

			int Steps = std::atoi(Instruction.c_str() + 1);

			Advance(Position, Heading, Steps, History, Crossings);
		}

		std::cout << "Distance: " << (std::abs(Position.first) + std::abs(Position.second)) << std::endl;
		if (!Crossings.empty())
		{
			std::cout << "Actual Distance: " << (std::abs(Crossings[0].first) + std::abs(Crossings[0].second)) << std::endl;
		}
	}

	system("Pause");

    return 0;
}

void Rotate(char Instruction, Direction & Heading)
{
	switch (Instruction)
	{
	case 'R':
		RotateRight(Heading);
		break;
	case 'L':
		RotateLeft(Heading);
		break;
	default:
		std::cout << "Something went wrong" << std::endl;
		exit(1);
	}
}

void RotateRight(Direction & Heading)
{
	switch (Heading)
	{
	case Direction::North:
		Heading = Direction::East;
		break;
	case Direction::East:
		Heading = Direction::South;
		break;
	case Direction::South:
		Heading = Direction::West;
		break;
	case Direction::West:
		Heading = Direction::North;
		break;
	}
}

void RotateLeft(Direction & Heading)
{
	switch (Heading)
	{
	case Direction::North:
		Heading = Direction::West;
		break;
	case Direction::East:
		Heading = Direction::North;
		break;
	case Direction::South:
		Heading = Direction::East;
		break;
	case Direction::West:
		Heading = Direction::South;
		break;
	}
}

void Advance(Coordinate & Position, const Direction Heading, int Steps, CoordinateSet & History, CoordinateVector & Crossings)
{
	std::function<void()> Step = nullptr;

	switch (Heading)
	{
	case Direction::North:
		Step = [&]() { Position.second++; };
		break;
	case Direction::East:
		Step = [&]() { Position.first++; };
		break;
	case Direction::South:
		Step = [&]() { Position.second--; };
		break;
	case Direction::West:
		Step = [&]() { Position.first--; };
		break;
	}

	for (int i = 0; i < Steps; i++)
	{
		Step();
		auto Result = History.insert(Position);

		if (!Result.second)
		{
			Crossings.push_back(Position);
		}
	}

}