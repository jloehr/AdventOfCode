// Day03.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

constexpr size_t ClothSize = 1000;
typedef std::array<size_t, ClothSize * ClothSize> ClothArray;
typedef std::shared_ptr<ClothArray> ClothArrayPtr;

int main()
{
	ClothArrayPtr Cloth = std::make_shared<ClothArray>();
	std::regex InputMatching("#(\\d+)\\s@\\s(\\d+),(\\d+):\\s(\\d+)x(\\d+)");
	StringVector File = GetFileLines("Input.txt");

	for (const std::string & Line : File)
	{
		std::smatch Matches;
		if (!std::regex_match(Line, Matches, InputMatching))
		{
			std::cout << "Line \"" << Line << "\" did not match the regex!" << std::endl;
			return 1;
		}

		size_t XStart = std::stoull(Matches[2]);
		size_t YStart = std::stoull(Matches[3]);
		size_t Width = std::stoull(Matches[4]);
		size_t Height = std::stoull(Matches[5]);

		for (size_t X = XStart; X < (XStart + Width); ++X)
			for (size_t Y = YStart; Y < (YStart + Height); ++Y)
				++(*Cloth)[Y * ClothSize + X];
	}

	size_t PartOne = std::accumulate(
		Cloth->begin(),
		Cloth->end(),
		size_t(0), 
		[](const size_t Value, const size_t Field) -> size_t 
		{ 
			return (Field > 1) ? Value + 1 : Value; 
		});

	std::cout << "Part One: " << PartOne << std::endl;
}