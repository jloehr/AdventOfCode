// Day16.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <regex>
#include <functional>
#include <numeric>
#include <unordered_set>

#include "../../Common/Common.h"

typedef std::function<bool(uint64_t)> Validator;
typedef std::pair<Validator, bool> FieldDescriptor;
typedef std::vector<FieldDescriptor> FieldDescriptors;
typedef std::vector<FieldDescriptors::const_iterator> FieldDescriptorList;
typedef std::vector<FieldDescriptorList> FieldDescriptorLists;

const std::regex RuleMatching("(departure)?.+: (\\d+)-(\\d+) or (\\d+)-(\\d+)", std::regex_constants::optimize);
const std::regex FieldMatching("(\\d+)", std::regex_constants::optimize);

FieldDescriptors FillRules(StringVector::const_iterator& Line);
FieldDescriptorLists ProcessNearbyTicket(const std::string& Line, const FieldDescriptorLists& PossibleFieldList, uint64_t& ErrorRate);
void ReducePossibleFields(FieldDescriptorLists& PossibleFieldList, FieldDescriptorList::value_type FieldDescriptor);
uint64_t GetDepartureProduct(const std::string& YourTicket, const FieldDescriptorLists& Fields);

int main()
{
	const StringVector Lines = GetFileLines("Input.txt");
	StringVector::const_iterator Line = std::cbegin(Lines);

	const FieldDescriptors Rules = FillRules(Line);
	const StringVector::const_iterator YourTicket = std::next(Line, 1);

	uint64_t ErrorRate = 0;
	FieldDescriptorLists PossibleFieldList(
		Rules.size(), 
		([&]()->FieldDescriptorList 
			{ 
				auto It = Rules.cbegin();  
				FieldDescriptorList NewList; 
				std::generate_n(std::back_inserter(NewList), Rules.size(), [&]() { return It++; }); 
				return NewList; 
			})());

	for(Line = std::next(YourTicket, 3); Line != std::cend(Lines); Line++)
		PossibleFieldList = ProcessNearbyTicket(*Line, PossibleFieldList, ErrorRate);

	std::cout << "Part One: " << ErrorRate << std::endl;
	std::cout << "Part Two: " << GetDepartureProduct(*YourTicket, PossibleFieldList) << std::endl;
}

FieldDescriptors FillRules(StringVector::const_iterator& Line)
{
	FieldDescriptors Rules;
	std::smatch Match;
	while (std::regex_match(*(Line++), Match, RuleMatching))
	{
		uint64_t Min1 = std::stoull(Match[2]);
		uint64_t Max1 = std::stoull(Match[3]);
		uint64_t Min2 = std::stoull(Match[4]);
		uint64_t Max2 = std::stoull(Match[5]);

		Rules.push_back(std::make_pair([=](uint64_t Value)->bool { return ((Value >= Min1) && (Value <= Max1) || (Value >= Min2) && (Value <= Max2)); }, Match[1].matched));
	}

	return Rules;
}

FieldDescriptorLists ProcessNearbyTicket(const std::string& Line, const FieldDescriptorLists& PossibleFieldList, uint64_t& ErrorRate)
{
	FieldDescriptorLists NewPossibleFieldList(PossibleFieldList);
	auto PossibleFieldsForPosition = std::begin(NewPossibleFieldList);

	for (auto Field = std::sregex_iterator(std::cbegin(Line), std::cend(Line), FieldMatching); Field != std::sregex_iterator(); Field++, PossibleFieldsForPosition++)
	{
		uint64_t FieldValue = std::stoull((*Field)[1]);
		FieldDescriptorList RemainingPossibleFields;

		std::copy_if(
			std::cbegin(*PossibleFieldsForPosition), 
			std::cend(*PossibleFieldsForPosition), 
			std::back_inserter(RemainingPossibleFields), 
			[=](const auto& FieldDescriptor) 
			{ 
				return FieldDescriptor->first(FieldValue); 
			}
		);

		if (RemainingPossibleFields.size() == 0)
		{
			ErrorRate += FieldValue;
			return PossibleFieldList;
		}

		if (PossibleFieldsForPosition->size() != RemainingPossibleFields.size())
		{
			(*PossibleFieldsForPosition) = std::move(RemainingPossibleFields);

			if (PossibleFieldsForPosition->size() == 1)
				ReducePossibleFields(NewPossibleFieldList, (*PossibleFieldsForPosition)[0]);
		}
			
	}

	return NewPossibleFieldList;
}

void ReducePossibleFields(FieldDescriptorLists& PossibleFieldList, FieldDescriptorList::value_type FieldDescriptor)
{
	FieldDescriptorList FieldsToRemove = { FieldDescriptor };
	FieldsToRemove.reserve(PossibleFieldList.size());
	auto FieldToRemove = std::begin(FieldsToRemove);

	while (FieldToRemove != std::end(FieldsToRemove))
	{
		std::for_each(
			std::begin(PossibleFieldList),
			std::end(PossibleFieldList),
			[&](FieldDescriptorList& FieldDiscriptors)
			{
				if (FieldDiscriptors.size() == 1) return;
				auto FieldDiscriptorEntry = std::find(std::begin(FieldDiscriptors), std::end(FieldDiscriptors), *FieldToRemove);
				if (FieldDiscriptorEntry != std::end(FieldDiscriptors))
				{
					FieldDiscriptors.erase(FieldDiscriptorEntry);
					if (FieldDiscriptors.size() == 1)
						FieldsToRemove.push_back(FieldDiscriptors[0]);
				}
			});

		FieldToRemove++;
	}
}

uint64_t GetDepartureProduct(const std::string& YourTicket, const FieldDescriptorLists& Fields)
{
	return std::inner_product(
		std::sregex_iterator(std::cbegin(YourTicket), std::cend(YourTicket), FieldMatching),
		std::sregex_iterator(),
		std::cbegin(Fields),
		static_cast<uint64_t>(1u),
		std::multiplies<>(),
		[](const std::smatch& Number, const FieldDescriptorList& FieldList)->uint64_t
		{
			return (FieldList[0]->second) ? std::stoull(Number[1]) : 1;
		}
	);
}