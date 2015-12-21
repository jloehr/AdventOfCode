// Day7.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Wire.h"

static void CreateWire(std::string & Line, WireNameMap & Wires);

int main()
{
	std::ifstream Input("Input.txt");

	if (!Input.is_open())
	{
		std::cout << "Error opening File!" << std::endl;
		return 0;
	}

	std::string Line;

	WireNameMap Wires;

	while (std::getline(Input, Line))
	{
		CreateWire(Line, Wires);
	}

	Input.close();

	uint32_t WireAValue = Wires["a"]->GetValue();
	std::cout << "Part One - Wire a: " << WireAValue << std::endl;

	Wires["b"]->ChangeValue(WireAValue);

	WireAValue = Wires["a"]->GetValue();
	std::cout << "Part Two - Wire a: " << WireAValue << std::endl;

	system("pause");

    return 0;
}

static void CreateWire(std::string & Line, WireNameMap & Wires)
{
	const static std::string ArrowDelimeter(" -> ");
	size_t ArrowDelimeterPosition = Line.find(ArrowDelimeter);

	std::stringstream InputStringStream(Line.substr(0, ArrowDelimeterPosition));
	std::string OutputWire = Line.substr(ArrowDelimeterPosition + ArrowDelimeter.size());

	std::vector<std::string> InputParts;

	while (InputStringStream)
	{
		std::string Buffer;
		std::getline(InputStringStream, Buffer, ' ');

		if (!Buffer.empty())
		{
			InputParts.push_back(Buffer);
		}
	}

	if (!Wires[OutputWire])
	{
		Wires[OutputWire].reset(new Wire(OutputWire));
	}

	Wires[OutputWire]->Setup(InputParts, Wires);
}
