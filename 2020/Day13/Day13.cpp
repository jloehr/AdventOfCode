// Day13.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <regex>
#include <algorithm>

#include "../../Common/Common.h"

int main()
{
	const std::regex BusMatching("(\\d+)");
	StringVector Lines = GetFileLines("Input.txt");

	size_t Timestamp = std::stoull(Lines[0]);
	std::vector<std::pair<size_t, size_t>> Buses;

	std::transform(
		std::sregex_iterator(std::cbegin(Lines[1]), std::cend(Lines[1]), BusMatching), 
		std::sregex_iterator(), 
		std::back_inserter(Buses), 
		[=](const std::smatch& Match)->std::pair<size_t, size_t> 
		{ 
			size_t BusID = std::stoull(Match.str());
			size_t NextDepartureIn = (BusID - (Timestamp % BusID));
			return { BusID, NextDepartureIn };
		}
	);

	auto NextBus = std::min_element(std::cbegin(Buses), std::cend(Buses), [](const std::pair<size_t, size_t>& A, const std::pair<size_t, size_t>& B)->bool { return A.second < B.second; });

	std::cout << "Part One: " << (NextBus->first * NextBus->second) << std::endl;
}
