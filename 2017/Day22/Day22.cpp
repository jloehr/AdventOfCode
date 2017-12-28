// Day22.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using Position = std::pair<int64_t, int64_t>;

int main()
{
	const StringVector Lines = GetFileLines("Input.txt");
	std::set<Position> Infected;
	Position Pos{ 0 , -1 * (Lines.size()/2) };
	uint64_t InfectionBurst = 0;

	for (const auto & Line : Lines)
	{
		Pos.first = -1 * (Line.size() / 2);
		for (char Cell : Line)
		{
			if (Cell == '#')
				Infected.insert(Pos);
			Pos.first++;
		}
		Pos.second++;
	}

	Pos = { 0, 0 };
	Position Direction{ 0, -1 };
	for (auto Burst = 0; Burst < 10000; ++Burst)
	{
		auto Result = Infected.find(Pos);
		if (Result == std::end(Infected))
		{
			InfectionBurst++;
			Infected.insert(Pos);
			std::swap(Direction.first, Direction.second);
			Direction.second *= -1;
		}
		else
		{
			Infected.erase(Result);
			std::swap(Direction.first, Direction.second);
			Direction.first *= -1;
		}

		Pos.first += Direction.first;
		Pos.second += Direction.second;
	}

	std::cout << "Burst of Infection: " << InfectionBurst << std::endl;

	system("pause");
    return 0;
}

