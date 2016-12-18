// Day18.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

constexpr size_t PartOne = 40;
constexpr size_t PartTwo = 400000;

typedef std::vector<bool> Row;

size_t ComputeRow(const std::string & Input, size_t RowCount);

int main()
{
	std::string Line = GetFileLines("Input.txt")[0];

	std::cout << PartOne << ": " << ComputeRow(Line, PartOne) << std::endl;
	std::cout << PartOne << ": " << ComputeRow(Line, PartTwo) << std::endl;

	system("pause");

    return 0;
}

Row FillRow(const std::string & Input, size_t & SafeTiles)
{
	Row NewRow;
	NewRow.reserve(Input.size());

	for (char Field : Input)
	{
		bool IsSafeTile = (Field == '.');
		NewRow.push_back(IsSafeTile);

		SafeTiles += IsSafeTile ? 1  : 0;
	}

	return NewRow;
}

bool IsSafe(const Row & PreviousRow, size_t Index)
{
	bool Left = (Index > 0) ? PreviousRow[Index - 1] : true;
	bool Right = (Index < (PreviousRow.size() - 1)) ? PreviousRow[Index + 1] : true;
		 
	return (Left == Right);
}

Row ComputeRow(const Row & PreviousRow, size_t & SafeTiles)
{
	Row NewRow;
	NewRow.reserve(PreviousRow.size());

	for (size_t i = 0; i < PreviousRow.size(); i++)
	{
		bool IsSafeTile = IsSafe(PreviousRow, i);
		NewRow.push_back(IsSafeTile);

		SafeTiles += IsSafeTile ? 1 : 0;
	}

	return NewRow;
}

size_t ComputeRow(const std::string & Input, size_t RowCount)
{
	size_t SafeTiles = 0;
	Row CurrentRow = FillRow(Input, SafeTiles);

	for (size_t i = 1; i < RowCount; i++)
	{
		Row NextRow = ComputeRow(CurrentRow, SafeTiles);
		std::swap(CurrentRow, NextRow);
	}

	return SafeTiles;
}