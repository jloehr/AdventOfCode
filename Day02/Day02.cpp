// Day2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int main()
{
	std::ifstream Input("Input.txt");

	if (!Input.is_open())
	{
		std::cout << "Error opening File!" << std::endl;
		return 0;
	}

	std::string Line;
	uint64_t WrappingPaper = 0;
	uint64_t Ribbon = 0;

	while (std::getline(Input, Line))
	{
		std::stringstream LineStream(Line);

		uint32_t l;
		uint32_t h;
		uint32_t w;

		LineStream >> l;
		LineStream.get();
		LineStream >> h;
		LineStream.get();
		LineStream >> w;

		uint32_t Side1 = l * w;
		uint32_t Side2 = h * w;
		uint32_t Side3 = l * h;

		uint32_t SmallestSide = std::min({ Side1, Side2, Side3 });
		uint32_t LargestDimension = std::max({ l, w, h });

		WrappingPaper += SmallestSide + 2 * Side1 + 2 * Side2 + 2 * Side3;
		Ribbon += l * w * h + 2 * l + 2 * w + 2 * h - 2 * LargestDimension;
	}

	Input.close();

	std::cout << "Wrapping Paper: " << WrappingPaper << std::endl;
	std::cout << "Ribbon: " << Ribbon << std::endl;

	system("pause");

    return 0;
}

