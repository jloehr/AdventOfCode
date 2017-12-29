// Day23.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

constexpr size_t RegisterCount = 8;

struct State;

using RegisterType = int64_t;
using Instruction = std::function<bool(State &)>;
using Registers = std::array<RegisterType, RegisterCount>;

struct State
{
	size_t InstructionPointer;
	Registers Registers;
	size_t MulCount;
};

std::function<RegisterType & (State &)> FetchRegisterReference(const std::string & Name)
{
	const char RegisterID = Name[0] - 'a';
	return [RegisterID](State & State)->RegisterType& { return State.Registers[RegisterID]; };
}

std::function<const RegisterType(State &)> FetchValue(const std::string & String)
{
	if (String[0] >= 'a')
	{
		const char RegisterID = String[0] - 'a';
		return[RegisterID](State & State)->const RegisterType{ return State.Registers[RegisterID]; };
	}
	else
	{
		const int Value = std::stoi(String);
		return[Value](State &)->const RegisterType{ return Value; };
	}
}

const std::unordered_map<std::string, std::function<Instruction(const StringVector &)>> CompilerMap{
	{ "set", [](const StringVector & Line)->Instruction
{
	auto Arg1 = FetchRegisterReference(Line[1]);
	auto Arg2 = FetchValue(Line[2]);
	return [=](State & State)->bool { Arg1(State) = Arg2(State); State.InstructionPointer++; return true; };
}
	},
	{ "sub", [](const StringVector & Line)->Instruction
{
	auto Arg1 = FetchRegisterReference(Line[1]);
	auto Arg2 = FetchValue(Line[2]);
	return [=](State & State)->bool { Arg1(State) -= Arg2(State); State.InstructionPointer++; return true; };
}
	},
	{ "mul", [](const StringVector & Line)->Instruction
{
	auto Arg1 = FetchRegisterReference(Line[1]);
	auto Arg2 = FetchValue(Line[2]);
	return [=](State & State)->bool { Arg1(State) *= Arg2(State); State.MulCount++; State.InstructionPointer++; return true; };
}
	},
	{ "jnz", [](const StringVector & Line)->Instruction
{
	auto Arg1 = FetchValue(Line[1]);
	auto Arg2 = FetchValue(Line[2]);
	return [=](State & State)->bool {
		State.InstructionPointer += (Arg1(State) != 0) ? Arg2(State) : 1;
		return true;
	};
}
	},
};

void PartOne(const std::vector<Instruction> & Instructions)
{
	State PartOne{ 0 };

	while (PartOne.InstructionPointer < Instructions.size())
	{
		Instructions[PartOne.InstructionPointer](PartOne);
	}

	std::cout << "Mul Count: " << PartOne.MulCount << std::endl;
}


int main()
{
	const StringVectorVector Lines = GetFileLineParts("Input.txt");

	std::vector<Instruction> Instructions;

	for (const auto & Line : Lines)
	{
		auto InstructionCompiler = CompilerMap.find(Line[0]);
		if (InstructionCompiler == CompilerMap.end())
		{
			std::cout << "Panic Alert: Unknown Opcode " << Line[0] << std::endl;
		}
		else
		{
			Instructions.emplace_back(InstructionCompiler->second(Line));
		}
	}

	PartOne(Instructions);

	system("pause");
    return 0;
}

