// Day17.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

typedef std::pair<size_t, size_t> Coordinate;
typedef std::pair<Coordinate, std::string> Position;

constexpr size_t GridSize = 4;

std::string GetPath(const Coordinate & Start, const Coordinate & Destination, const std::string & Prefix);
void GetDoorStatus(const std::string & Hash, bool & Up, bool & Down, bool & Left, bool & Right);

int main()
{
	const std::string Input = "rrrbmfta";

	std::cout << "Path: " << GetPath(Coordinate(0, 0), Coordinate(3, 3), Input) << std::endl;

	system("pause");

    return 0;
}

size_t GetDistance(const Coordinate & Position, const Coordinate & Destination)
{
	int64_t dX = Destination.first - Position.first;
	int64_t dY = Destination.second - Position.second;

	return std::abs(dX) + std::abs(dY);
}

size_t GetHeuristic(const Coordinate & Position, const Coordinate & Destination, const std::string & Path)
{
	return GetDistance(Position, Destination) + Path.size();
}

void AddToList(std::multimap<size_t, Position> & OpenList, const Coordinate & Position, const Coordinate & Destination, const std::string & Path)
{
	OpenList.insert({ GetHeuristic(Position, Destination, Path), { Position, Path } });
}

std::string GetPath(const Coordinate & Start, const Coordinate & Destination, const std::string & Prefix)
{
	std::multimap<size_t, Position> OpenList = { { GetDistance(Start, Destination), { Start, "" } } };

	while (!OpenList.empty())
	{
		Position Node = OpenList.begin()->second;
		OpenList.erase(OpenList.begin());

		if (Node.first == Destination)
		{
			return Node.second;
		}

		bool Up, Down, Left, Right;
		GetDoorStatus(Prefix + Node.second, Up, Down, Left, Right);

		if (Up && (Node.first.second > 0))
		{
			AddToList(OpenList, { Node.first.first, Node.first.second - 1 }, Destination, Node.second + "U");
		}

		if (Down && (Node.first.second < GridSize - 1))
		{
			AddToList(OpenList, { Node.first.first, Node.first.second + 1 }, Destination, Node.second + "D");
		}

		if (Left && (Node.first.first > 0))
		{
			AddToList(OpenList, { Node.first.first - 1, Node.first.second }, Destination, Node.second + "L");
		}

		if (Right && (Node.first.first < GridSize - 1))
		{
			AddToList(OpenList, { Node.first.first + 1, Node.first.second }, Destination, Node.second + "R");
		}
	}

	return std::string();
}

void GetDoorStatus(const std::string & Hash, bool & Up, bool & Down, bool & Left, bool & Right)
{
	MD5 Hasher;
	MD5::Hash Result = Hasher.Compute(ByteVector(Hash.begin(), Hash.end()));

	Up = ((Result[0] & 0xF0) > 0xA0) ? true : false;
	Down = ((Result[0] & 0x0F) > 0x0A) ? true : false;

	Left = ((Result[1] & 0xF0) > 0xA0) ? true : false;
	Right = ((Result[1] & 0x0F) > 0x0A) ? true : false;
}