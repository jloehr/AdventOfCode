// Day05.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

std::string Collapse(std::istream & Input);

int main()
{
	std::ifstream Input("Input.txt");
	std::string Polymer = Collapse(Input);

	std::cout << "Part One: " << Polymer.size() << std::endl;
	
	std::string UnitsToTest(std::begin(Polymer), std::end(Polymer));
	std::transform(std::begin(UnitsToTest), std::end(UnitsToTest), std::begin(UnitsToTest), std::toupper);
	std::sort(std::begin(UnitsToTest), std::end(UnitsToTest));
	auto Last = std::unique(std::begin(UnitsToTest), std::end(UnitsToTest));
	UnitsToTest.erase(Last, std::end(UnitsToTest));

	size_t Smallest = SIZE_MAX;

	for (const char UnitDeleted : UnitsToTest)
	{
		std::stringstream PolymerCopy;
		std::ostream_iterator<char> Inserter(PolymerCopy);
		std::copy_if(std::begin(Polymer), std::end(Polymer), Inserter, [UnitDeleted](const char Character) { return (std::toupper(Character) != UnitDeleted); });

		std::string Collapsed = Collapse(PolymerCopy);
		Smallest = std::min(Smallest, Collapsed.size());
	}

	std::cout << "Part Two: " << Smallest << std::endl;
}

std::string Collapse(std::istream & Input)
{
	std::string Polymer;
	size_t Pointer = 0;
	char Character;

	while (Input.get(Character))
	{
		if ((Pointer == 0) || !((Character == (Polymer[Pointer - 1] + 32)) || (Character == (Polymer[Pointer - 1] - 32))))
		{
			if ((Pointer == Polymer.size()))
				Polymer.push_back(Character);
			else
				Polymer[Pointer] = Character;

			++Pointer;
		}
		else
			--Pointer;
	}

	Polymer.resize(Pointer);
	return Polymer;
}
