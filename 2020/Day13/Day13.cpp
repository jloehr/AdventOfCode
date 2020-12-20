// Day13.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <regex>
#include <algorithm>
#include <numeric>

#include "../../Common/Common.h"

struct Bus
{
	size_t BusID;
	size_t NextDepartureIn;
	size_t Offset;
};

int main()
{
	const std::regex BusMatching("((\\d+)|x)");
	StringVector Lines = GetFileLines("Input.txt");

	size_t Timestamp = std::stoull(Lines[0]);
	std::vector<Bus> Buses = std::accumulate(
		std::sregex_iterator(std::cbegin(Lines[1]), std::cend(Lines[1]), BusMatching), 
		std::sregex_iterator(), 
		std::vector<Bus>(),
		[=](std::vector<Bus> Buses, const std::smatch& Match)->std::vector<Bus>
		{ 
			static size_t Offset = 0;

			if (Match.str() != "x")
			{
				size_t BusID = std::stoull(Match.str());
				size_t NextDepartureIn = (BusID - (Timestamp % BusID));
				Buses.push_back(Bus{ BusID, NextDepartureIn, Offset });
			}

			Offset++;
			return std::move(Buses);
		}
	);

	auto NextBus = std::min_element(std::cbegin(Buses), std::cend(Buses), [](const Bus& A, const Bus& B)->bool { return A.NextDepartureIn < B.NextDepartureIn; });

	std::cout << "Part One: " << (NextBus->BusID * NextBus->NextDepartureIn) << std::endl;

	std::sort(std::begin(Buses), std::end(Buses), [](const Bus& A, const Bus& B)->bool { return A.BusID > B.BusID; });
	Timestamp = std::cbegin(Buses)->BusID - std::cbegin(Buses)->Offset;
	size_t TimestampSteps = std::cbegin(Buses)->BusID;

	for (auto Bus = std::next(std::cbegin(Buses)); Bus != std::cend(Buses); Bus++)
	{
		for (; ((Timestamp + Bus->Offset) % Bus->BusID) != 0; Timestamp += TimestampSteps);
		TimestampSteps *= Bus->BusID;
	}

	std::cout << "Part Two: " << Timestamp << std::endl;
}
