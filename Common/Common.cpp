#include "stdafx.h"

#include "Common.h"

StringVector GetFileLines(const std::string & InputFileName)
{
	std::ifstream Input(InputFileName);

	if (!Input.is_open())
	{
		std::cout << "Error opening Input File!" << std::endl;
		return StringVector();
	}

	StringVector InputLines;
	std::string Line;

	while (std::getline(Input, Line))
	{
		InputLines.push_back(Line);
	}

	Input.close();

	return InputLines;
}

StringVectorVector GetFileLineParts(const std::string & InputFileName)
{
	return GetLineValues<std::string>(InputFileName);
}
