// Day02.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

typedef std::pair<unsigned, unsigned> Coordinate;

int main()
{
	StringVector Lines = GetFileLines("Input.txt");

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
	system("pause");
}

