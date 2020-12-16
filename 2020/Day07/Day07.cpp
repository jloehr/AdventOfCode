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
typedef std::stack<BagPtr> BagStack;
typedef std::unordered_map<std::string, BagPtr> BagMap;

struct Bag
{
	std::string Name;
	BagSet Contains;
	BagSet ContainedBy;

	Bag(const std::string& Name) : Name(Name) {};
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

		BagPtr & BagInstance = KnownBags[BagName];
		if (!BagInstance) BagInstance = std::make_shared<Bag>(BagName);

		for (std::sregex_iterator Match = std::sregex_iterator(CurrentBagMatch[2].second, Line.cend(), LineMatching); Match != std::sregex_iterator(); Match++)
		{
			const std::string EmbeddedBagName = Match->operator[](2).str();

			BagPtr& EmbeddedBagInstance = KnownBags[EmbeddedBagName];
			if (!EmbeddedBagInstance) EmbeddedBagInstance = std::make_shared<Bag>(EmbeddedBagName);

			BagInstance->Contains.insert(EmbeddedBagInstance);
			EmbeddedBagInstance->ContainedBy.insert(BagInstance);
		}
	}

	BagSet VisitedBags;
	BagStack BagStack;
	BagStack.push(KnownBags["shiny gold"]);

	while (!BagStack.empty())
	{
		BagPtr CurrentBag = BagStack.top();
		BagStack.pop();

		if (VisitedBags.insert(CurrentBag).second)
			for (const auto& EmbeddedBag : CurrentBag->ContainedBy)
				BagStack.push(EmbeddedBag);
	}

	std::cout << "Part One: " << (VisitedBags.size() - 1) << std::endl;
}
