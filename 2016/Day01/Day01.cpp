// Day01.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

enum class Direction { North, East, South, West };

void Rotate(char Instruction, Direction & Heading);
void RotateRight(Direction & Heading);
void RotateLeft(Direction & Heading);
void Advance(std::pair<signed, signed> & Position, const Direction Heading, int Steps);

int main()
{
	StringVectorVector File = GetFileLineParts("Input.txt");

	for (const StringVector & Line : File)
	{
		Direction Heading = Direction::North;
		std::pair<signed, signed> Position(0, 0);

		for (std::string Instruction : Line)
		{
			Rotate(Instruction[0], Heading);

			int Steps = std::atoi(Instruction.c_str() + 1);

			Advance(Position, Heading, Steps);
		}

		std::cout << "Distance: " << (std::abs(Position.first) + std::abs(Position.second)) << std::endl;
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

void Advance(std::pair<signed, signed> & Position, const Direction Heading, int Steps)
{
	switch (Heading)
	{
	case Direction::North:
		Position.second += Steps;
		break;
	case Direction::East:
		Position.first += Steps;
		break;
	case Direction::South:
		Position.second -= Steps;
		break;
	case Direction::West:
		Position.first -= Steps;
		break;
	}
}