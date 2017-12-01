// Day01.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int main()
{
	StringVector File = GetFileLines("Input.txt");
	const std::string & Line = File[0];
	uint64_t SumPartOne = 0;
	uint64_t SumPartTwo = 0;

	char LastDigit = *Line.crbegin();
	auto OppositeDigitIt = Line.begin() + Line.size() / 2;
	for (auto DigitIt = Line.begin(); DigitIt != Line.end(); ++DigitIt)
	{
		const char Digit = *DigitIt;
		SumPartOne += (Digit == LastDigit) ? Digit - '0' : 0;
		SumPartTwo += (Digit == *OppositeDigitIt) ? Digit - '0' : 0;

		LastDigit = Digit;
		OppositeDigitIt = (++OppositeDigitIt == Line.end()) ? Line.begin() : OppositeDigitIt;
	}

	std::cout << "Captcha part one: " << SumPartOne << std::endl;
	std::cout << "Captcha part two: " << SumPartTwo << std::endl;

	system("pause");

    return 0;
}

