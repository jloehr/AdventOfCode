// Day03.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

constexpr uint64_t Input = 289326;

uint64_t GetUpperRingBound(const uint64_t Ring)
{
	return ((Ring * 2) + 1) * ((Ring * 2) + 1);
}

int main()
{
	uint64_t Ring = 0;

	while (Input > GetUpperRingBound(Ring))
		Ring++;

	uint64_t Steps = Ring;
	uint64_t Remaining = Input - GetUpperRingBound(Ring - 1);
	if (Ring > 0)
	{
		Remaining %= (Ring * 2);
		Steps += (Remaining < Ring) ? Ring - Remaining : Remaining - Ring;
	}

	std::cout << "Ring: " << Steps << std::endl;

	system("pause");
    return 0;
}

