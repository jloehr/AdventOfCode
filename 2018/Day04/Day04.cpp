// Day04.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

typedef std::array<unsigned, 60> MinuteArray;

struct Guard
{
	unsigned Id;
	unsigned TotalMinutesAsleep;
	MinuteArray TimeAsleepCounter;

	Guard(unsigned Id)
		:Id(Id), TotalMinutesAsleep(0), TimeAsleepCounter({ 0 }) {}

	Guard() = default;
};

typedef std::unordered_map<unsigned, Guard> GuardMap;

const GuardMap AnalyzeGuards(const StringVector & Lines);

int main()
{
	StringVector File = GetFileLines("Input.txt");
	std::sort(File.begin(), File.end());

	const GuardMap Guards = AnalyzeGuards(File);

	GuardMap::const_iterator MostAsleepGuard = std::max_element(
		std::begin(Guards),
		std::end(Guards),
		[](const auto & a, const auto & b) { return a.second.TotalMinutesAsleep < b.second.TotalMinutesAsleep; }
	);

	MinuteArray::const_iterator MostAspleepMinute = std::max_element(
		std::begin(MostAsleepGuard->second.TimeAsleepCounter),
		std::end(MostAsleepGuard->second.TimeAsleepCounter)
	);

	std::cout << "Part One: " << MostAsleepGuard->second.Id * std::distance(std::begin(MostAsleepGuard->second.TimeAsleepCounter), MostAspleepMinute) << std::endl;
}

const GuardMap AnalyzeGuards(const StringVector & Lines)
{
	std::regex RegEx("\\[\\d{4}-\\d{2}-\\d{2}\\s\\d{2}:(\\d{2})\\]\\s(.)(?:uard\\s#(\\d+))?.+", std::regex_constants::optimize);
	std::smatch Matches;
	GuardMap Guards;
	GuardMap::iterator CurrentGuard;
	unsigned StartTime = 0;

	for (const std::string & Line : Lines)
	{
		if (!std::regex_match(Line, Matches, RegEx))
			std::cout << Line << " - did not match!" << std::endl;

		switch (*Matches[2].first)
		{
		case 'G':
			{
				unsigned Id = std::stoul(Matches[3]);
				CurrentGuard = Guards.insert({ Id, Guard(Id) }).first;
				break;
			}
		case 'f':
			StartTime = std::stoul(Matches[1]);
			break;
		case 'w':
			{
				unsigned EndTime = std::stoul(Matches[1]);
				CurrentGuard->second.TotalMinutesAsleep += EndTime - StartTime;

				std::for_each(
					std::begin(CurrentGuard->second.TimeAsleepCounter) + StartTime,
					std::begin(CurrentGuard->second.TimeAsleepCounter) + EndTime,
					[](unsigned & Counter) { ++Counter; }
				);
				break;
			}
		}
	}

	return Guards;
}
