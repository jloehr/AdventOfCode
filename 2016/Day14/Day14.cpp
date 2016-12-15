// Day14.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

struct KeyFindingData
{
	std::map<uint8_t, std::set<size_t>> PossibleKeys;
	size_t ValidKeysCount = 0;
	size_t KeyIndex = 0;
};

void ConsumeHash(const MD5::Hash & Hash, const ByteVector & HashInput, KeyFindingData & KeyData);
MD5::Hash Get2016thHash(std::string Input);
void AnalyzeHash(const MD5::Hash & Hash, uint8_t & Triplet, std::set<uint8_t> & Fivelets);
void IncreaseASCIIInteger(ByteVector & String);

static const std::string Input = "cuanljph";

int main()
{
	ByteVector HashInput(Input.begin(), Input.end());
	MD5 Hashing;

	KeyFindingData PartOne;
	KeyFindingData PartTwo;

	HashInput.push_back('0');

	while ((PartOne.KeyIndex == 0) || (PartTwo.KeyIndex == 0))
	{
		const MD5::Hash & Result = Hashing.Compute(HashInput);

		if (PartOne.KeyIndex == 0)
		{
			ConsumeHash(Result, HashInput, PartOne);
		}

		MD5::Hash StrechedHash = Get2016thHash(Hashing.GetHexString());

		if (PartTwo.KeyIndex == 0)
		{
			ConsumeHash(StrechedHash, HashInput, PartTwo);
		}
		
		IncreaseASCIIInteger(HashInput);
	}

	std::cout << "Part One - 64th Key Index: " << PartOne.KeyIndex << std::endl;
	std::cout << "Part Two - 64th Key Index: " << PartTwo.KeyIndex << std::endl;

	system("pause");

	return 0;
}

void ConsumeHash(const MD5::Hash & Hash, const ByteVector & HashInput, KeyFindingData & KeyData)
{
	std::set<uint8_t> Fivelets;
	uint8_t Nibble = 0xF0;

	AnalyzeHash(Hash, Nibble, Fivelets);

	if (!Fivelets.empty())
	{
		std::string IndexString(HashInput.begin() + Input.size(), HashInput.end());
		size_t Index = std::stoi(IndexString);

		for (uint8_t Byte : Fivelets)
		{
			std::set<size_t> & KeyIndices = KeyData.PossibleKeys[Byte];
			auto FirstValid = KeyIndices.upper_bound((Index < 1000) ? 0 : Index - 1000);

			while (FirstValid != KeyIndices.end())
			{
				KeyData.ValidKeysCount++;
				if (KeyData.ValidKeysCount == 64)
				{
					KeyData.KeyIndex = *FirstValid;
					return;
				}
				FirstValid++;
			}

			KeyIndices.clear();
		}
	}

	if (Nibble != 0xF0)
	{
		std::string IndexString(HashInput.begin() + Input.size(), HashInput.end());
		size_t Index = std::stoi(IndexString);
		KeyData.PossibleKeys[Nibble].emplace(Index);
	}
}

MD5::Hash Get2016thHash(std::string Input)
{
	MD5 Hasher;

	ByteVector InputData(Input.begin(), Input.end());

	for (size_t i = 0; i < 2016; i++)
	{
		Hasher.Compute(InputData);
		const std::string & Buffer = Hasher.GetHexString();
		InputData.assign(Buffer.begin(), Buffer.end());
	}

	return Hasher.GetHash();
}

void AnalyzeHash(const MD5::Hash & Hash, uint8_t & Triplet, std::set<uint8_t> & Fivelets)
{
	Triplet = 0xF0;
	uint8_t LastNibble = 0xF0;
	size_t Count = 0;

	for (uint8_t Byte : Hash)
	{
		uint8_t HighNible = Byte >> 4;
		Byte &= 0x0F;

		Count = (LastNibble == HighNible) ? Count + 1 : 1;
		if ((Count == 3) && (Triplet == 0xF0))
		{
			Triplet = HighNible;
		} 
		else if (Count == 5)
		{
			Fivelets.insert(HighNible);
		}

		Count = (HighNible == Byte) ? Count + 1 : 1;
		if ((Count == 3) && (Triplet == 0xF0))
		{
			Triplet = Byte;
		} 
		else if (Count == 5)
		{
			Fivelets.insert(Byte);
		}

		LastNibble = Byte;
	}
}

void IncreaseASCIIInteger(ByteVector & String, ByteVector::reverse_iterator It)
{
	if (It == String.rend() || ((*It) < '0') || ((*It) > '9'))
	{
		String.insert(It.base(), '1');
	}
	else if ((*It) == '9')
	{
		(*It) = '0';
		IncreaseASCIIInteger(String, It + 1);
	}
	else
	{
		(*It)++;
	}
}

void IncreaseASCIIInteger(ByteVector & String)
{
	IncreaseASCIIInteger(String, String.rbegin());
}
