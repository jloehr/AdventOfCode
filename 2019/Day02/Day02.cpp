// Day02.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

int main()
{
	std::string File = GetFileLines("Input.txt")[0];
	std::vector<size_t> Intcodes;

	std::regex NumberPattern("\\d+");
	auto IntcodesMatches = std::sregex_iterator(File.cbegin(), File.cend(), NumberPattern);
	std::transform(IntcodesMatches, std::sregex_iterator(), std::back_inserter(Intcodes), [](std::smatch IntcodesMatch) -> size_t { return static_cast<size_t>(std::atoll(IntcodesMatch.str().c_str())); });

	auto ProgrammPointer = Intcodes.begin();
	bool Running = true;

	Intcodes[1] = 12;
	Intcodes[2] = 2;

	while (Running)
	{
		switch (*ProgrammPointer)
		{
		case 1:
			Intcodes[*(ProgrammPointer + 3)] = Intcodes[*(ProgrammPointer + 1)] + Intcodes[*(ProgrammPointer + 2)];
			break;
		case 2:
			Intcodes[*(ProgrammPointer + 3)] = Intcodes[*(ProgrammPointer + 1)] * Intcodes[*(ProgrammPointer + 2)];
			break;
		case 99:
			Running = false;
			std::cout << "Halting" << std::endl;
			break;
		default:
			Running = false;
			std::cout << "Error" << std::endl;
			break;
		}

		ProgrammPointer += 4;
	}

	std::cout << "Part One:" << Intcodes[0] << std::endl;
}
