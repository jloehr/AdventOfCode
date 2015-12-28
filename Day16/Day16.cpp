// Day16.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

typedef std::map<std::string, size_t> StringIntMap;

static StringIntMap ParseTickerData();


int main()
{
	StringIntMap TickerData = ParseTickerData();

	StringVectorVector Input = GetFileLineParts("Input.txt");

	for (const StringVector & Line : Input)
	{
		std::string SueNo = Line[1].substr(0, Line[1].size() - 1);

		bool MissmatchFound = false;

		for (StringVector::const_iterator PartIter = (Line.begin() + 2); PartIter != Line.end(); PartIter += 2)
		{
			std::string Type = PartIter->substr(0, PartIter->size() - 1);
			std::string AmountString = ((*((PartIter + 1)->rbegin())) != ',') ? *(PartIter + 1) : (PartIter + 1)->substr(0, (PartIter + 1)->size() - 1);
			size_t Amount = std::atoi(AmountString.c_str());

			auto Data = TickerData.find(Type);
			if (Data != TickerData.end())
			{
				if (Data->second != Amount)
				{
					MissmatchFound = true;
					break;
				}
			}
		}

		if (!MissmatchFound)
		{
			std::cout << "Sue no. " << SueNo << std::endl;
		}
	}

	system("pause");

    return 0;
}


static StringIntMap ParseTickerData()
{
	StringVectorVector TickerLinesParts = GetFileLineParts("TickerInput.txt");
	StringIntMap TickerData;

	for (const StringVector & Line : TickerLinesParts)
	{
		TickerData[Line[0].substr(0, Line[0].size() - 1)] = std::atoi(Line[1].c_str());
	}

	return TickerData;
}