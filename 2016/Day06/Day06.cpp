// Day06.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

typedef std::pair<char, unsigned> CharUIntPair;
typedef std::map<char, unsigned> CharUIntMap;
typedef std::vector<CharUIntMap> CharUIntMapVector;

CharUIntMapVector GetLetterFrequency(const StringVector & Lines);
void GetErrorCorrectedMessage(const CharUIntMapVector & LetterFrequency, std::string & MostFrequentMessage, std::string & LeastFrequentMessage);

int main()
{
	StringVector Lines = GetFileLines("Input.txt");

	CharUIntMapVector LetterFrequency = GetLetterFrequency(Lines);

	std::string MostFrequentMessage;
	std::string LeastFrequentMessage;
	
	GetErrorCorrectedMessage(LetterFrequency, MostFrequentMessage, LeastFrequentMessage);

	std::cout << "Message Part One: " << MostFrequentMessage << std::endl;
	std::cout << "Message Part Two: " << LeastFrequentMessage << std::endl;

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


void GetErrorCorrectedMessage(const CharUIntMapVector & LetterFrequency, std::string & MostFrequentMessage, std::string & LeastFrequentMessage)
{
	MostFrequentMessage.clear();
	LeastFrequentMessage.clear();

	for (const CharUIntMap & LetterCountMap : LetterFrequency)
	{
		CharUIntPair MostFrequentLetter('*', 0);
		CharUIntPair LeastFrequentLetter('*', UINT_MAX);

		for (auto & LetterCount : LetterCountMap)
		{
			if (LetterCount.second > MostFrequentLetter.second)
			{
				MostFrequentLetter.first = LetterCount.first;
				MostFrequentLetter.second = LetterCount.second;
			}

			if (LetterCount.second < LeastFrequentLetter.second)
			{
				LeastFrequentLetter.first = LetterCount.first;
				LeastFrequentLetter.second = LetterCount.second;
			}
		}

		MostFrequentMessage += MostFrequentLetter.first;
		LeastFrequentMessage += LeastFrequentLetter.first;
	}
}

