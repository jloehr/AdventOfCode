// Day02.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

typedef std::pair<signed, signed> Coordinate;

void PartOne(const StringVector & Lines);
void PartTwo(const StringVector & Lines);

int main()
{
	StringVector Lines = GetFileLines("Input.txt");

	PartOne(Lines);
	PartTwo(Lines);

	system("pause");
}

void PartOne(const StringVector & Lines)
{
	std::cout << "Part One: ";

	for (const std::string & Line : Lines)
	{
		Coordinate Position(1, 1);

		for (const char & Instruction : Line)
		{
			switch (Instruction)
			{
			case 'U':
				if (Position.second > 0)
				{
					Position.second--;
				}
				break;
			case 'D':
				if (Position.second < 2)
				{
					Position.second++;
				}
				break;
			case 'L':
				if (Position.first > 0)
				{
					Position.first--;
				}
				break;
			case 'R':
				if (Position.first < 2)
				{
					Position.first++;
				}
				break;
			}
		}

		signed Keypad = (Position.second * 3) + Position.first + 1;
		std::cout << Keypad;
	}

	std::cout << std::endl;
}

bool IsValid(signed X, signed Y)
{
	if ((X < 0) || (X > 4) || (Y < 0) || (Y > 4))
	{
		return false;
	}

	signed dX = std::abs(X - 2);
	signed dY = std::abs(Y - 2);

	return ((dX + dY) <= 2);
}

void PartTwo(const StringVector & Lines)
{
	static const std::string Keypad("**1***234*56789*ABC***D**");

	std::cout << "Part Two: ";
	Coordinate Position(0, 2);

	for (const std::string & Line : Lines)
	{
		for (const char & Instruction : Line)
		{
			switch (Instruction)
			{
			case 'U':
				if (IsValid(Position.first, Position.second - 1))
				{
					Position.second--;
				}
				break;
			case 'D':
				if (IsValid(Position.first, Position.second + 1))
				{
					Position.second++;
				}
				break;
			case 'L':
				if (IsValid(Position.first - 1, Position.second))
				{
					Position.first--;
				}
				break;
			case 'R':
				if (IsValid(Position.first + 1, Position.second))
				{
					Position.first++;
				}
				break;
			}
		}


		size_t Key = Position.second * 5 + Position.first;
		std::cout <<  Keypad[Key];
	}

	std::cout << std::endl;
}