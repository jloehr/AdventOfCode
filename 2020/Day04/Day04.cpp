// Day04.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <regex>

#include "../../Common/Common.h"


int main()
{
	std::regex FieldMatching("([a-z]{3}):(\\S+)", std::regex_constants::optimize);
	StringVector Lines = GetFileLines("Input.txt");
	Lines.push_back(""); // Add empty line, so processing will finish last passport

	size_t ValidPassports = 0;
	size_t FieldCount = 0;

	for (const std::string& Line : Lines)
	{
		if (Line.size() > 0)
		{
			for (std::sregex_iterator Match = std::sregex_iterator(Line.cbegin(), Line.cend(), FieldMatching); Match != std::sregex_iterator(); Match++)
			{
				if ((*Match)[1] != "cid")
					FieldCount++;
			}
		}
		else
		{
			if (FieldCount == 7)
				ValidPassports++;
			FieldCount = 0;
		}
	}

	std::cout << "Part One:" << ValidPassports << std::endl;
}
