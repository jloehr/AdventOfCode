// Day15.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

constexpr uint64_t g1 = 16807;
constexpr uint64_t g2 = 48271;

template<uint64_t g, uint64_t m = 1>
class Generator {
public:
	Generator(uint64_t Seed)
		:X(Seed) {}

	uint64_t Next()
	{
		while ((X = (X * g) % INT32_MAX) % m != 0);
		return X;
	}

private:
	uint64_t X;
};

int main()
{
	const StringVectorVector Lines = GetFileLineParts("Input.txt");
	const uint64_t Seed1 = static_cast<uint64_t>(std::stoul(Lines[0][4]));
	const uint64_t Seed2 = static_cast<uint64_t>(std::stoul(Lines[1][4]));

	Generator<g1> Generator1(Seed1);
	Generator<g2> Generator2(Seed2);
	Generator<g1, 4> Generator3(Seed1);
	Generator<g2, 8> Generator4(Seed2);
	size_t MatchesPartOne = 0;
	size_t MatchesPartTwo = 0;

	for (auto i = 0; i < 40000000; ++i)
	{
		MatchesPartOne += (static_cast<uint16_t>(Generator1.Next()) == static_cast<uint16_t>(Generator2.Next())) ? 1 : 0;
	}

	for (auto i = 0; i < 5000000; ++i)
	{
		MatchesPartTwo += (static_cast<uint16_t>(Generator3.Next()) == static_cast<uint16_t>(Generator4.Next())) ? 1 : 0;
	}

	std::cout << "Matches part one: " << MatchesPartOne << std::endl;
	std::cout << "Matches part two: " << MatchesPartTwo << std::endl;

	system("pause");
    return 0;
}

