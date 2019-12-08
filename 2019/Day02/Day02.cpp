// Day02.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

int main()
{
	std::string File = GetFileLines("Input.txt")[0];
	std::vector<size_t> Intcodes;
	const size_t PartTwoOutput = 19690720;

	std::regex NumberPattern("\\d+");
	auto IntcodesMatches = std::sregex_iterator(File.cbegin(), File.cend(), NumberPattern);
	std::transform(IntcodesMatches, std::sregex_iterator(), std::back_inserter(Intcodes), [](std::smatch IntcodesMatch) -> size_t { return static_cast<size_t>(std::atoll(IntcodesMatch.str().c_str())); });

	size_t PartOne = SIZE_MAX;
	size_t PartTwo = SIZE_MAX;

	for(size_t Noun = 0; Noun <= 99; Noun++)
	{
		for (size_t Verb = 0; Verb <= 99; Verb++)
		{
			// Skip if part two is solved but not part one.
			if ((PartTwo != SIZE_MAX) && (Noun != 12) && (Verb != 2))
				continue;

			std::vector<size_t> Memory = std::vector<size_t>(Intcodes);
			auto ProgrammPointer = Memory.begin();
			bool Running = true;

			Memory[1] = Noun;
			Memory[2] = Verb;

			while (Running)
			{
				switch (*ProgrammPointer)
				{
				case 1:
					Memory[*(ProgrammPointer + 3)] = Memory[*(ProgrammPointer + 1)] + Memory[*(ProgrammPointer + 2)];
					break;
				case 2:
					Memory[*(ProgrammPointer + 3)] = Memory[*(ProgrammPointer + 1)] * Memory[*(ProgrammPointer + 2)];
					break;
				case 99:
					Running = false;
					break;
				default:
					Running = false;
					break;
				}

				ProgrammPointer += 4;
			}

			if ((Noun == 12) && (Verb == 2))
				PartOne = Memory[0];

			if (Memory[0] == PartTwoOutput)
				PartTwo = 100 * Noun + Verb;

			if ((PartOne != SIZE_MAX) && (PartTwo != SIZE_MAX))
				break;
		}
	}

	std::cout << "Part One:" << PartOne << std::endl;
	std::cout << "Part Two:" << PartTwo << std::endl;
}
