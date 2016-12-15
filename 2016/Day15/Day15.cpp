// Day15.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

struct Disc
{
	size_t Positions;
	size_t Position;

	Disc(size_t Positions, size_t Position)
		:Positions(Positions), Position(Position)
	{}
};

typedef std::vector<Disc> DiscVector;

DiscVector CreateDiscStack(const StringVectorVector & Lines);
size_t GetButtonTime(DiscVector Discs);

int main()
{
	StringVectorVector Lines = GetFileLineParts("Input.txt");

	DiscVector Discs = CreateDiscStack(Lines);

	std::cout << "Part One: " << GetButtonTime(Discs) << std::endl;

	Discs.emplace_back(11, 0);

	std::cout << "Part Two: " << GetButtonTime(Discs) << std::endl;

	system("pause");

    return 0;
}

DiscVector CreateDiscStack(const StringVectorVector & Lines)
{
	DiscVector Discs;

	for (StringVector Line : Lines)
	{
		size_t Positions = std::stoi(Line[3]);
		size_t Start = std::stoi(Line[11]);

		Discs.emplace_back(Positions, Start);
	}

	return Discs;
}

size_t GetButtonTime(DiscVector Discs)
{
	size_t Time = 0;

	while (true)
	{
		bool GoesThrough = true;

		for (size_t i = 0; i < Discs.size(); i++)
		{
			if (!(((Time + i + 1 + Discs[i].Positions) + Discs[i].Position) % Discs[i].Positions) == 0)
			{
				GoesThrough = false;
				break;
			}
		}

		if (GoesThrough)
			return Time;

		Time++;
	}
}