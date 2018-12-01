// Day01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

int main()
{
	int64_t Frequency = 0;
	std::set<int64_t> Frequencies;
	std::vector<int64_t> File = GetValues<int64_t>("Input.txt");
	std::vector<int64_t>::iterator Line = File.begin();

	std::cout << "Part One: " << std::accumulate<std::vector<int64_t>::iterator, int64_t>(File.begin(), File.end(), 0) << std::endl;

	while (Frequencies.insert(Frequency).second)
	{
		Frequency += *Line;

		if (++Line == File.end())
			Line = File.begin();
	}

	std::cout << "Part Two: " << Frequency << std::endl;
}
