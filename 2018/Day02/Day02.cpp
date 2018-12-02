// Day02.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

unsigned int PartOne(const StringVector & Lines);
std::string PartTwo(const StringVector & Lines);

int main()
{
	StringVector File = GetFileLines("Input.txt");
	
	std::cout << "Part One: " << PartOne(File) << std::endl;
	std::cout << "Part Two: " << PartTwo(File) << std::endl;
}

unsigned int PartOne(const StringVector & Lines)
{
	unsigned int Doubles = 0;
	unsigned int Triples = 0;

	for (const std::string & Line : Lines)
	{
		std::unordered_map<std::string::value_type, unsigned int> CharacterCount;
		for (const std::string::value_type & Character : Line)
			++CharacterCount[Character];

		bool HasDouble = false;
		bool HasTripple = false;

		for (std::pair<std::string::value_type, unsigned int> Count : CharacterCount)
		{
			if (Count.second == 2)
				HasDouble = true;

			if (Count.second == 3)
				HasTripple = true;

			if (HasDouble && HasTripple)
				break;
		}

		if (HasDouble)
			++Doubles;
		if (HasTripple)
			++Triples;
	}

	return (Doubles * Triples);
}

std::string PartTwo(const StringVector & Lines)
{
	for (auto Line = Lines.begin(); Line != Lines.end(); ++Line)
	{
		for (auto CompareLine = (Line + 1); CompareLine != Lines.end(); ++CompareLine)
		{
			auto FirstMismatch = std::mismatch(Line->begin(), Line->end(), CompareLine->begin());

			// Both are equal, should never occur or by the puzzle definition not a solution.
			if (FirstMismatch.first == Line->end() || FirstMismatch.second == CompareLine->end())
				continue;

			auto SecondMismatchCheck = std::mismatch(FirstMismatch.first + 1, Line->end(), FirstMismatch.second + 1);

			// After the first Mismatch, rest of both is the same, therefore those are the IDs we are looking for
			if (SecondMismatchCheck.first == Line->end() && SecondMismatchCheck.second == CompareLine->end())
			{
				return std::string(*Line).erase(std::distance(Line->begin(), FirstMismatch.first), 1);
			}
		}
	}

	return "No Match found!";
}
