// Day12.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

const static std::string NumericCharacters("-0123456789");


int main()
{
	std::ifstream Input("Input.txt");

	if (!Input.is_open())
	{
		std::cout << "Error opening File!" << std::endl;
		return 0;
	}

	std::string InputString((std::istreambuf_iterator<char>(Input)),
							(std::istreambuf_iterator<char>()));

	Input.close();

	int64_t Sum = 0;
	size_t CurrentPosition = 0;

	while (CurrentPosition != InputString.npos)
	{
		CurrentPosition = InputString.find_first_of(NumericCharacters, CurrentPosition);
		size_t NextStart = InputString.find_first_not_of(NumericCharacters, CurrentPosition);

		Sum += std::atoi(InputString.c_str() + CurrentPosition);

		CurrentPosition = NextStart;
	}

	std::cout << Sum << std::endl;

	system("pause");

    return 0;
}

