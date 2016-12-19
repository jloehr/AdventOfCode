// Day19.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

constexpr uint64_t Input = 3005290;

uint64_t PartOne(uint64_t ElveCount);
uint64_t PartTwo(uint64_t ElveCount);

int main()
{
	std::cout << "Part One Elf: " << PartOne(Input) << std::endl;
	std::cout << "Part Two Elf: " << PartTwo(Input) << std::endl;
	system("pause");

    return 0;
}

uint64_t PartOne(uint64_t ElveCount)
{
	uint64_t ElfIndex = 0;
	uint64_t Offset = 1;

	while (ElveCount != 1)
	{
		bool Even = (ElveCount % 2) == 0;
		ElveCount /= 2;
		Offset *= 2;

		if (!Even)
		{
			ElfIndex += Offset;
		}
	}

	return ElfIndex + 1;
}

uint64_t PartTwo(uint64_t ElveCount)
{
	double ElveLog = std::log2(ElveCount) / std::log2(3);
	uint64_t ElveLowerBound = static_cast<uint64_t>(std::pow(3.0, std::floor(ElveLog)));

	ElveCount -= ElveLowerBound;
	if (ElveCount < ElveLowerBound)
	{
		return ElveCount;
	}
	else
	{
		return ((ElveCount - ElveLowerBound) * 2) + ElveLowerBound;
	}
}

