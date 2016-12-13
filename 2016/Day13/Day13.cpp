// Day13.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

typedef std::pair<uint64_t, uint64_t> Touple;

struct Map
{
	Map(uint64_t Seed)
		:Seed(Seed)
	{}

	bool IsFree(const Touple & Coordinate)
	{
		auto Result = Cache.find(Coordinate);
		if (Result != Cache.end())
		{
			return Result->second;
		}
		else
		{
			return CalculateSpace(Coordinate);
		}
	}

private:
	uint64_t Seed;
	std::map<Touple, bool> Cache;

	bool CalculateSpace(const Touple & Coordinate)
	{
		uint64_t X = Coordinate.first;
		uint64_t Y = Coordinate.second;

		uint64_t Number = X * X + 3 * X + 2 * X * Y + Y + Y * Y + Seed;
		std::bitset<64> Bits = Number;

		size_t BitCount = Bits.count();

		bool IsFree = ((BitCount % 2) == 0);
		Cache[Coordinate] = IsFree;

		return IsFree;
	}
};

uint64_t Pathfinding(Map & Maze, const Touple & Start, const Touple * Destination);
void InsertNode(Map & Maze, const Touple * Destination, Touple Node, uint64_t Steps, std::set<Touple> & ClosedList, std::map<Touple, uint64_t> & StepsPerCorrdinate, std::multimap<float, Touple> & FPerCorrdinate);

int main()
{
	constexpr uint64_t Input = 1350;
	constexpr Touple Start = { 1,1 };
	constexpr Touple Destination = { 31, 39 };

	Map Maze(Input);

	uint64_t Steps = Pathfinding(Maze, Start, &Destination);
	uint64_t Locations = Pathfinding(Maze, Start, nullptr);

	std::cout << "Steps: " << Steps << std::endl;
	std::cout << "Locations: " << Locations << std::endl;

	system("pause");

    return 0;
}

uint64_t Pathfinding(Map & Maze, const Touple & Start, const Touple * Destination)
{
	bool FindDesitnation = (Destination != nullptr);

	std::set<Touple> ClosedList;
	std::map<Touple, uint64_t> StepsPerCorrdinate;
	std::multimap<float, Touple> FPerCorrdinate;

	StepsPerCorrdinate[Start] = 0;
	FPerCorrdinate.emplace(0.f, Start);

	while (!FPerCorrdinate.empty())
	{
		float F = FPerCorrdinate.begin()->first;
		Touple Node = FPerCorrdinate.begin()->second;
		FPerCorrdinate.erase(FPerCorrdinate.begin());

		if (FindDesitnation && (Node == *Destination))
		{
			return StepsPerCorrdinate[Node];
		}
		else if (!FindDesitnation && (F > 50.f))
		{
			return ClosedList.size();
		}

		ClosedList.emplace(Node);
		uint64_t Steps = StepsPerCorrdinate[Node] + 1;

		if (Node.first > 0)
			InsertNode(Maze, Destination, { Node.first - 1, Node.second }, Steps, ClosedList, StepsPerCorrdinate, FPerCorrdinate);

		InsertNode(Maze, Destination, { Node.first + 1, Node.second }, Steps, ClosedList, StepsPerCorrdinate, FPerCorrdinate);

		if (Node.second > 0)
			InsertNode(Maze, Destination, { Node.first, Node.second - 1}, Steps, ClosedList, StepsPerCorrdinate, FPerCorrdinate);

		InsertNode(Maze, Destination, { Node.first, Node.second + 1}, Steps, ClosedList, StepsPerCorrdinate, FPerCorrdinate);

	}

	return 0;
}

void InsertNode(Map & Maze, const Touple * Destination, Touple Node, uint64_t Steps, std::set<Touple> & ClosedList, std::map<Touple, uint64_t> & StepsPerCorrdinate, std::multimap<float, Touple> & FPerCorrdinate)
{
	if (ClosedList.find(Node) != ClosedList.end())
		return;

	if (!Maze.IsFree(Node))
		return;

	auto StepsResult = StepsPerCorrdinate.insert({ Node, Steps });
	if (!StepsResult.second)
	{
		if (Steps >= StepsResult.first->second)
			return;

		StepsResult.first->second = Steps;
	}

	float f = Steps;

	if (Destination != nullptr)
	{
		float dX = static_cast<float>(Destination->first - Node.first);
		float dY = static_cast<float>(Destination->second - Node.second);

		f += std::sqrtf(dX * dX + dY * dY);
	}

	FPerCorrdinate.emplace(f, Node);
}