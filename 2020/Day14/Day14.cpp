// Day14.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iterator>
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
	std::unordered_map<size_t, U36> V1Memory, V2Memory;
	U36 V1AndMask, V1OrMask;
	U36 V2Mask;
	std::vector<size_t> V2FloatingBits;

	for (const auto& Line : Lines)
	{
		std::smatch Match;
		if (std::regex_match(Line, Match, MaskMatching))
		{
			V2FloatingBits.clear();
			for (auto MaskCharacter = Match[1].first; MaskCharacter != Match[1].second; MaskCharacter++)
			{
				// Part One
				V1AndMask <<= 1;
				V1OrMask <<= 1;
				if (*MaskCharacter != '0') V1AndMask.set(0);
				if (*MaskCharacter == '1') V1OrMask.set(0);

				// Part Two
				V2Mask <<= 1;
				if (*MaskCharacter == '1') V2Mask.set(0);
				if (*MaskCharacter == 'X') V2FloatingBits.push_back(std::distance(MaskCharacter, Match[1].second) - 1);
			}
		}
		else if(std::regex_match(Line, Match, MemoryMatching))
		{
			// Part One
			uint64_t Number = std::stoull(Match[2]);
			size_t MemoryAddress = std::stoull(Match[1]);
			V1Memory[MemoryAddress] = ((U36(Number) & V1AndMask) | V1OrMask);

			//Part Two
			U36 V2MemoryAddress = U36(MemoryAddress) | V2Mask;
			for (size_t BitPermutation = 0; BitPermutation < (static_cast<size_t>(1) << V2FloatingBits.size()); BitPermutation++)
			{
				U36 Permutation(BitPermutation);
				for (size_t Bit = 0; Bit < V2FloatingBits.size(); Bit++) V2MemoryAddress[V2FloatingBits[Bit]] = Permutation[Bit];
				V2Memory[V2MemoryAddress.to_ullong()] = Number;
			}
		}
		else
		{
			std::cout << "Line didn't match any regex!" << std::endl;
		}
	}

	auto MemorySum = [](const std::unordered_map<size_t, U36>& Memory)->uint64_t
	{
		return std::accumulate(
			std::cbegin(Memory),
			std::cend(Memory),
			static_cast<uint64_t>(0), [](uint64_t Sum, std::pair<size_t, U36> Memory) { return Sum += Memory.second.to_ullong(); }
		);
	};

	std::cout << "Part One: " << MemorySum(V1Memory) << std::endl;
	std::cout << "Part Two: " << MemorySum(V2Memory) << std::endl;
}
