// Day14.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <numeric>
#include <bitset>
#include <regex>
#include <unordered_map>

#include "../../Common/Common.h"

typedef std::bitset<36> U36;

int main()
{
	const std::regex MaskMatching("mask = ([X01]{36})", std::regex_constants::optimize);
	const std::regex MemoryMatching("mem\\[(\\d+)\\] = (\\d+)", std::regex_constants::optimize);

	const StringVector Lines = GetFileLines("Input.txt");
	std::unordered_map<size_t, U36> Memory;
	U36 AndMask, OrMask;

	for (const auto& Line : Lines)
	{
		std::smatch Match;
		if (std::regex_match(Line, Match, MaskMatching))
		{
			for (const char MaskCharacter : Match[1].str())
			{
				AndMask <<= 1;
				OrMask <<= 1;

				if (MaskCharacter != '0') AndMask.set(0);
				if (MaskCharacter == '1') OrMask.set(0);
			}
		}
		else if(std::regex_match(Line, Match, MemoryMatching))
		{
			U36 Number = ((U36(std::stoull(Match[2])) & AndMask) | OrMask);
			Memory[std::stoull(Match[1])] = Number;
		}
		else
		{
			std::cout << "Line didn't match any regex!" << std::endl;
		}
	}

	uint64_t MemorySum = std::accumulate(
			std::cbegin(Memory), 
			std::cend(Memory), 
			static_cast<uint64_t>(0), [](uint64_t Sum, std::pair<size_t, U36> Memory) { return Sum += Memory.second.to_ullong(); }
		);
	std::cout << "Part One: " << MemorySum << std::endl;
}
