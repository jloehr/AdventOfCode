// Day06.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

typedef std::map<char, unsigned> CharUIntMap;
typedef std::vector<CharUIntMap> CharUIntMapVector;

CharUIntMapVector GetLetterFrequency(const StringVector & Lines);
std::string GetErrorCorrectedMessage(const CharUIntMapVector & LetterFrequency);

int main()
{
	StringVector Lines = GetFileLines("Input.txt");

	CharUIntMapVector LetterFrequency = GetLetterFrequency(Lines);

	std::string Message = GetErrorCorrectedMessage(LetterFrequency);

	std::cout << "Message: " << Message << std::endl;

	system("pause");
}

CharUIntMapVector GetLetterFrequency(const StringVector & Lines)
{
	CharUIntMapVector LetterFrequency(Lines[0].size());

	for (const std::string & Line : Lines)
	{
		CharUIntMapVector::iterator LetterFrequencyIt = LetterFrequency.begin();

		for (const char & Letter : Line)
		{
			(*LetterFrequencyIt++)[Letter]++;
		}
	}

	return LetterFrequency;
}


std::string GetErrorCorrectedMessage(const CharUIntMapVector & LetterFrequency)
{
	std::string Message;

	for (const CharUIntMap & LetterCountMap : LetterFrequency)
	{
		char Letter = '*';
		signed Count = 0;

		for (auto & LetterCount : LetterCountMap)
		{
			if (LetterCount.second > Count)
			{
				Letter = LetterCount.first;
				Count = LetterCount.second;
			}
		}

		Message += Letter;
	}

	return Message;
}

