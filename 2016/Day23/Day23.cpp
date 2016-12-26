// Day23.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

enum class OpCode { Inc, Dec, Copy, Jump, Toggle, Noop };

struct Registers;
struct Instruction;
typedef std::vector<Instruction> InstructionVector;
typedef std::function<signed&(Registers &)> FetchFunction;
typedef std::function<signed(Registers &)> FetchValueFunction;

struct Registers
{
	signed A = 0;
	signed B = 0;
	signed C = 0;
	signed D = 0;
	size_t IP = 0;

	Registers() = default;
	Registers(signed A, signed B, signed C, signed D, size_t IP)
		:A(A), B(B), C(C), D(D), IP(IP)
	{}

	void Print() const
	{
		std::cout.width(8);
		std::cout << IP << " | ";
		std::cout.width(8);
		std::cout << A;
		std::cout.width(8);
		std::cout << B;
		std::cout.width(8);
		std::cout << C;
		std::cout.width(8);
		std::cout << D;
	}
};

struct Instruction
{
	OpCode Code;
	FetchFunction RefFetch;
	FetchValueFunction Arg1;
	FetchValueFunction Arg2;
	
	Instruction()
		:Code(OpCode::Noop), RefFetch(nullptr), Arg1(nullptr), Arg2(nullptr)
	{}

	Instruction(OpCode Code, FetchFunction RefFetch)
		:Code(Code), RefFetch(RefFetch), Arg1(nullptr), Arg2(nullptr)
	{}

	Instruction(OpCode Code, FetchValueFunction Arg1)
		:Code(Code), RefFetch(nullptr), Arg1(Arg1), Arg2(nullptr)
	{}

	Instruction(OpCode Code, FetchFunction RefFetch, FetchValueFunction Arg1)
		:Code(Code), RefFetch(RefFetch), Arg1(Arg1), Arg2(nullptr)
	{}

	Instruction(OpCode Code, FetchValueFunction Arg1, FetchValueFunction Arg2)
		:Code(Code), RefFetch(nullptr), Arg1(Arg1), Arg2(Arg2)
	{}

	void operator() (Registers & State, InstructionVector & Instructions)
	{
		switch (Code)
		{
		case OpCode::Inc:
			if(RefFetch)
				RefFetch(State)++;
			break;
		case OpCode::Dec:
			if (RefFetch)
				RefFetch(State)--;
			break; 
		case OpCode::Copy:
			if (RefFetch)
				RefFetch(State) = Arg1(State);
			break;
		case OpCode::Jump:
			if (Arg1(State) != 0)
			{
				signed JumpDistance = RefFetch ? RefFetch(State) : Arg2(State);
				State.IP += JumpDistance;
				return;
			}
			break;
		case OpCode::Toggle:
			{
				size_t Instruction = State.IP + (RefFetch ? RefFetch(State) : Arg1(State));
				if (Instruction < Instructions.size())
				{
					Instructions[Instruction].Toggle();
				}
			}
			break;
		default:
			std::cout << "Unknown Op Code" << std::endl;
		case OpCode::Noop:
			break;
		};

		State.IP++;
	};

	void Toggle()
	{
		switch(Code)
		{
		case OpCode::Inc:
			Code = OpCode::Dec;
			break;
		case OpCode::Dec:
		case OpCode::Toggle:
			Code = OpCode::Inc;
			break;
		case OpCode::Copy:
			Code = OpCode::Jump;
			break;
		case OpCode::Jump:
			Code = OpCode::Copy;
			break;
		}
	}

	void Print() const
	{
		std::cout << " | ";

		switch (Code)
		{
		case OpCode::Inc:
			std::cout << "Inc";
			break;
		case OpCode::Dec:
			std::cout << "Dec";
			break;
		case OpCode::Copy:
			std::cout << "Copy";
			break;
		case OpCode::Jump:
			std::cout << "Jump";
			break;
		case OpCode::Toggle:
			std::cout << "Toogle";
		break;
		case OpCode::Noop:
			std::cout << "Noop";
			break;
		default:
			std::cout << "Unknown Op Code";
			break;
		};
	}
};

Instruction ParseInc(const StringVector & Line);
Instruction ParseDec(const StringVector & Line);
Instruction ParseCpy(const StringVector & Line);
Instruction ParseJnz(const StringVector & Line);
Instruction ParseTgl(const StringVector & Line);

