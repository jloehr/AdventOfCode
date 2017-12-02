// Day02.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int main()
{
	StringVectorVector FileParts = GetFileLineParts("Input.txt");
	uint64_t Checksum = 0;

	for (const auto & Line : FileParts)
	{
		uint64_t Max = 0;
		uint64_t Min = UINT64_MAX;

		for (const auto & NumberString : Line)
		{
			uint64_t Number = std::stoull(NumberString);
			Max = std::max(Max, Number);
			Min = std::min(Min, Number);
		}

		Checksum += Max - Min;
	}

	std::cout << "Checksum: " << Checksum << std::endl;
	system("pause");

    return 0;
}

