// Day06.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

constexpr uint32_t TotalDistanceLimit = 10000;

struct Area 
{
	int32_t X, Y, Size;
	bool Infinite;
};

int main()
{
	const StringVectorVector File = GetFileLineParts("Input.txt");
	std::vector<Area> Areas;
	std::vector<std::pair<Area *, uint32_t>> Distances;
	int32_t MinX = 0;
	int32_t MaxX = 0;
	int32_t MinY = 0;
	int32_t MaxY = 0;
	int32_t PartTwoAreaSize = 0;

	std::transform(
		std::begin(File),
		std::end(File),
		std::back_inserter(Areas),
		[&](const auto & Line)
	{
		int32_t X = std::stoul(Line[0]);
		int32_t Y = std::stoul(Line[1]);

		MinX = std::min(MinX, X);
		MaxX = std::max(MaxX, X);
		MinY = std::min(MinY, Y);
		MaxY = std::max(MaxY, Y);

		return Area{ X, Y, 0, false };
	});

	Distances.reserve(Areas.size());

	for (int32_t Y = MinY; Y <= MaxY; ++Y)
	{
		for (int32_t X = MinX; X <= MaxX; ++X)
		{
			Distances.clear();

			for (Area & Start : Areas)
				Distances.push_back({ &Start, static_cast<uint32_t>(std::abs(X - Start.X) + std::abs(Y - Start.Y))});

			std::sort(std::begin(Distances), std::end(Distances), [](const auto & r, const auto & l) { return r.second < l.second; });
			uint32_t TotalDistances = std::accumulate(std::begin(Distances), std::end(Distances), 0, [](const uint32_t Value, const auto & Distance) { return Value + Distance.second; });

			if (TotalDistances < TotalDistanceLimit)
				++PartTwoAreaSize;

			// Same distance to multiple Starting Points, therefore tied
			if (Distances[0].second == Distances[1].second)
				continue;

			Area & Closest = *Distances[0].first;

			++Closest.Size;
			if ((!Closest.Infinite) && ((X == MinX) || (X == MaxX) || (Y == MinY) || (Y == MaxY)))
				Closest.Infinite = true;
		}
	}

	std::sort(std::begin(Areas), std::end(Areas), [](const Area & r, const Area & l) { return r.Size > l.Size; });
	Area & Largest = *std::find_if(std::begin(Areas), std::end(Areas), [](const Area & k) { return !k.Infinite; });

	std::cout << "Part One: " << Largest.Size << std::endl;
	std::cout << "Part Two: " << PartTwoAreaSize << std::endl;
}
