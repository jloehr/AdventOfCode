// Day16.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

typedef std::function<bool(size_t, size_t)> CompareFunction;

typedef std::map<std::string, size_t> StringIntMap;
typedef std::map<std::string, CompareFunction> CompareFunctionMap;

static StringIntMap ParseTickerData();

static CompareFunction LessThan = [](size_t A, size_t B)->bool { return (A < B); };
static CompareFunction Equal = [](size_t A, size_t B)->bool { return (A == B); };
static CompareFunction GreaterThan = [](size_t A, size_t B)->bool { return (A > B); };

int main()
{
	StringIntMap TickerData = ParseTickerData();

	StringVectorVector Input = GetFileLineParts("Input.txt");
	CompareFunctionMap SpecialTypeCompareFunctions = { { "cats", GreaterThan }, { "trees", GreaterThan }, { "pomeranians", LessThan }, { "goldfish", LessThan } };

	for (const StringVector & Line : Input)
	{
		std::string SueNo = Line[1].substr(0, Line[1].size() - 1);

		bool MissmatchFoundForPartOne = false;
		bool MissmatchFoundForPartTwo = false;

		for (StringVector::const_iterator PartIter = (Line.begin() + 2); PartIter != Line.end(); PartIter += 2)
		{
			std::string Type = PartIter->substr(0, PartIter->size() - 1);
			std::string AmountString = ((*((PartIter + 1)->rbegin())) != ',') ? *(PartIter + 1) : (PartIter + 1)->substr(0, (PartIter + 1)->size() - 1);
			size_t Amount = std::atoi(AmountString.c_str());

			auto Data = TickerData.find(Type);
			if (Data != TickerData.end())
			{
				// Part One
				if (Data->second != Amount)
				{
					MissmatchFoundForPartOne = true;
				}

				// Part Two
				auto SpecialCompareFunctionResult = SpecialTypeCompareFunctions.find(Type);
				auto SpecialCompareFunction = (SpecialCompareFunctionResult == SpecialTypeCompareFunctions.end()) ? Equal : SpecialCompareFunctionResult->second;
				if (!SpecialCompareFunction(Amount, Data->second))
				{
					MissmatchFoundForPartTwo = true;
				}
			}
		}

		if (!MissmatchFoundForPartOne)
		{
			std::cout << "Sue no. (Part One): " << SueNo << std::endl;
		}


		if (!MissmatchFoundForPartTwo)
		{
			std::cout << "Sue no. (Part Two): " << SueNo << std::endl;
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