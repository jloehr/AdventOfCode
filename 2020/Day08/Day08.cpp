// Day08.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <functional>
#include <unordered_set>

#include "../../Common/Common.h"

enum class InstructionType { Acc = 'a', Nop = 'n', Jmp = 'j' };
typedef std::pair<InstructionType, uint64_t> Instruction;
typedef std::vector<Instruction> InstructionVector;

std::pair<bool, uint64_t> Run(const InstructionVector& Instructions);

int main()
{
	const StringVectorVector LineParts = GetFileLineParts("Input.txt");
	InstructionVector Instructions;
	Instructions.reserve(LineParts.size());

	for (const auto& Line : LineParts)
	{
		Instructions.emplace_back(static_cast<InstructionType>(Line[0][0]), std::stoll(Line[1]));
	}

	std::cout << "Part One: " << Run(Instructions).second << std::endl;

	for (Instruction& Instruction : Instructions)
	{
		if (Instruction.first == InstructionType::Acc) continue;

		Instruction.first = (Instruction.first == InstructionType::Jmp) ? InstructionType::Nop : InstructionType::Jmp;
		auto Result =  Run(Instructions);
		if (Result.first)
		{
			std::cout << "Part Two: " << Result.second << std::endl;
			break;
		}
		else
		{
			Instruction.first = (Instruction.first == InstructionType::Jmp) ? InstructionType::Nop : InstructionType::Jmp;
		}
	}
}

std::pair<bool, uint64_t> Run(const InstructionVector& Instructions)
{
	std::unordered_set<size_t> VisitedInstructions;
	size_t InstructionPointer = 0;
	uint64_t Accumulator = 0;

	while (VisitedInstructions.insert(InstructionPointer).second)
	{
		if (InstructionPointer >= Instructions.size())
			return std::make_pair(true, Accumulator);

		const Instruction& Instruction = Instructions[InstructionPointer];
		switch (Instruction.first)
		{
		case InstructionType::Acc:
			Accumulator += Instruction.second;
		case InstructionType::Nop:
			InstructionPointer++;
			break;
		case InstructionType::Jmp:
			InstructionPointer += Instruction.second;
			break;
		}
	}

	return std::make_pair(false, Accumulator);
}
