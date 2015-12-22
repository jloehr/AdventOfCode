// Day8.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

static size_t GetUnescapedSize(const std::string & String);

int main()
{
	std::ifstream Input("Input.txt");

	if (!Input.is_open())
	{
		std::cout << "Error opening File!" << std::endl;
		return 0;
	}

	std::string Line;

	int32_t Difference = 0;

	while (std::getline(Input, Line))
	{
		Difference += (int32_t)(Line.size() - GetUnescapedSize(Line));
	}

	Input.close();

	std::cout << "Difference: " << Difference << std::endl;

	system("pause");

	return 0;
}

size_t GetUnescapedSize(const std::string & String)
{
	size_t Size = 0;
	for (std::string::const_iterator Character = (String.begin() + 1); Character != (String.end() - 1); Character++)
	{
		Size++;
		if (*Character == '\\')
		{
			Character++;

			switch (*Character)
			{
			case 'x':
				Character += 2;
				break;
			default:
				break;
			}
		}
	}

	return Size;
}
