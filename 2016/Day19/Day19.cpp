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
	double ElfLog = std::log2(ElveCount);
	uint64_t ElfLowerBound = static_cast<uint64_t>(std::pow(2.0, std::floor(ElfLog)));

	return (((ElveCount - ElfLowerBound) * 2) + 1);
}

uint64_t PartTwo(uint64_t ElveCount)
{
	double ElfLog = std::log2(ElveCount) / std::log2(3);
	uint64_t ElfLowerBound = static_cast<uint64_t>(std::pow(3.0, std::floor(ElfLog)));

	ElveCount -= ElfLowerBound;
	if (ElveCount < ElfLowerBound)
	{
		return ElveCount;
	}
	else
	{
		return ((ElveCount - ElfLowerBound) * 2) + ElfLowerBound;
	}
}
