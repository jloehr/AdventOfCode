// Day20.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

typedef std::pair<uint32_t, uint32_t> Range;
typedef std::set<Range> RangeSet;

RangeSet ParseInput(std::string File);
RangeSet MergeRanges(RangeSet Set);

int main()
{
	RangeSet Input = ParseInput("Input.txt");

	RangeSet MergedRanges = MergeRanges(Input);

	std::cout << "First valid: " << (MergedRanges.begin()->second + 1) << std::endl;

	system("pause");

    return 0;
}

RangeSet ParseInput(std::string File)
{
	RangeSet Set;
	StringVector Lines = GetFileLines(File);

	for (const std::string & Line : Lines)
	{
		size_t DelimiterPosition;
		uint32_t RangeStart = std::stoul(Line, &DelimiterPosition);
		uint32_t RangeEnd = std::stoul(Line.substr(DelimiterPosition + 1));

		Set.emplace(RangeStart, RangeEnd);
	}

	return Set;
}

RangeSet MergeRanges(RangeSet Set)
{
	RangeSet MergedSet;
	Range CurrentRange = *Set.begin();

	for (const Range & Range : Set)
	{
		// If Range is outside of old range
		if (Range.first > (CurrentRange.second + 1))
		{
			MergedSet.insert(CurrentRange);
			CurrentRange = Range;
			continue;
		}

		CurrentRange.second = std::max(CurrentRange.second, Range.second);
	}

	return MergedSet;
}