// Day01.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int main()
{
	StringVector File = GetFileLines("Input.txt");
	const std::string & Line = File[0];
	uint64_t Sum = 0;

	char LastDigit = *Line.crbegin();
	for (auto DigitIt = Line.begin(); DigitIt != Line.end(); ++DigitIt)
	{
		const char Digit = *DigitIt;
		if (Digit == LastDigit)
		{
			Sum += Digit - '0';
		}

		LastDigit = Digit;
	}

	std::cout << "Captcha: " << Sum << std::endl;

	system("pause");

    return 0;
}

