// Day09.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "../../Common/Common.h"

typedef std::vector<uint64_t> U64Vector;
typedef std::array<U64Vector::value_type, 25> U64Array;

bool Valid(U64Vector::const_iterator LowValue, U64Vector::const_iterator HighValue, U64Vector::value_type Number);

int main()
{
	U64Vector Values = GetValues<U64Vector::value_type>("Input.txt");
	U64Vector::value_type InvalidNumber = 0;

	for (U64Vector::const_iterator CurrentNumber = std::begin(Values) + 25; CurrentNumber != std::end(Values); CurrentNumber++)
	{
		if (!Valid(CurrentNumber - 25, CurrentNumber, (*CurrentNumber)))
		{
			InvalidNumber = (*CurrentNumber);
			break;
		}
	}

	U64Vector::iterator LowIterator = std::begin(Values);
	U64Vector::iterator HighIterator = std::begin(Values);
	U64Vector::value_type Sum = *LowIterator;

	while (Sum != InvalidNumber)
	{
		if ((Sum < InvalidNumber) || (LowIterator == HighIterator))
		{
			Sum += (*(++HighIterator));
		}
		else
		{
			Sum -= (*(LowIterator++));
		}
	}

	std::sort(LowIterator, HighIterator + 1);
	std::cout << "Part One: " << InvalidNumber << std::endl;
	std::cout << "Part Two: " << ((*LowIterator) + (*HighIterator)) << std::endl;
}

bool Valid(U64Vector::const_iterator Begin, U64Vector::const_iterator End, U64Vector::value_type Number)
{
	U64Array Values;
	std::partial_sort_copy(Begin, End, std::begin(Values), std::end(Values));

	U64Array::const_iterator LowValue = std::cbegin(Values);
	U64Array::const_iterator HighValue = std::cend(Values) - 1;

	while (LowValue != HighValue)
	{
		U64Vector::value_type Sum = *LowValue + *HighValue;

		if (Sum == Number)
			return true;
		else if (Sum > Number)
			HighValue--;
		else
			LowValue++;
	}

	return false;
}