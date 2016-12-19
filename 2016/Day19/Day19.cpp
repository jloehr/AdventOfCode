// Day19.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

constexpr uint64_t Input = 3005290;

uint64_t GetElf(uint64_t ElveCount);

int main()
{
	std::cout << "Winning Elf: " << GetElf(Input) << std::endl;

	system("pause");

    return 0;
}

uint64_t GetElf(uint64_t ElveCount)
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