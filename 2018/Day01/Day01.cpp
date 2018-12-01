// Day01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

int main()
{
	int64_t Frequency = 0;
	StringVector File = GetFileLines("Input.txt");

	std::for_each(File.begin(), File.end(), [&](const std::string & Line) { Frequency += std::stoll(Line); });
	std::cout << "Part One: " << Frequency << std::endl;
}
