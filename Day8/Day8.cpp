// Day8.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

static size_t GetUnescapedSize(const std::string & String);
static size_t GetEscapedSize(const std::string & String);

int main()
{
	std::ifstream Input("Input.txt");

	if (!Input.is_open())
	{
		std::cout << "Error opening File!" << std::endl;
		return 0;
	}

	std::string Line;

	int32_t DifferenceUnescaped = 0;
	int32_t DifferenceEscaped = 0;

	while (std::getline(Input, Line))
	{
		DifferenceUnescaped += (int32_t)(Line.size() - GetUnescapedSize(Line));
		DifferenceEscaped += (int32_t)(GetEscapedSize(Line) - Line.size());
	}

	Input.close();

	std::cout << "Difference Unescaped: " << DifferenceUnescaped << std::endl;
	std::cout << "Difference Escaped: " << DifferenceEscaped << std::endl;

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

size_t GetEscapedSize(const std::string & String)
{
	size_t Size = 0;
	for (char Character : String)
	{
		switch (Character)
		{
		case '\\':
		case '\"':
			Size++;
		default:
			Size++;
			break;
		}

	}

	return Size + 2;
}