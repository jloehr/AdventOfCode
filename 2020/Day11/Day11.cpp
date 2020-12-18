// Day11.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "../../Common/Common.h"

enum class Tile { Empty, Taken, Floor };
typedef std::pair<size_t, size_t> Coordinate;

int main()
{
	const StringVector Lines = GetFileLines("Input.txt");
	const size_t Height = Lines.size();
	const size_t Width = Lines[0].size();

	std::vector<Tile> Map;
	Map.reserve(Height * Width);

	for (const auto& Line : Lines)
		std::transform(
			std::cbegin(Line), 
			std::cend(Line), 
			std::back_inserter(Map), 
			[](const char Input)->Tile { return (Input == 'L') ? Tile::Empty : Tile::Floor; }
		);

	auto GetIndex = [&](size_t X, size_t Y)->size_t { return Y * Width + X; };
	auto GetTile = [&](size_t X, size_t Y)->Tile { return ((X < 0) || (X >= Width) || (Y < 0) || (Y >= Height)) ? Tile::Floor : Map[GetIndex(X,Y)]; };
	auto IsTaken = [&](Coordinate Tile)->bool { return (GetTile(Tile.first, Tile.second) == Tile::Taken); };
	auto GetNewTileState = [&](size_t X, size_t Y)->Tile
	{
		const std::array<Coordinate, 8> NeighborCoordinates{ {
			{X - 1, Y - 1},
			{X + 0, Y - 1},
			{X + 1, Y - 1},
			{X - 1, Y + 0},
			{X + 1, Y + 0},
			{X - 1, Y + 1},
			{X + 0, Y + 1},
			{X + 1, Y + 1}
		} };

		switch (GetTile(X, Y))
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
				) >= 4) ? Tile::Empty : Tile::Taken;
		}
	};

	std::vector<Tile> NewMap(Map.size());
	bool Stabilized;
	do
	{
		Stabilized = true;

		for (size_t Y = 0; Y < Height; Y++)
			for (size_t X = 0; X < Width; X++)
			{
				NewMap[GetIndex(X, Y)] = GetNewTileState(X, Y);
				Stabilized &= (NewMap[GetIndex(X, Y)] == Map[GetIndex(X, Y)]);
			}

		std::swap(Map, NewMap);
	} while (!Stabilized);

	std::cout << "Part One: " << std::count(std::cbegin(Map), std::cend(Map), Tile::Taken) << std::endl;
}
