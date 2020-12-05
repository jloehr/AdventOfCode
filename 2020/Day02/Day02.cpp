// Day02.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <regex>

#include "../../Common/Common.h"

int main()
{
	std::regex LineMatching("(\\d+)-(\\d+)\\s(\\S):\\s(\\S+)", std::regex_constants::optimize);

	StringVector Lines = GetFileLines("Input.txt");
	size_t ValidPasswords = 0;

	for(const std::string & Line : Lines)
	{
		std::smatch Matches;
		if (!std::regex_match(Line, Matches, LineMatching))
		{
			std::cout << "Line \"" << Line << "\" did not match the regex!" << std::endl;
			return 1;
		}

		size_t CharacterCount = std::count(Matches[4].first, Matches[4].second, Matches[3]);
		size_t MinCharacterCount = std::stoull(Matches[1]);
		size_t MaxCharacterCount = std::stoull(Matches[2]);

		if ((CharacterCount >= MinCharacterCount) && (CharacterCount <= MaxCharacterCount))
			ValidPasswords++;
	}

	std::cout << "Part One: " << ValidPasswords << std::endl;

}