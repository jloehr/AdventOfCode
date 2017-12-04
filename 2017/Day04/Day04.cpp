// Day04.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int main()
{
	const StringVectorVector FileParts = GetFileLineParts("Input.txt");
	uint64_t ValidPassphrasesPartOne = 0;
	uint64_t ValidPassphrasesPartTwo = 0;

	for (const auto & Line : FileParts)
	{
		std::unordered_set<std::string> Words;
		std::set<std::map<char, size_t>> WordCharacters;
		bool ValidPartOne = true;
		bool ValidPartTwo = true;

		for (const auto & Word : Line)
		{
			// Part One
			{
				auto Result = Words.insert(Word);
				if (!Result.second)
				{
					ValidPartOne = false;
				}
			}

			// Part two
			{
				std::map<char, size_t> Characters;
				for (const char & Character : Word)
				{
					Characters[Character]++;
				}

				auto Result = WordCharacters.insert(Characters);
				if (!Result.second)
				{
					ValidPartTwo = false;
				}
			}

			if (!ValidPartOne && !ValidPartTwo)
				break;
		}

		if (ValidPartOne)
			ValidPassphrasesPartOne++;
		if (ValidPartTwo)
			ValidPassphrasesPartTwo++;
	}

	std::cout << "Valid passwords part one: " << ValidPassphrasesPartOne << std::endl;
	std::cout << "Valid passwords part two: " << ValidPassphrasesPartTwo << std::endl;

	system("pause");
    return 0;
}

