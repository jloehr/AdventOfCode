// Day14.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

void AnalyzeHash(const MD5::Hash & Hash, uint8_t & Triplet, std::set<uint8_t> & Fivelets);
void IncreaseASCIIInteger(ByteVector & String);

int main()
{
	const std::string Input = "cuanljph";
	ByteVector HashInput(Input.begin(), Input.end());
	std::map<uint8_t, std::set<size_t>> PossibleKeys;
	MD5 Hashing;
	size_t ValidKeys = 0;
	size_t KeyIndex = 0;

	HashInput.push_back('0');

	while (KeyIndex == 0)
	{
		const MD5::Hash & Result = Hashing.Compute(HashInput);
		std::set<uint8_t> Fivelets;
		uint8_t Nibble = 0xF0;

		AnalyzeHash(Result, Nibble, Fivelets);

		if (!Fivelets.empty())
		{
			std::string IndexString(HashInput.begin() + Input.size(), HashInput.end());
			size_t Index = std::stoi(IndexString);

			for (uint8_t Byte : Fivelets)
			{
				std::set<size_t> & KeyIndices = PossibleKeys[Byte];
				auto FirstValid = KeyIndices.upper_bound((Index < 1000) ? 0 : Index - 1000);
				
				while (FirstValid != KeyIndices.end())
				{
					ValidKeys++;
					if (ValidKeys == 64)
					{
						KeyIndex = *FirstValid;
						break;
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
			PossibleKeys[Nibble].emplace(Index);
		}

		IncreaseASCIIInteger(HashInput);
	}

	std::cout << "64th Key Index: " << KeyIndex << std::endl;

	system("pause");

	return 0;
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
