// Day23.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


enum OpCode { hlf, tpl, inc, jmp, jie, jio };

struct CPUState {
	size_t InstructionPointer;

	size_t a;
	size_t b;

	CPUState()
		:InstructionPointer(0), a(0), b(0)
	{}

	CPUState(size_t a, size_t b)
		:InstructionPointer(0), a(a), b(b)
	{}
};

typedef std::function<void(CPUState &)> InstructionFunction;
typedef std::vector<InstructionFunction> InstructionVector;
typedef std::map<std::string, OpCode> StringOpCodeMap;
typedef std::function<size_t&(CPUState &)> RegisterFetchFunction;
typedef std::map<std::string, RegisterFetchFunction> StringRegisterFetchFunctionMap;


static CPUState Run(const std::string & InputFile, CPUState CPU = CPUState());
static InstructionFunction ParseAssembly(const StringVector & Line);

static const StringOpCodeMap StringToOpCodeMap({ { "hlf", hlf },{ "tpl", tpl },{ "inc", inc },{ "jmp", jmp },{ "jie", jie },{ "jio", jio }, });
static const StringRegisterFetchFunctionMap StringToRegisterFetch({ {"a", [](CPUState & CPU)->size_t& { return CPU.a; }},{ "b", [](CPUState & CPU)->size_t& { return CPU.b; }} });

int main()
{
	CPUState PartOne = Run("Input.txt");
	CPUState PartTwo = Run("Input.txt", CPUState(1, 0));

	std::cout << "b (Part One): " << PartOne.b << std::endl;
	std::cout << "b (Part Two): " << PartTwo.b << std::endl;

	system("pause");

	return 0;
}


static CPUState Run(const std::string & InputFile, CPUState CPU)
{
	StringVectorVector InputParts = GetFileLineParts(InputFile);
	InstructionVector ParsedInstructions(InputParts.size(), nullptr);

	while (CPU.InstructionPointer < ParsedInstructions.size())
	{
		if (!ParsedInstructions[CPU.InstructionPointer])
		{
			ParsedInstructions[CPU.InstructionPointer] = ParseAssembly(InputParts[CPU.InstructionPointer]);
		}

		ParsedInstructions[CPU.InstructionPointer](CPU);
	}

	return CPU;
}

static InstructionFunction ParseAssembly(const StringVector & Line)
{
	const OpCode Instruction = StringToOpCodeMap.find(Line[0])->second;

	RegisterFetchFunction RegisterFetch = nullptr;;
	switch (Instruction)
	{
	case hlf:
	case tpl:
	case inc:
		RegisterFetch = StringToRegisterFetch.find(Line[1])->second;
		break;
	case jie:
	case jio:
		RegisterFetch = StringToRegisterFetch.find(Line[1].substr(0, Line[1].size() - 1))->second;
		break;
	}

	int64_t JumpDistance = 0;
	switch (Instruction)
	{
	case jmp:
		JumpDistance = std::atoi(Line[1].c_str());
		break;
	case jie:
	case jio:
		JumpDistance = std::atoi(Line[2].c_str());
		break;
	}

	switch (Instruction)
	{
	case hlf:
		return [RegisterFetch](CPUState & CPU) { RegisterFetch(CPU) /= 2; ++CPU.InstructionPointer; };
	case tpl:
		return [RegisterFetch](CPUState & CPU) { RegisterFetch(CPU) *= 3; ++CPU.InstructionPointer; };
	case inc:
		return [RegisterFetch](CPUState & CPU) { ++RegisterFetch(CPU); ++CPU.InstructionPointer; };
	case jmp:
		return [JumpDistance](CPUState & CPU) { CPU.InstructionPointer += JumpDistance; };
	case jie:
		return [RegisterFetch, JumpDistance](CPUState & CPU) { CPU.InstructionPointer += ((RegisterFetch(CPU) % 2) == 0) ? JumpDistance : 1; };
	case jio:
		return [RegisterFetch, JumpDistance](CPUState & CPU) { CPU.InstructionPointer += (RegisterFetch(CPU) == 1) ? JumpDistance : 1; };
	}

	return nullptr;
}
