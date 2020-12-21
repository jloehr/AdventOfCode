// Day16.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <regex>
#include <functional>

#include "../../Common/Common.h"

typedef std::function<bool(uint64_t)> Validator;

int main()
{
	std::regex RuleMatching("(.+): (\\d+)-(\\d+) or (\\d+)-(\\d+)", std::regex_constants::optimize);
	std::regex FieldMatching("(\\d+)", std::regex_constants::optimize);

	const StringVector Lines = GetFileLines("Input.txt");
	StringVector::const_iterator Line = std::cbegin(Lines);

	std::vector<Validator> Rules;

	std::smatch Match;
	while (std::regex_match(*(Line++), Match, RuleMatching))
	{
		uint64_t Min1 = std::stoull(Match[2]);
		uint64_t Max1 = std::stoull(Match[3]);
		uint64_t Min2 = std::stoull(Match[4]);
		uint64_t Max2 = std::stoull(Match[5]);

		Rules.push_back([=](uint64_t Value)->bool { return ((Value >= Min1) && (Value <= Max2) || (Value >= Min2) && (Value <= Max2)); });
	}

	// Skip until "nearby tickets:"

	uint64_t ErrorRate = 0;
	for(Line = std::next(Line, 5); Line != std::cend(Lines); Line++)
	{
		for (auto Field = std::sregex_iterator(std::cbegin(*Line), std::cend(*Line), FieldMatching); Field != std::sregex_iterator(); Field++)
		{
			uint64_t FieldValue = std::stoull((*Field)[1]);
			if (!std::all_of(std::cbegin(Rules), std::cend(Rules), [=](const auto& Validator) { return Validator(FieldValue); }))
				ErrorRate += FieldValue;
		}
	}

	std::cout << "Part One: " << ErrorRate << std::endl;
}