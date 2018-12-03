// Day03.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

constexpr size_t ClothSize = 1000;
typedef std::array<std::pair<size_t, size_t>, ClothSize * ClothSize> ClothArray;
typedef std::shared_ptr<ClothArray> ClothArrayPtr;

int main()
{
	ClothArrayPtr Cloth = std::make_shared<ClothArray>();
	std::regex InputMatching("#(\\d+)\\s@\\s(\\d+),(\\d+):\\s(\\d+)x(\\d+)");
	StringVector File = GetFileLines("Input.txt");
	size_t OverlappingSquares = 0;
	std::unordered_set<size_t> NonOverlappingIDs;

	for (const std::string & Line : File)
	{
		std::smatch Matches;
		if (!std::regex_match(Line, Matches, InputMatching))
		{
			std::cout << "Line \"" << Line << "\" did not match the regex!" << std::endl;
			return 1;
		}

		bool IsNonOverlapping = true;
		size_t ID = std::stoull(Matches[1]);
		size_t XStart = std::stoull(Matches[2]);
		size_t YStart = std::stoull(Matches[3]);
		size_t Width = std::stoull(Matches[4]);
		size_t Height = std::stoull(Matches[5]);

		for (size_t X = XStart; X < (XStart + Width); ++X)
			for (size_t Y = YStart; Y < (YStart + Height); ++Y)
			{
				auto & Field = (*Cloth)[Y * ClothSize + X];

				if (Field.first == 1)
					++OverlappingSquares;

				if (Field.first != 0)
				{
					NonOverlappingIDs.erase(Field.second);
					IsNonOverlapping = false;
				}

				++Field.first;
				Field.second = ID;
			}

		if (IsNonOverlapping)
			NonOverlappingIDs.insert(ID);
	}

	std::cout << "Part One: " << OverlappingSquares << std::endl;
	std::cout << "Part One: " << *NonOverlappingIDs.begin() << std::endl;
}