// Day02.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <regex>

#include "../../Common/Common.h"

int main()
{
	std::regex LineMatching("(\\d+)-(\\d+)\\s(\\S):\\s(\\S+)", std::regex_constants::optimize);

	StringVector Lines = GetFileLines("Input.txt");
	size_t PartOne = 0;
	size_t PartTwo = 0;

	for(const std::string & Line : Lines)
	{
		std::smatch Matches;
		if (!std::regex_match(Line, Matches, LineMatching))
		{
			std::cout << "Line \"" << Line << "\" did not match the regex!" << std::endl;
			return 1;
		}

		size_t Lower = std::stoull(Matches[1]);
		size_t Higher = std::stoull(Matches[2]);

		size_t CharacterCount = std::count(Matches[4].first, Matches[4].second, Matches[3]);
		if ((CharacterCount >= Lower) && (CharacterCount <= Higher))
			PartOne++;

		if ((*(Matches[4].first + (Lower - 1)) == Matches[3]) != (*(Matches[4].first + (Higher - 1)) == Matches[3]))
			PartTwo++;
	}

	std::cout << "Part One: " << PartOne << std::endl;
	std::cout << "Part Two: " << PartTwo << std::endl;

}