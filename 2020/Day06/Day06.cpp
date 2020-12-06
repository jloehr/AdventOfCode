// Day06.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <unordered_set>

#include "../../Common/Common.h"

int main()
{
	StringVector Lines = GetFileLines("Input.txt");
	Lines.push_back("");

	std::unordered_set<char> Group;
	size_t Sum = 0;

	for (const std::string& Line : Lines)
	{
		if (Line.size() > 0)
		{
			Group.insert(Line.cbegin(), Line.cend());
		}
		else
		{
			Sum += Group.size();
			Group.clear();
		}
	}

	std::cout << "Part One: " << Sum << std::endl;
}