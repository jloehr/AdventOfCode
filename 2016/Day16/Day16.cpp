// Day16.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

constexpr size_t FirstDiskSize = 272; 
constexpr size_t SecondDiskSize = 35651584;

std::string GenerateRandomData(std::string Data, size_t RequiredLength);
std::string GetCheckSum(std::string Data);

int main()
{
	std::string Input = "10111011111001111";

	std::cout << "Part One: " << GetCheckSum(GenerateRandomData(Input, FirstDiskSize)) << std::endl;
	std::cout << "Part Two: " << GetCheckSum(GenerateRandomData(Input, SecondDiskSize)) << std::endl;

	system("pause");

	// x = 0 - Input.size								- NumberOfOnes
	// x = 1 - 2 * Input.size + 1						- NumberOfOnes - (Length(x - 1) - NumberOfOnes)											| 2 * NumberOfOnes - Length(x-1)
	// x = 2 - 2 * (2 * Input.size + 1) + 1				- 2 * NumberOfOnes - Length(x-2) - (Length(x - 1) - (2 * NumberOfOnes - Length(x-2)))	| 4 * NumberOfOnes - 2 * Length(x-2) - Length(x - 1)
	// x = 3 - 2 * (2 * (2 * Input.size + 1) + 1) + 1	- 4 * NumberOfOnes - 2 * Length(x-3) - Length(x - 2) - (Length(x-1) - (4 * NumberOfOnes - 2 * Length(x-3) - Length(x - 2)))
	//         2 * (4 * Input.size() + 2 + 1 ) + 1		- 8 * NumberOfOnes - 4 * Length(x-3) - 2 * Length(x - 2) - Length(x-1) 
	//		   8 * Input.size() + 4 + 2 + 1				- 2^x * NumberOfOnes - Sum[y: 0 -> y < x](2^(y) * Length(x - y - 1))
	//		   8 * Input.size() + 7
	//		   

	// x = 3
	// 8 * NumberOfOnes - 4 * Input.size - 2 * (2 * Input.size + 1) - (2 * (2 * Input.size + 1) + 1)
	// 8 * NumberOfOnes - 4 * Input.size - 4 * Input.size - 2 - 4 * Input.size - 2 - 1
	// 8 * NumberOfOnes - 3 * 4 * Input.size - 9
	// 8 * NumberOfOnes - 12 * Input.size - 9
	// 2^x * NumberOfOnes - x * 2^(x-1) * Input.size - (2^x + 1)

	// x = 4
	// 16 * NumberOfOnes - 8 * Length(x - 4) - 4 * Length(x-3) - 2 * Length(x - 2) - Length(x-1) 
	// 16 * NumberOfOnes - 8 * Input.size - 4 * (2 * Input.size + 1) - 2 * (2 * (2 * Input.size + 1) + 1) - (2 * (2 * (2 * Input.size + 1) + 1) + 1)
	// 16 * NumberOfOnes - 8 * Input.size - 8 * Input.size - 4 - 8 * Input.size - 6 - 8 * Input.size - 7
	// 16 * NumberOfOnes - x * 8 * Input.size - 17
	// 2^x * NumberOfOnes - x * 2^(x-1) * Input.size - (2^x + 1)

    return 0;
}

std::string GenerateRandomData(std::string Data, size_t RequiredLength)
{
	while (Data.size() < RequiredLength)
	{
		Data.append("0");

		for (size_t i = Data.size() - 1; i > 0; i--)
		{
			Data += (Data[i - 1] == '0') ? '1' : '0';
		}
	}

	return Data.substr(0, RequiredLength);
}

std::string GetCheckSum(std::string Data)
{
	while ((Data.size() % 2) != 1)
	{
		std::string Buffer;

		for (std::string::const_iterator It = Data.begin(); It != Data.end(); It += 2)
		{
			Buffer += (*It == *(It + 1)) ? '1' : '0';
		}

		std::swap(Buffer, Data);
	}

	return Data;
}