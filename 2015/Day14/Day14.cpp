// Day14.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

struct Reindeer {
	uint64_t Velocity;
	uint64_t FlyingTime;
	uint64_t RestingTime;

	uint64_t TimeFlyingLeft;
	uint64_t TimeRestingLeft;

	uint64_t Distance;
	uint64_t Points;

	Reindeer(uint64_t Velocity, uint64_t FlyingTime, uint64_t RestingTime)
		:Velocity(Velocity), FlyingTime(FlyingTime), RestingTime(RestingTime), TimeFlyingLeft(FlyingTime), TimeRestingLeft(RestingTime), Points(0)
	{}

	void Tick()
	{
		if (TimeFlyingLeft > 0)
		{
			Distance += Velocity;
			--TimeFlyingLeft;
		}
		else
		{
			--TimeRestingLeft;
			if (TimeRestingLeft == 0)
			{
				TimeFlyingLeft = FlyingTime;
				TimeRestingLeft = RestingTime;
			}
		}
	}
};

typedef std::vector<Reindeer> ReindeerVector;

static void RaceForPoints(ReindeerVector & Reindeers);

static const uint64_t Seconds = 2503;

int main()
{
	std::ifstream Input("Input.txt");

	if (!Input.is_open())
	{
		std::cout << "Error opening File!" << std::endl;
		return 0;
	}

	std::string Line;
	uint64_t WinningDistance = 0;
	ReindeerVector Reindeers;

	while (std::getline(Input, Line))
	{
		std::stringstream LineStream(Line);
		std::string Buffer;
		std::vector<std::string> LineParts;

		while (std::getline(LineStream, Buffer, ' '))
		{
			LineParts.push_back(Buffer);
		}

		uint64_t Velocity = std::atoi(LineParts[3].c_str());
		uint64_t FlyingTime = std::atoi(LineParts[6].c_str());
		uint64_t RestingTime = std::atoi(LineParts[13].c_str());

		Reindeers.push_back(Reindeer(Velocity, FlyingTime, RestingTime));

		uint64_t FullCycleTime = FlyingTime + RestingTime;
		uint64_t FullCycleDistance = FlyingTime * Velocity;

		uint64_t FullCycles = Seconds / FullCycleTime;
		uint64_t ReindeerDistance = FullCycles * FullCycleDistance;

		uint64_t SecondsLeft = Seconds % FullCycleTime;
		uint64_t FlyingSeconds = (SecondsLeft > FlyingTime) ? FlyingTime : SecondsLeft;
		ReindeerDistance += FlyingSeconds * Velocity;

		WinningDistance = (ReindeerDistance > WinningDistance) ? ReindeerDistance : WinningDistance;
	}

	Input.close();

	std::cout << "Winning Distance: " << WinningDistance << std::endl;

	RaceForPoints(Reindeers);

	std::cout << "Winning Points: " << Reindeers[0].Points << std::endl;

	system("pause");

    return 0;
}


static void RaceForPoints(ReindeerVector & Reindeers)
{
	for (size_t i = 0; i < Seconds; i++)
	{
		for (auto & Reindeer : Reindeers)
		{
			Reindeer.Tick();
		}

		std::sort<ReindeerVector::iterator>(Reindeers.begin(), Reindeers.end(), [](Reindeer & A, Reindeer & B)->bool { return A.Distance > B.Distance; });

		size_t CurrentBestDistance = Reindeers[0].Distance;

		for (auto & Reindeer : Reindeers)
		{
			if (Reindeer.Distance == CurrentBestDistance)
			{
				++Reindeer.Points;
			}
			else
			{
				break;
			}
		}
	}

	std::sort<ReindeerVector::iterator>(Reindeers.begin(), Reindeers.end(), [](Reindeer & A, Reindeer & B)->bool { return A.Points > B.Points; });
}