// Day02.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int main()
{
	StringVectorVector FileParts = GetFileLineParts("Input.txt");
	uint64_t ChecksumPartOne = 0;
	uint64_t ChecksumPartTwo = 0;

	for (const auto & Line : FileParts)
	{
		std::vector<uint64_t> Numbers(Line.size());
		std::transform(Line.begin(), Line.end(), Numbers.begin(), [](const std::string & Number) -> uint64_t {
			return std::stoull(Number);
		});

		std::sort(Numbers.begin(), Numbers.end());
		ChecksumPartOne += *Numbers.rbegin() - *Numbers.begin();

		for (auto NumberIt = Numbers.begin(); NumberIt != Numbers.end(); ++NumberIt)
		{
			for (auto DivisorIt = Numbers.begin(); DivisorIt != NumberIt; ++DivisorIt)
			{
				if ((*NumberIt % *DivisorIt) == 0)
				{
					ChecksumPartTwo += (*NumberIt / *DivisorIt);
					NumberIt = Numbers.end() - 1;
					break;
				}
			}
		}
	}

	std::cout << "Checksum part one: " << ChecksumPartOne << std::endl;
	std::cout << "Checksum part two: " << ChecksumPartTwo << std::endl;
	system("pause");

    return 0;
}

