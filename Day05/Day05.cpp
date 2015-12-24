// Day5.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

static bool IsStringNicePartOne(const std::string & String);
static bool IsStringNicePartTwo(const std::string & String);

int main()
{
	std::ifstream Input("Input.txt");

	if (!Input.is_open())
	{
		std::cout << "Error opening File!" << std::endl;
		return 0;
	}

	std::string Line;
	uint32_t NumberOfNiceStringsPartOne = 0;
	uint32_t NumberOfNiceStringsPartTwo = 0;


	while (std::getline(Input, Line))
	{
		if (IsStringNicePartOne(Line))
		{
			NumberOfNiceStringsPartOne++;
		}

		if (IsStringNicePartTwo(Line))
		{
			NumberOfNiceStringsPartTwo++;
		}
	}

	Input.close();

	std::cout << "Nice Strings (Part One): " << NumberOfNiceStringsPartOne << std::endl;
	std::cout << "Nice Strings (Part Two): " << NumberOfNiceStringsPartTwo << std::endl;

	system("pause");
	
	return 0;
}

static bool IsStringNicePartOne(const std::string & String)
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

static bool IsStringNicePartTwo(const std::string & String)
{
	bool PairRepeated = false;
	bool LetterRepeated = false;

	std::map<char, std::set<char>> FollowingLetters;
	
	for (std::string::const_iterator Letter = String.begin(); (Letter != String.end()) && (!(PairRepeated && LetterRepeated)); Letter++)
	{
		// Check for the Pair
		if (!PairRepeated)
		{
			// Check if Pair of current and next Letter is present
			if (Letter <= (String.end() - 2))
			{
				auto CurrentLetterResult = FollowingLetters.find(*Letter);
				if (CurrentLetterResult != FollowingLetters.end())
				{
					char NextLetter = *(Letter + 1);
					auto PairResult = CurrentLetterResult->second.find(NextLetter);
					if (PairResult != CurrentLetterResult->second.end())
					{
						PairRepeated = true;
					}
				}
			}

			// Add Pair of previous and current Letter
			if (Letter >= (String.begin() + 1))
			{
				char PreviousLetter = *(Letter - 1);
				auto PreviousLetterResult = FollowingLetters.find(PreviousLetter);

				if (PreviousLetterResult != FollowingLetters.end())
				{
					PreviousLetterResult->second.insert(*Letter);
				}
				else
				{
					FollowingLetters.insert({ PreviousLetter,{ *Letter } });
				}
			}
		}
		
		// Check for the letter repeating with on in between
		if ((!LetterRepeated) && (Letter >= (String.begin() + 2)))
		{
			if ((*Letter) == (*(Letter - 2)))
			{
				LetterRepeated = true;
			}
		}

	}

	return (PairRepeated && LetterRepeated);
}