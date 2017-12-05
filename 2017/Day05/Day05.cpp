// Day05.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int main()
{
	const StringVector Lines = GetFileLines("Input.txt");
	std::vector<int> Instructions(Lines.size());
	std::transform(Lines.begin(), Lines.end(), Instructions.begin(), [](const auto & String) -> int { return std::stoi(String); });

	size_t JumpCount = 0;
	size_t Index = 0;

	while (Index < Instructions.size())
	{
		Index += Instructions[Index]++;
		JumpCount++;
	}

	std::cout << "Jumps: " << JumpCount << std::endl;
	system("pause");

    return 0;
}

