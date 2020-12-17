// Day07.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <regex>
#include <unordered_set>
#include <unordered_map>
#include <stack>

#include "../../Common/Common.h"

struct Bag;
typedef std::shared_ptr<Bag> BagPtr;
typedef std::unordered_set<BagPtr> BagSet;
typedef std::unordered_map<BagPtr, size_t> BagIntMap;
typedef std::unordered_map<std::string, BagPtr> BagMap;
typedef std::stack<BagPtr> BagStack;
typedef std::pair<BagPtr, BagIntMap::const_iterator> BagAndIter;
typedef std::stack<BagAndIter> BagAndIterStack;

struct Bag
{
	std::string Name;
	BagSet ContainedBy;
	BagIntMap Contains;
	size_t SumBagsContained;

	Bag(const std::string& Name) : Name(Name), SumBagsContained(0) {};
};

int main()
{
	std::regex LineMatching("(?:(\\d\\s)?(\\S+\\s\\S+) bags?)");
	const StringVector Lines = GetFileLines("Input.txt");

	BagMap KnownBags;

	for (const std::string& Line : Lines)
	{
		std::smatch CurrentBagMatch;
		std::regex_search(Line.cbegin(), Line.cend(), CurrentBagMatch, LineMatching);
		const std::string BagName = CurrentBagMatch[2].str();

		BagPtr& BagInstance = KnownBags[BagName];
		if (!BagInstance) BagInstance = std::make_shared<Bag>(BagName);

		for (std::sregex_iterator Match = std::sregex_iterator(CurrentBagMatch[2].second, Line.cend(), LineMatching); Match != std::sregex_iterator(); Match++)
		{
			// "contains no other bags"
			if (!(*Match)[1].matched) continue;

			const std::string EmbeddedBagName = (*Match)[2].str();

			BagPtr& EmbeddedBagInstance = KnownBags[EmbeddedBagName];
			if (!EmbeddedBagInstance) EmbeddedBagInstance = std::make_shared<Bag>(EmbeddedBagName);

			EmbeddedBagInstance->ContainedBy.insert(BagInstance);
			BagInstance->Contains.insert(std::make_pair(EmbeddedBagInstance, std::stoull((*Match)[1].str())));
		}
	}

	BagSet VisitedBags;
	BagStack BagStack;
	BagStack.push(KnownBags["shiny gold"]);

	while (!BagStack.empty())
	{
		BagPtr& CurrentBag = BagStack.top();
		BagStack.pop();

		if (VisitedBags.insert(CurrentBag).second)
			for (const auto& EmbeddedBag : CurrentBag->ContainedBy)
				BagStack.push(EmbeddedBag);
	}

	std::cout << "Part One: " << (VisitedBags.size() - 1) << std::endl;

	VisitedBags.clear();
	BagAndIterStack BagAndIterStack;
	BagAndIterStack.emplace(KnownBags["shiny gold"], std::cbegin(KnownBags["shiny gold"]->Contains));
	bool BagFinished = false;

	while (!BagAndIterStack.empty())
	{
		BagPtr& CurrentBag = BagAndIterStack.top().first;
		BagIntMap::const_iterator& NextBag = BagAndIterStack.top().second;

		if (BagFinished)
		{
			CurrentBag->SumBagsContained += NextBag->second * (NextBag->first->SumBagsContained + 1);
			NextBag++;
			BagFinished = false;
		}
		else if (NextBag != std::cend(CurrentBag->Contains))
		{
			const BagPtr& ContainedBag = (*NextBag).first;

			if (VisitedBags.find(ContainedBag) != std::cend(VisitedBags))
				BagFinished = true;
			else
				BagAndIterStack.push(std::make_pair(ContainedBag, std::cbegin(ContainedBag->Contains)));
		}
		else
		{
			VisitedBags.insert(CurrentBag);
			BagAndIterStack.pop();
			BagFinished = true;
		}
	}

	std::cout << "Part Two: " << KnownBags["shiny gold"]->SumBagsContained << std::endl;
}
