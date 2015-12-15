// Day5.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

static bool IsStringNice(const std::string & String);

int main()
{
	std::ifstream Input("Input.txt");

	if (!Input.is_open())
	{
		std::cout << "Error opening File!" << std::endl;
		return 0;
	}

	std::string Line;
	uint32_t NumberOfNiceStrings = 0;


	while (std::getline(Input, Line))
	{
		if (IsStringNice(Line))
		{
			NumberOfNiceStrings++;
		}
	}

	Input.close();

	std::cout << "Nice Strings: " << NumberOfNiceStrings << std::endl;

	system("pause");
	
	return 0;
}

static bool IsStringNice(const std::string & String)
{
	static const std::set<char> Vowels = { 'a', 'e','i', 'o', 'u' };
	static const std::map<char, std::string> BadStrings = { { 'a', "ab" }, { 'c', "cd" }, { 'p', "pq" }, { 'x', "xy" } };

	uint32_t NumberOfVowels = 0;
	bool DoubleLetter = false;

	auto PossibleBadString = BadStrings.end();

	for (std::string::const_iterator Letter = String.begin(); Letter != String.end(); Letter++)
	{
		// Check for bad string
		if (PossibleBadString != BadStrings.end())
		{
			if (PossibleBadString->second[1] == (*Letter))
			{
				return false;
			}
		}

		PossibleBadString = BadStrings.find(*Letter);

		// Check for Vowel
		if ((NumberOfVowels < 3) && (Vowels.find(*Letter) != Vowels.end()))
		{
			NumberOfVowels++;
		}

		// Check for Double Letter
		if ((!DoubleLetter) && (Letter != String.begin()))
		{
			if ((*Letter) == (*(Letter - 1)))
			{
				DoubleLetter++;
			}
		}
	}

	return ((NumberOfVowels >= 3) && DoubleLetter);
}