// Day20.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

static const size_t Input = 36000000;

static void PartOne();
static void PartTwo();

int main()
{
	PartOne();

	PartTwo();


	system("pause");

    return 0;
}

static void PartOne()
{
	// House Value = Acum( Dividors ) * 10
	//  1: 1  - 1				= 1
	//  2: 3  - 1, 2			= 1 * 2
	//  3: 4  - 1, 3			= 1 * 3
	//  4: 7  - 1, 2, 4			= 1 * 2 * 2
	//  5: 6  - 1, 5			= 1 * 5
	//  6: 12 - 1, 2, 3, 6		= 1 * 2 * 3
	//  7: 8  - 1, 7			= 1 * 7
	//  8: 15 - 1, 2, 4, 8		= 1 * 2 * 2 * 2
	//  9: 13 - 1, 3, 9			= 1 * 3 * 3
	// 10: 18 - 1, 2, 5, 10		= 1 * 2 * 5
	// 11: 12 - 1, 11			= 1 * 11
	// 12: 28 - 1, 2, 3, 4, 6, 12 = 1 * 2 * 2 * 3
	// 13: 14 - 1, 13			= 1 * 13
	// 14: 24 - 1, 2, 7, 14		= 1 * 2 * 7
	// 15: 24 - 1, 3, 5, 15		= 1 * 3 * 5
	// 16: 31 - 1, 2, 4, 8, 16	= 1 * 2 * 2 * 2 * 2
	// 17: 18 - 1, 17			= 1 * 17
	// 18: 39 - 1, 2, 3, 6, 9, 18	= 1 * 2 * 3 * 3
	// 19: 20 - 1, 19			= 1 * 19
	// 20: 42 - 1, 2, 4, 5, 10, 20	= 1 * 2 * 2 * 5
	// 21: 32 - 1, 3, 7, 21		= 1 * 3 * 7

	size_t Value = 0;
	size_t Number = 0;

	do
	{
		Value = 0;
		++Number;

		size_t SqrRoot = (size_t)std::sqrt(Number);

		for (size_t Dividor = 1; Dividor <= SqrRoot; Dividor++)
		{
			if ((Number % Dividor) == 0)
			{
				Value += Dividor;
				size_t OtherDividor = (Number / Dividor);

				if (Dividor != OtherDividor)
				{
					Value += OtherDividor;
				}
			}
		}

		/*if ((Number % 10000) == 0)
		{
		std::cout << Number << ": " << Value << std::endl;
		}*/

	} while (Value < (Input / 10));

	std::cout << "Part One: " << Number << " - " << Value << std::endl;
}

static void PartTwo()
{
	size_t Value = 0;
	size_t Number = 0;

	// 100: (217) 216 - (1,) 2, 4, 5, 10, 20, 25, 50, 100

	do
	{
		Value = 0;
		++Number;

		size_t SqrRoot = (size_t)std::sqrt(Number);
		size_t MinDividor = (((Number - 1) / 50) + 1);

		for (size_t Dividor = 1; Dividor <= SqrRoot; Dividor++)
		{
			if ((Number % Dividor) == 0)
			{
				Value += (Dividor >= MinDividor) ? Dividor : 0;

				size_t OtherDividor = (Number / Dividor);

				if (Dividor != OtherDividor)
				{
					Value += (OtherDividor >= MinDividor) ? OtherDividor : 0;
				}
			}
		}

		Value *= 11;

		/*if ((Number % 10000) == 0)
		{
			std::cout << Number << ": " << Value << std::endl;
		}*/

	} while (Value < Input);

	std::cout << "Part Two: " << Number << " - " << Value << std::endl;
}