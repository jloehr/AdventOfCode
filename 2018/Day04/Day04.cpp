// Day04.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

typedef std::array<size_t, 60> MinuteArray;

struct Guard
{
	size_t Id;
	size_t TotalMinutesAsleep;
	size_t MostAsleepMinute;
	MinuteArray TimeAsleepCounter;

	Guard(size_t Id)
		:Id(Id), TotalMinutesAsleep(0), TimeAsleepCounter({ 0 }) {}

	Guard() = default;
};

typedef std::unordered_map<size_t, Guard> GuardMap;

const GuardMap AnalyzeGuards(const StringVector & Lines);

int main()
{
	StringVector File = GetFileLines("Input.txt");
	std::sort(File.begin(), File.end());

	const GuardMap Guards = AnalyzeGuards(File);

	GuardMap::const_iterator MostAsleepGuardByTotal = std::max_element(
		std::begin(Guards),
		std::end(Guards),
		[](const auto & a, const auto & b) { return a.second.TotalMinutesAsleep < b.second.TotalMinutesAsleep; }
	);

	GuardMap::const_iterator MostAsleepGuardByMinute = std::max_element(
		std::begin(Guards),
		std::end(Guards),
		[](const auto & a, const auto & b) { return a.second.TimeAsleepCounter[a.second.MostAsleepMinute] < b.second.TimeAsleepCounter[b.second.MostAsleepMinute]; }
	);
	
	std::cout << "Part One: " << MostAsleepGuardByTotal->second.Id * MostAsleepGuardByTotal->second.MostAsleepMinute << std::endl;
	std::cout << "Part Two: " << MostAsleepGuardByMinute->second.Id * MostAsleepGuardByMinute->second.MostAsleepMinute << std::endl;
}

const GuardMap AnalyzeGuards(const StringVector & Lines)
{
	std::regex RegEx("\\[\\d{4}-\\d{2}-\\d{2}\\s\\d{2}:(\\d{2})\\]\\s(.)(?:uard\\s#(\\d+))?.+", std::regex_constants::optimize);
	std::smatch Matches;
	GuardMap Guards;
	GuardMap::iterator CurrentGuard;
	size_t StartTime = 0;

	for (const std::string & Line : Lines)
	{
		if (!std::regex_match(Line, Matches, RegEx))
			std::cout << Line << " - did not match!" << std::endl;

		switch (*Matches[2].first)
		{
		case 'G':
			{
				size_t Id = std::stoull(Matches[3]);
				CurrentGuard = Guards.insert({ Id, Guard(Id) }).first;
				break;
			}
		case 'f':
			StartTime = std::stoull(Matches[1]);
			break;
		case 'w':
			{
				size_t EndTime = std::stoull(Matches[1]);
				CurrentGuard->second.TotalMinutesAsleep += EndTime - StartTime;

				std::for_each(
					std::begin(CurrentGuard->second.TimeAsleepCounter) + StartTime,
					std::begin(CurrentGuard->second.TimeAsleepCounter) + EndTime,
					[](size_t & Counter) { ++Counter; }
				);

				break;
			}
		}
	}

	std::for_each(std::begin(Guards), std::end(Guards), [](auto & Entry)
		{
			MinuteArray::const_iterator MostAspleepMinute = std::max_element(
				std::begin(Entry.second.TimeAsleepCounter),
				std::end(Entry.second.TimeAsleepCounter)
			);

			Entry.second.MostAsleepMinute = std::distance(std::cbegin(Entry.second.TimeAsleepCounter), MostAspleepMinute);
		}
	);

	return Guards;
}