static const std::map<std::string, std::function<Instruction(const StringVector &)>> AssemblyParserMap = {
	{ "inc", &ParseInc },
	{ "dec", &ParseDec },
	{ "cpy", &ParseCpy },
	{ "jnz", &ParseJnz },
	{ "tgl", &ParseTgl }
};

static const std::map<std::string, FetchFunction> RegisterFetchMap = {
	{ "a", [](Registers & State)->signed& { return State.A; } },
	{ "b", [](Registers & State)->signed& { return State.B; } },
	{ "c", [](Registers & State)->signed& { return State.C; } },
	{ "d", [](Registers & State)->signed& { return State.D; } },
};


InstructionVector ParseAssembly(const StringVectorVector & Lines);
Registers Run(InstructionVector & Instructions, Registers InitialState = Registers());

int main()
{
	StringVectorVector Lines = GetFileLineParts("Input.txt");
	InstructionVector Instructions = ParseAssembly(Lines);

	Registers PartOne = Run(Instructions, { 7, 0, 0, 0, 0 });

	std::cout << "A: " << PartOne.A << std::endl;

	system("pause");

	return 0;
}

InstructionVector ParseAssembly(const StringVectorVector & Lines)
{
	InstructionVector Instructions;
	Instructions.reserve(Lines.size());

	for(const StringVector & Line : Lines)
	{
		auto Parser = AssemblyParserMap.find(Line[0]);
		if (Parser == AssemblyParserMap.end())
		{
			std::cout << "Unknwon Assembly: " << Line[0] << std::endl;
			__debugbreak();
			Instructions.emplace_back();
		}
		else
		{
			Instructions.push_back(Parser->second(Line));
		}
	}

	return Instructions;
}

void PrintDebug(const Registers & State, const Instruction & Instruction)
{
	State.Print();
	Instruction.Print();
	std::cout << std::endl;
	getchar();
}

Registers Run(InstructionVector & Instructions, Registers InitialState)
{
	Registers & State = InitialState;

	while (State.IP < Instructions.size())
	{
		//PrintDebug(State, Instructions[State.IP]);

		Instructions[State.IP](State, Instructions);
	}

	return State;
}

FetchFunction GetFetchFunction(const std::string & Identifier)
{
	auto RegisterFetch = RegisterFetchMap.find(Identifier);
	if (RegisterFetch == RegisterFetchMap.end())
	{
		return nullptr;
	}
	else
	{
		return RegisterFetch->second;
	}
}

FetchValueFunction GetFetchValueFunction(const std::string & Identifier)
{
	auto RegisterFetchFunction = GetFetchFunction(Identifier);
	if (RegisterFetchFunction == nullptr)
	{
		signed Value = std::stoi(Identifier);
		return [Value](Registers & State) { return Value; };
	}
	else
	{
		return [RegisterFetchFunction](Registers & State) { return static_cast<signed>(RegisterFetchFunction(State)); };
	}
}

Instruction ParseInc(const StringVector & Line)
{
	return Instruction(OpCode::Inc, GetFetchFunction(Line[1]));
}

Instruction ParseDec(const StringVector & Line)
{
	return Instruction(OpCode::Dec, GetFetchFunction(Line[1]));
}

Instruction ParseCpy(const StringVector & Line)
{
	// Get First Argument as Value
	FetchValueFunction FetchValue = GetFetchValueFunction(Line[1]);
	FetchFunction RegisterFetch = GetFetchFunction(Line[2]);

	return Instruction(OpCode::Copy, RegisterFetch, FetchValue);
}

Instruction ParseJnz(const StringVector & Line)
{
	// Get Second Argument as Value
	FetchValueFunction FetchTestValue = GetFetchValueFunction(Line[1]);
	FetchFunction FetchJumpValue = GetFetchFunction(Line[2]);

	if (FetchJumpValue)
	{
		return Instruction(OpCode::Jump, FetchJumpValue, FetchTestValue);
	}
	else
	{
		return Instruction(OpCode::Jump, FetchTestValue, GetFetchValueFunction(Line[2]));
	}
}

Instruction ParseTgl(const StringVector & Line)
{
	FetchFunction FetchValue = GetFetchFunction(Line[1]);

	if (FetchValue)
	{
		return Instruction(OpCode::Toggle, FetchValue);
	}
	else
	{
		return Instruction(OpCode::Toggle, GetFetchValueFunction(Line[1]));
	}
}