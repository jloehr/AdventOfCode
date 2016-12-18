// Day18.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

constexpr size_t Rows = 40;

typedef std::vector<bool> Row;
typedef std::array <Row, Rows> Board;

Board PopulateBoard(const std::string & Input, size_t & SafeTiles);

int main()
{
	std::string Line = GetFileLines("Input.txt")[0];

	size_t SafeTiles;
	Board Floor = PopulateBoard(Line, SafeTiles);

	std::cout << "Safe Tiles: " << SafeTiles << std::endl;

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

Board PopulateBoard(const std::string & Input, size_t & SafeTiles)
{
	SafeTiles = 0;
	Board Floor;

	Floor[0] = FillRow(Input, SafeTiles);

	for (size_t i = 1; i < Rows; i++)
	{
		Floor[i] = ComputeRow(Floor[i - 1], SafeTiles);
	}

	return Floor;
}