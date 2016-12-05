// Day04.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

typedef std::map<char, unsigned> CharacterCountMap;
typedef std::pair<char, unsigned> CharUIntPair;
typedef std::vector<CharUIntPair> CharUIntVector;
typedef std::pair<std::string, unsigned> StringUIntPair;
typedef std::vector<StringUIntPair> StringUIntVector;

StringUIntVector GetValidRooms(const StringVector & Lines);
CharacterCountMap CountCharacters(const std::string & String, size_t Length);
bool ValidateCheckSum(const CharacterCountMap & CharacterCount, const std::string & CheckSum);

unsigned DecryptNamesAndAccumulateSetorIDs(StringUIntVector & ValidRooms);

int main()
{
	const std::string RoomName("northpole object storage");

	StringVector Lines = GetFileLines("Input.txt");
	StringUIntVector ValidRooms = GetValidRooms(Lines);
	unsigned SectorIDSum = DecryptNamesAndAccumulateSetorIDs(ValidRooms);

	std::cout << "Sector ID Sum: " << SectorIDSum << std::endl;

	for (auto Pair : ValidRooms)
	{
		if (Pair.first == RoomName)
		{
			std::cout << Pair.first << " - " << Pair.second << std::endl;
		}
	}

	system("pause");
}

StringUIntVector GetValidRooms(const StringVector & Lines)
{
	StringUIntVector ValidRooms;

	for (const std::string & Line : Lines)
	{
		const size_t CheckSumStart = Line.rfind('[') + 1;
		const size_t CheckSumEnd = Line.rfind(']');
		const std::string CheckSum = Line.substr(CheckSumStart, CheckSumEnd - CheckSumStart);

		const size_t SectorIDStart = Line.rfind('-', CheckSumStart) + 1;
		const unsigned SectorID = std::atoi(Line.c_str() + SectorIDStart);

		const CharacterCountMap CharacterCount = CountCharacters(Line, SectorIDStart - 1);

		if (ValidateCheckSum(CharacterCount, CheckSum))
		{
			ValidRooms.emplace_back(Line.substr(0, SectorIDStart - 1), SectorID);
		}
	}

	return ValidRooms;
}

CharacterCountMap CountCharacters(const std::string & String, size_t Length)
{
	CharacterCountMap CharacterCount;

	for (std::string::const_iterator Character = String.begin(); Character != String.begin() + Length; Character++)
	{
		if ((*Character) == '-')
		{
			continue;
		}

		CharacterCount[(*Character)]++;
	}

	return CharacterCount;
}

bool ValidateCheckSum(const CharacterCountMap & CharacterCount, const std::string & CheckSum)
{
	CharUIntVector SortedCount(CharacterCount.begin(), CharacterCount.end());
	std::sort(SortedCount.begin(), SortedCount.end(), [](CharUIntPair A, CharUIntPair B) {
		if (B.second == A.second)
		{
			return A.first < B.first;
		}

		return B.second < A.second;
	});
	
	CharUIntVector::iterator CountIt = SortedCount.begin();

	for (std::string::const_iterator CheckSumIt = CheckSum.begin(); CheckSumIt != CheckSum.end(); CheckSumIt++, CountIt++)
	{
		if (CountIt == SortedCount.end())
		{
			return false;
		}

		if (CountIt->first != (*CheckSumIt))
		{
			return false;
		}
	}

	return true;
}

unsigned DecryptNamesAndAccumulateSetorIDs(StringUIntVector & ValidRooms)
{
	unsigned SectorIDSum = 0;

	for (auto & Room : ValidRooms)
	{
		SectorIDSum += Room.second;

		for (char & Character : Room.first)
		{
			if (Character == '-')
			{
				Character = ' ';
			}
			else
			{
				Character = (((Character - 'a') + Room.second) % 26) + 'a';
			}
		}
	}

	return SectorIDSum;
}
