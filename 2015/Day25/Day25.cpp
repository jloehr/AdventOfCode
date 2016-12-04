// Day25.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

typedef std::pair<size_t, size_t> IntPair;

static IntPair GetCodeCoordinates(const std::string & InputFile);
static uint64_t GetCodePosition(const IntPair & Coordinates);
static uint64_t CalculateCode(uint64_t CodePosition);

static const uint64_t Start = 20151125;
static const uint64_t Multiplier = 252533;
static const uint64_t Dividor = 33554393;

int main()
{
	IntPair CodeCoordinate = GetCodeCoordinates("Input.txt");

	std::cout << CodeCoordinate.first << " - " << CodeCoordinate.second << std::endl;
	std::cout << GetCodePosition(CodeCoordinate) << std::endl;
	std::cout << CalculateCode(GetCodePosition(CodeCoordinate)) << std::endl;

	system("pause");

    return 0;
}


static IntPair GetCodeCoordinates(const std::string & InputFile)
{
	StringVectorVector InputParts = GetFileLineParts(InputFile);

	//Santa starts his rows and columns at 1 instead of 0
	return std::make_pair(std::atoi(InputParts[0][15].c_str()) - 1, std::atoi(InputParts[0][17].c_str()) - 1);
}

static uint64_t GetCodePosition(const IntPair & Coordinates)
{
	// Cantor Pairing Function, but Santa starts at 1 instead of 0
	return ((((Coordinates.first + Coordinates.second) * (Coordinates.first + Coordinates.second + 1) / 2) + Coordinates.second) + 1);
}

static uint64_t CalculateCode(uint64_t CodePosition)
{
	uint64_t Code = Start;

	for (size_t i = 1; i < CodePosition; i++)
	{
		Code *= Multiplier;
		Code %= Dividor;
	}

	return Code;
}