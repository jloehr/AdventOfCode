// Day06.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

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
	int32_t Width = 0;
	int32_t Height = 0;

	std::transform(
		std::begin(File),
		std::end(File),
		std::back_inserter(Areas),
		[&](const auto & Line)
	{
		int32_t X = std::stoul(Line[0]);
		int32_t Y = std::stoul(Line[1]);

		Width = std::max(Width, X + 1);
		Height = std::max(Height, Y + 1);

		return Area{ X, Y, 0, false };
	});

	Distances.reserve(Areas.size());

	for (int32_t Y = 0; Y < Height; ++Y)
	{
		for (int32_t X = 0; X < Width; ++X)
		{
			Distances.clear();

			for (Area & Start : Areas)
				Distances.push_back({ &Start, static_cast<uint32_t>(std::abs(X - Start.X) + std::abs(Y - Start.Y))});

			std::sort(std::begin(Distances), std::end(Distances), [](const auto & r, const auto & l) { return r.second < l.second; });

			// Same distance to multiple Starting Points, therefore tied
			if (Distances[0].second == Distances[1].second)
				continue;

			Area & Closest = *Distances[0].first;

			++Closest.Size;
			if ((!Closest.Infinite) && ((X == 0) || (X == (Width - 1)) || (Y == 0) || (Y == (Height - 1))))
				Closest.Infinite = true;
		}
	}

	std::sort(std::begin(Areas), std::end(Areas), [](const Area & r, const Area & l) { return r.Size > l.Size; });
	Area & Largest = *std::find_if(std::begin(Areas), std::end(Areas), [](const Area & k) { return !k.Infinite; });

	std::cout << " Part One: " << Largest.Size << std::endl;
}
