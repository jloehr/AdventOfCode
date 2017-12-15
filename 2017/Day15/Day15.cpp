// Day15.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

constexpr uint64_t g1 = 16807;
constexpr uint64_t g2 = 48271;

int main()
{
	const StringVectorVector Lines = GetFileLineParts("Input.txt");

	uint64_t Generator1 = static_cast<uint64_t>(std::stoul(Lines[0][4]));
	uint64_t Generator2 = static_cast<uint64_t>(std::stoul(Lines[1][4]));
	size_t Matches = 0;

	for (auto i = 0; i < 40000000; ++i)
	{
		Generator1 = (g1 * Generator1) % INT32_MAX;
		Generator2 = (g2 * Generator2) % INT32_MAX;
		Matches += (static_cast<uint16_t>(Generator1) == static_cast<uint16_t>(Generator2)) ? 1 : 0;
	}

	std::cout << "Matches: " << Matches << std::endl;

	system("pause");
    return 0;
}

