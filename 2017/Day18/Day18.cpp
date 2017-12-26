// Day18.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

constexpr size_t RegisterCount = 'z' - 'a';

struct State;

using RegisterType = int64_t;
using Instruction = std::function<size_t(State &)>;
using Registers = std::array<RegisterType, RegisterCount>;

struct State
{
	Registers Registers;
	int Frequency;
	bool RecoveryFlag;
};


RegisterType & FetchRegister(const std::string & Name, Registers & Regs);
std::function<const RegisterType()> FetchSecondArg(const std::string & String, Registers & Regs);

const std::unordered_map<std::string, std::function<Instruction(const StringVector &, Registers &)>> CompilerMap{
	{"set", [](const StringVector & Line, Registers & Regs)->Instruction
		{
			RegisterType & Arg1 = FetchRegister(Line[1], Regs);
			auto Arg2 = FetchSecondArg(Line[2], Regs);
			return [Arg2, &Arg1](State &)->size_t { Arg1 = Arg2(); return 1; };
		}
	},
	{"add", [](const StringVector & Line, Registers & Regs)->Instruction
		{
			RegisterType & Arg1 = FetchRegister(Line[1], Regs);
			auto Arg2 = FetchSecondArg(Line[2], Regs);
			return [Arg2, &Arg1](State &)->size_t { Arg1 += Arg2(); return 1; };
		}
	},
	{ "mul", [](const StringVector & Line, Registers & Regs)->Instruction
		{
			RegisterType & Arg1 = FetchRegister(Line[1], Regs);
			auto Arg2 = FetchSecondArg(Line[2], Regs);
			return [Arg2, &Arg1](State &)->size_t { Arg1 *= Arg2(); return 1; };
		}
	},
	{ "mod", [](const StringVector & Line, Registers & Regs)->Instruction
		{
			RegisterType & Arg1 = FetchRegister(Line[1], Regs);
			auto Arg2 = FetchSecondArg(Line[2], Regs);
			return [Arg2, &Arg1](State &)->size_t { Arg1 %= Arg2(); return 1; };
		}
	},
	{ "snd", [](const StringVector & Line, Registers & Regs)->Instruction
		{
			RegisterType & Arg1 = FetchRegister(Line[1], Regs);
			return [&Arg1](State & State)->size_t { State.Frequency = Arg1; return 1; };
		}
	},
	{ "rcv", [](const StringVector & Line, Registers & Regs)->Instruction
		{
			RegisterType & Arg1 = FetchRegister(Line[1], Regs);
			return [&Arg1](State & State)->size_t {
				if (Arg1 != 0)
				{
					Arg1 = State.Frequency;
					State.RecoveryFlag = true;
				}
				return 1; 
			};
		}
	},
	{ "jgz", [](const StringVector & Line, Registers & Regs)->Instruction
		{
			auto Arg1 = FetchSecondArg(Line[1], Regs);
			auto Arg2 = FetchSecondArg(Line[2], Regs);
			return [Arg1, Arg2](State &)->size_t {
				if (Arg1() > 0)
					return Arg2();
				return 1;
			};
		}
	},
};

RegisterType & FetchRegister(const std::string & Name, Registers & Regs)
{
	return Regs[Name[0] - 'a'];
}

std::function<const RegisterType()> FetchSecondArg(const std::string & String, Registers & Regs)
{
	if (String[0] >= 'a')
	{
		const char Register = String[0] - 'a';
		return [Register, &Regs]()->const int { return Regs[Register]; };
	}
	else
	{
		const int Value = std::stoi(String);
		return [Value]()->const int { return Value; };
	}
}

int main()
{
	const StringVectorVector Lines = GetFileLineParts("Input.txt");
	
	State State{ 0 };
	size_t InstructionPointer = 0;
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
			Instructions.emplace_back(InstructionCompiler->second(Line, State.Registers));
		}
	}

	while (InstructionPointer < Instructions.size())
	{
		InstructionPointer += Instructions[InstructionPointer](State);
		if (State.RecoveryFlag)
		{
			std::cout << "Last Frequency: " << State.Frequency << std::endl;
			break;
		}
	}

	system("pause");
    return 0;
}

