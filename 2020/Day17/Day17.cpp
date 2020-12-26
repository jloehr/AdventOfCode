// Day17.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <unordered_set>
#include <functional>

#include "../../Common/Common.h"

struct Coordinate
{
    int64_t X, Y, Z, W;

    Coordinate(int64_t X, int64_t Y, int64_t Z, int64_t W) : X(X), Y(Y), Z(Z), W(W) { }

    bool operator==(const Coordinate& other) const
    {
        return 
            (X == other.X) &&
            (Y == other.Y) &&
            (Z == other.Z) &&
            (W == other.W);
    }
};

struct CoordinateHasher
{
    std::size_t operator() (const Coordinate& Coordinate) const
    {
        return 
            (Coordinate.X * 73856093) xor 
            (Coordinate.Y * 19349663) xor 
            (Coordinate.Z * 83492791) xor
            (Coordinate.W * 45845791);
    }
};

typedef std::unordered_set<Coordinate, CoordinateHasher> CoordinateSet;

template<bool UseFourthDimension> size_t Run(CoordinateSet Space, size_t Cycles);
template<bool UseFourthDimension> void ForEachNeighborCell(const Coordinate& Cell, std::function<void(const Coordinate&)> Function);

int main()
{
    const StringVector Lines = GetFileLines("Input.txt");

    CoordinateSet Space;
    for (size_t Y = 0; Y < Lines.size(); Y++)
        for (size_t X = 0; X < Lines[Y].size(); X++)
            if (Lines[Y][X] == '#') Space.emplace(X, Y, 0, 0);

    std::cout << "Part One: " << Run<false>(Space, 6) << std::endl;
    std::cout << "Part Two: " << Run<true>(Space, 6) << std::endl;
}

template<bool UseFourthDimension>
size_t Run(CoordinateSet Space, size_t Cycles)
{
    for(; Cycles > 0; Cycles--)
    {
        CoordinateSet NewSpace;
        CoordinateSet EmptyNeighbors;

        for (const auto& Cell : Space)
        {
            size_t ActiveNeighbors = 0;
            ForEachNeighborCell<UseFourthDimension>(Cell, [&](const Coordinate& Neighbor)
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
            ForEachNeighborCell<UseFourthDimension>(Cell, [&](const Coordinate& Neighbor)
                {
                    if (Space.find(Neighbor) != std::end(Space))
                        ActiveNeighbors++;
                });

            if ((ActiveNeighbors == 3))
                NewSpace.insert(Cell);
        }

        std::swap(Space, NewSpace);
    }

    return Space.size();
}

template<bool UseFourthDimension>
void ForEachNeighborCell(const Coordinate& Cell, std::function<void(const Coordinate&)> Function)
{
    for (int64_t XDir = -1; XDir <= 1; XDir++)
        for (int64_t YDir = -1; YDir <= 1; YDir++)
            for (int64_t ZDir = -1; ZDir <= 1; ZDir++)
                for (int64_t WDir = (UseFourthDimension ? -1 : 0); WDir <= (UseFourthDimension ? 1 : 0); WDir++)
                {
                    if ((XDir == 0) && (YDir == 0) && (ZDir == 0) && (WDir == 0)) continue;
                    Function(Coordinate(Cell.X + XDir, Cell.Y + YDir, Cell.Z + ZDir, Cell.W + WDir));
                }
}