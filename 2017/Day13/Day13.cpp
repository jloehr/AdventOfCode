// Day13.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int main()
{
	const StringVectorVector Lines = GetFileLineParts("Input.txt");
	std::vector<std::pair<size_t, size_t>> Layers;
	size_t Severity = 0;

	for (const StringVector & Line : Lines)
	{
		size_t TimeStep = std::stoull(Line[0]);
		size_t Depth = std::stoull(Line[1]);
		if (TimeStep % ((Depth - 1) * 2) == 0)
			Severity += TimeStep * Depth;

		Layers.emplace_back(TimeStep, Depth);
	}

	std::cout << "Severity: " << Severity << std::endl;

	size_t Delay = 0;
	while (std::any_of(std::begin(Layers), std::end(Layers), [=](const auto & Layer)->bool { return (((Layer.first + Delay) % ((Layer.second - 1) * 2)) == 0); }))
	{
		++Delay;
	}

	std::cout << "Delay: " << Delay << std::endl;

	system("pause");
    return 0;
}

