// Day10.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

constexpr std::array<uint8_t,16> Lengths{ 147,37,249,1,31,2,226,0,161,71,254,243,183,255,30,70 };

int main()
{
	std::array<uint8_t, 256> List;
	std::iota(List.begin(), List.end(), 0);

	uint8_t SkipSize = 0;	
	size_t Cursor = 0;
	for (auto Length : Lengths)
	{
		for (auto i = 0; i < Length / 2; ++i)
			std::swap(List[(Cursor + i) % List.size()], List[(Cursor + Length - 1 - i) % List.size()]);

		Cursor = (Cursor + Length + SkipSize++) % List.size();
	}

	std::cout << List[0] * List[1] << std::endl;

	system("pause");
    return 0;
}

