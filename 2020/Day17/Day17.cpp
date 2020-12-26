// Day17.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <unordered_set>
#include <functional>

#include "../../Common/Common.h"

struct Coordinate
{
    int64_t X, Y, Z;

    Coordinate(int64_t X, int64_t Y, int64_t Z) : X(X), Y(Y), Z(Z) { }

    bool operator==(const Coordinate& other) const
    {
        return 
            (X == other.X) &&
            (Y == other.Y) &&
            (Z == other.Z);
    }
};

struct CoordinateHasher
{
    std::size_t operator() (const Coordinate& Coordinate) const
    {
        return (Coordinate.X * 73856093) xor (Coordinate.Y * 19349663) xor (Coordinate.Z * 83492791);
    }
};

typedef std::unordered_set<Coordinate, CoordinateHasher> CoordinateSet;

void Run(CoordinateSet& Space, size_t Cycles);
void ForEachNeighborCell(const Coordinate& Cell, std::function<void(const Coordinate&)> Function);

int main()
{
    const StringVector Lines = GetFileLines("Input.txt");

    CoordinateSet Space;
    for (size_t Y = 0; Y < Lines.size(); Y++)
        for (size_t X = 0; X < Lines[Y].size(); X++)
            if (Lines[Y][X] == '#') Space.emplace(X, Y, 0);

    Run(Space, 6);

    std::cout << "Part One: " << Space.size() << std::endl;
}

void Run(CoordinateSet& Space, size_t Cycles)
{
    for(; Cycles > 0; Cycles--)
    {
        CoordinateSet NewSpace;
        CoordinateSet EmptyNeighbors;

        for (const auto& Cell : Space)
        {
            size_t ActiveNeighbors = 0;
            ForEachNeighborCell(Cell, [&](const Coordinate& Neighbor)
                {
                    if (Space.find(Neighbor) != std::end(Space))
                        ActiveNeighbors++;
                    else
                        EmptyNeighbors.insert(Neighbor);
                });

            if ((ActiveNeighbors == 2) || (ActiveNeighbors == 3))
                NewSpace.insert(Cell);
        }

        for (const auto& Cell : EmptyNeighbors)
        {
            size_t ActiveNeighbors = 0;
            ForEachNeighborCell(Cell, [&](const Coordinate& Neighbor)
                {
                    if (Space.find(Neighbor) != std::end(Space))
                        ActiveNeighbors++;
                });

            if ((ActiveNeighbors == 3))
                NewSpace.insert(Cell);
        }

        std::swap(Space, NewSpace);
    }
}

void ForEachNeighborCell(const Coordinate& Cell, std::function<void(const Coordinate&)> Function)
{
    for (int64_t XDir = -1; XDir <= 1; XDir++)
        for (int64_t YDir = -1; YDir <= 1; YDir++)
            for (int64_t ZDir = -1; ZDir <= 1; ZDir++)
            {
                if ((XDir == 0) && (YDir == 0) && (ZDir == 0)) continue;
                Function(Coordinate(Cell.X + XDir, Cell.Y + YDir, Cell.Z + ZDir));
            }
}