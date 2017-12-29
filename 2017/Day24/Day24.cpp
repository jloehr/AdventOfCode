// Day24.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

struct Component {
	std::array<uint8_t, 2> Ports;
	bool Available;

	Component(uint8_t Port1, uint8_t Port2)
		:Ports({Port1, Port2}), Available(true) {}
};

using PComponent = std::shared_ptr<Component>;
using ComponentVector = std::vector<PComponent>;
using ComponentMap = std::unordered_map<uint8_t, ComponentVector>;

size_t BuildBridge(const ComponentMap & Buckets, uint8_t Coupling, size_t Strength)
{
	auto Result = Buckets.find(Coupling);
	if (Result == std::end(Buckets))
		return Strength;

	size_t NewStrength = Strength;

	for(const auto & Component : Result->second)
	{
		if (!Component->Available)
			continue;

		Component->Available = false;
		uint8_t OtherCoupling = (Component->Ports[0] == Coupling) ? Component->Ports[1] : Component->Ports[0];
		NewStrength = std::max(BuildBridge(Buckets, OtherCoupling, Strength + Coupling + OtherCoupling), NewStrength);
		Component->Available = true;
	}

	return NewStrength;
}

int main()
{
	const StringVector Lines = GetFileLines("Input.txt");
	ComponentMap Buckets;

	for (const auto & Line : Lines)
	{
		size_t Delim = Line.find('/');
		PComponent NewComponent = std::make_shared<Component>(static_cast<uint8_t>(std::stoul(Line)), static_cast<uint8_t>(std::stoul(Line.substr(Delim + 1))));

		Buckets[NewComponent->Ports[0]].push_back(NewComponent);
		Buckets[NewComponent->Ports[1]].push_back(NewComponent);
	}

	std::cout << "Strongest Bridge: " << BuildBridge(Buckets, 0, 0) << std::endl;

	system("pause");
    return 0;
}

