// Day11.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <functional>

#include "../../Common/Common.h"

enum class Tile { Empty, Taken, Floor };
typedef std::vector<Tile> TileVector;
typedef std::pair<size_t, size_t> Coordinate;
typedef std::array<Coordinate, 8> Neighbors;
typedef std::vector<Neighbors> NeighborVector;
typedef std::function<Neighbors(size_t X, size_t Y)> NeighborsGenerator;

struct Map
{
public:
	const size_t Width;
	const size_t Height;
	TileVector Seats;

	Map(size_t Width, size_t Height) : Width(Width), Height(Height)
	{
		Seats.reserve(Width * Height);
	}

	size_t GetIndex(size_t X, size_t Y) const
	{ 
		return Y * Width + X; 
	}

	bool IsCoordinateValid(size_t X, size_t Y) const 
	{ 
		return !((X < 0) || (X >= Width) || (Y < 0) || (Y >= Height)); 
	}

	Tile GetTile(size_t X, size_t Y) const
	{ 
		return IsCoordinateValid(X, Y) ? Seats[GetIndex(X, Y)] : Tile::Floor; 
	};
};

size_t RunMap(Map Map, const size_t TakenThreshold, const NeighborsGenerator Generator);

int main()
{
	const StringVector Lines = GetFileLines("Input.txt");
	const size_t Height = Lines.size();
	const size_t Width = Lines[0].size();

	Map Map(Width, Height);

	for (const auto& Line : Lines)
		std::transform(
			std::cbegin(Line), 
			std::cend(Line), 
			std::back_inserter(Map.Seats), 
			[](const char Input)->Tile { return (Input == 'L') ? Tile::Empty : Tile::Floor; }
		);

	// Part One
	auto GetDirectNeigbors = [](size_t X, size_t Y)->Neighbors 
	{
		return { {
			{X - 1, Y - 1},
			{X + 0, Y - 1},
			{X + 1, Y - 1},
			{X - 1, Y + 0},
			{X + 1, Y + 0},
			{X - 1, Y + 1},
			{X + 0, Y + 1},
			{X + 1, Y + 1}
		} };
	};

	// Part Two
	NeighborVector DistanceNeighbors;
	DistanceNeighbors.reserve(Map.Seats.size());

	auto GetDistanceSeat = [=](size_t X, size_t Y, size_t XDir, size_t YDir)->Coordinate {
		do
		{
			X += XDir;
			Y += YDir;
		} while (Map.IsCoordinateValid(X, Y) && (Map.GetTile(X, Y) == Tile::Floor));

		return { X,Y };
	};

	for (size_t Y = 0; Y < Height; Y++)
		for (size_t X = 0; X < Width; X++)
		{
			DistanceNeighbors.push_back(
				{
					GetDistanceSeat(X, Y, -1, -1),
					GetDistanceSeat(X, Y, +0, -1),
					GetDistanceSeat(X, Y, +1, -1),
					GetDistanceSeat(X, Y, -1, +0),
					GetDistanceSeat(X, Y, +1, +0),
					GetDistanceSeat(X, Y, -1, +1),
					GetDistanceSeat(X, Y, +0, +1),
					GetDistanceSeat(X, Y, +1, +1)
				});
		}

	auto GetDistanceNeigbors = [=](size_t X, size_t Y)->Neighbors { return DistanceNeighbors[Map.GetIndex(X, Y)]; };

	std::cout << "Part One: " << RunMap(Map, 4, GetDirectNeigbors) << std::endl;
	std::cout << "Part Two: " << RunMap(Map, 5, GetDistanceNeigbors) << std::endl;
}

size_t RunMap(Map Map, const size_t TakenThreshold, const NeighborsGenerator Generator)
{
	auto IsTaken = [&](Coordinate Tile)->bool { return (Map.GetTile(Tile.first, Tile.second) == Tile::Taken); };
	auto GetNewTileState = [&](size_t X, size_t Y)->Tile
	{
		const Neighbors NeighborCoordinates = Generator(X, Y);

		switch (Map.GetTile(X, Y))
		{
		case Tile::Floor:
			return Tile::Floor;
		case Tile::Empty:
			return
				std::none_of(
					std::cbegin(NeighborCoordinates),
					std::cend(NeighborCoordinates),
					IsTaken
				) ? Tile::Taken : Tile::Empty;
			break;
		case Tile::Taken:
			return (
				std::count_if(
					std::cbegin(NeighborCoordinates),
					std::cend(NeighborCoordinates),
					IsTaken
				) >= static_cast<ptrdiff_t>(TakenThreshold)) ? Tile::Empty : Tile::Taken;
		}
	};

	TileVector NewMap(Map.Seats.size());
	bool Stabilized;
	do
	{
		Stabilized = true;

		for (size_t Y = 0; Y < Map.Height; Y++)
			for (size_t X = 0; X < Map.Width; X++)
			{
				NewMap[Map.GetIndex(X, Y)] = GetNewTileState(X, Y);
				Stabilized &= (NewMap[Map.GetIndex(X, Y)] == Map.Seats[Map.GetIndex(X, Y)]);
			}

		std::swap(Map.Seats, NewMap);
	} while (!Stabilized);

	return std::count(std::cbegin(Map.Seats), std::cend(Map.Seats), Tile::Taken);
}