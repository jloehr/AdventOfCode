// Day01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

int main()
{
	std::vector<uint64_t> File = GetValues<uint64_t>("Input.txt");

	uint64_t Fuel = std::accumulate(File.cbegin(), File.cend(), static_cast<uint64_t>(0u), [](uint64_t sum, uint64_t value) -> uint64_t { return sum + ((value / 3) - 2); });

	std::cout << "Required fuel: " << Fuel << std::endl;
}
