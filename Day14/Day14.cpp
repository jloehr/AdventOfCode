// Day14.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

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

	system("pause");

    return 0;
}

