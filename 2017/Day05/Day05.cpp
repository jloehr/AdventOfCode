// Day05.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

size_t Emulate(std::vector<int> Instructions, bool SpecialJumps = false);

int main()
{
	const StringVector Lines = GetFileLines("Input.txt");
	std::vector<int> Instructions(Lines.size());
	std::transform(Lines.begin(), Lines.end(), Instructions.begin(), [](const auto & String) -> int { return std::stoi(String); });

	std::cout << "Jumps part one: " << Emulate(Instructions, false) << std::endl;
	std::cout << "Jumps part two: " << Emulate(Instructions, true) << std::endl;
	system("pause");

    return 0;
}

size_t Emulate(std::vector<int> Instructions, bool SpecialJumps)
{
	size_t JumpCount = 0;
	size_t Index = 0;

	while (Index < Instructions.size())
	{
		Index += (SpecialJumps && (Instructions[Index] >= 3)) ? Instructions[Index]-- : Instructions[Index]++;
		JumpCount++;
	}

	return JumpCount;
}

