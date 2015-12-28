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
	StringVector FileLines = GetFileLines(InputFileName);

	StringVectorVector FileLineParts(FileLines.size());
	StringVectorVector::iterator FileLinePartsIterator = FileLineParts.begin();

	for (const std::string & Line : FileLines)
	{
		std::stringstream LineStream(Line);

		std::copy(std::istream_iterator<std::string>(LineStream),
			std::istream_iterator<std::string>(),
			std::back_inserter(*(FileLinePartsIterator++)));
	}

	return FileLineParts;
}