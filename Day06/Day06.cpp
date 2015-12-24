// Day6.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

enum LightCommand { TurnOn, TurnOff, Toggle };
typedef std::pair<uint16_t, uint16_t> Coordinate;

struct Instruction {
	LightCommand Commnad;
	Coordinate TopLeft;
	Coordinate BottomRight;
};

static Instruction ParseLine(std::stringstream & Line);

int main()
{
	std::ifstream Input("Input.txt");

	if (!Input.is_open())
	{
		std::cout << "Error opening File!" << std::endl;
		return 0;
	}

	std::string Line;

	std::array<std::array<bool, 1000>, 1000> Lights = {};
	std::array<std::array<uint32_t, 1000>, 1000> LightBrightness = { 0 };

	uint64_t LightsOn = 0;
	uint64_t TotalBrightness = 0;

	while (std::getline(Input, Line))
	{
		std::stringstream LineStream(Line);

		Instruction CurrentInstruction = ParseLine(LineStream);
		std::function<void (uint32_t, uint32_t)> LightMethod;


		switch (CurrentInstruction.Commnad)
		{
		case Toggle:
			LightMethod = [&](uint32_t x, uint32_t y) {
				Lights[x][y] = !Lights[x][y];
				LightsOn += (Lights[x][y]) ? 1 : -1;

				LightBrightness[x][y] += 2;
				TotalBrightness += 2;

			};
			break;
		case TurnOff:
			LightMethod = [&](uint32_t x, uint32_t y) {
				if (Lights[x][y])
				{
					Lights[x][y] = false;
					LightsOn--;
				}

				if (LightBrightness[x][y] > 0)
				{
					LightBrightness[x][y]--;
					TotalBrightness--;
				}
			};
			break;
		case TurnOn:
			LightMethod = [&](uint32_t x, uint32_t y) {
				if (!Lights[x][y])
				{
					Lights[x][y] = true;
					LightsOn++;
				}

				LightBrightness[x][y]++;
				TotalBrightness++;
			};
			break;
		}

		for (uint32_t x = CurrentInstruction.TopLeft.first; x <= CurrentInstruction.BottomRight.first; x++)
			for (uint32_t y = CurrentInstruction.TopLeft.second; y <= CurrentInstruction.BottomRight.second; y++)
			{
				LightMethod(x, y);
			}
	}

	Input.close();

	std::cout << "Lit Lights: " << LightsOn << std::endl;
	std::cout << "Total Brightness: " << TotalBrightness << std::endl;

	system("pause");

	return 0;
}

static Instruction ParseLine(std::stringstream & Line)
{
	Instruction NewInstruction;

	std::string CommandPart;
	std::getline(Line, CommandPart, ' ');

	if (CommandPart == "toggle")
	{
		NewInstruction.Commnad = Toggle;
	}
	else
	{
		std::getline(Line, CommandPart, ' ');
		NewInstruction.Commnad = (CommandPart == "on") ? TurnOn : TurnOff;
	}

	Line >> NewInstruction.TopLeft.first;
	Line.get();
	Line >> NewInstruction.TopLeft.second;
	Line.get();

	std::string ThroughString;
	std::getline(Line, ThroughString, ' ');


	Line >> NewInstruction.BottomRight.first;
	Line.get();
	Line >> NewInstruction.BottomRight.second;

	return NewInstruction;
}