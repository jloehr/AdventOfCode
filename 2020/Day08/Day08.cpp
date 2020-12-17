// Day08.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <functional>
#include <unordered_set>

#include "../../Common/Common.h"

typedef std::function <ptrdiff_t(uint64_t&)> Instruction;
typedef std::vector<Instruction> InstructionVector;

int main()
{
	const StringVectorVector LineParts = GetFileLineParts("Input.txt");
	InstructionVector Instructions;
	Instructions.reserve(LineParts.size());

	for (const auto& Line : LineParts)
	{
		uint64_t Argument = std::stoll(Line[1]);
		switch (Line[0][0])
		{
		case 'a':
			Instructions.push_back([=](uint64_t& Accumulator)->ptrdiff_t { Accumulator += Argument; return 1; });
			break;
		case 'n':
			Instructions.push_back([=](uint64_t& Accumulator)->ptrdiff_t { return 1; });
			break;
		case 'j':
			Instructions.push_back([=](uint64_t& Accumulator)->ptrdiff_t { return Argument; });
			break;
		}
	}

	std::unordered_set<size_t> VisitedInstructions;
	size_t InstructionPointer = 0;
	uint64_t Accumulator = 0;

	while (VisitedInstructions.insert(InstructionPointer).second)
	{
		InstructionPointer += Instructions[InstructionPointer](Accumulator);
	}

	std::cout << "Part One: " << Accumulator << std::endl;

}
