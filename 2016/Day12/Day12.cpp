// Day12.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

struct Registers;
typedef std::function<void(Registers &)> Instruction;
typedef std::vector<Instruction> InstructionVector;

struct Registers
{
	size_t A = 0;
	size_t B = 0;
	size_t C = 0;
	size_t D = 0;
	size_t IP = 0;

	Registers() = default;
	Registers(size_t A, size_t B, size_t C, size_t D, size_t IP)
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

Instruction ParseInc(const StringVector & Line);
Instruction ParseDec(const StringVector & Line);
Instruction ParseCpy(const StringVector & Line);
Instruction ParseJnz(const StringVector & Line);

static const std::map<std::string, std::function<Instruction(const StringVector &)>> AssemblyParserMap = {
	{ "inc", &ParseInc },
	{ "dec", &ParseDec },
	{ "cpy", &ParseCpy },
	{ "jnz", &ParseJnz }
};


typedef std::function<size_t&(Registers &)> FetchFunction;
typedef std::function<signed(Registers &)> FetchValueFunction;
static const std::map<std::string, FetchFunction> RegisterFetchMap = {
	{ "a", [](Registers & State)->size_t& { return State.A; } },
	{ "b", [](Registers & State)->size_t& { return State.B; } },
	{ "c", [](Registers & State)->size_t& { return State.C; } },
	{ "d", [](Registers & State)->size_t& { return State.D; } },
};

static const Instruction NOOP = [](Registers & State) { /* NOOP */};

Registers Run(const StringVectorVector & Lines, InstructionVector & Instructions, Registers InitialState = Registers());
Instruction ParseAssembly(const StringVector & Line);

int main()
{
	StringVectorVector Lines = GetFileLineParts("Input.txt");
	InstructionVector Instructions;

	Registers PartOne = Run(Lines, Instructions);
	Registers PartTwo = Run(Lines, Instructions, { 0, 0, 1, 0, 0 });

	std::cout << "PartOne: " << PartOne.A << std::endl;
	std::cout << "PartTwo: " << PartTwo.A << std::endl;
	
	system("pause");

    return 0;
}

void PrintDebug(const StringVectorVector & Lines, const Registers & State)
{
	State.Print();
	std::cout << " | ";
	for (const std::string & LinePart : Lines[State.IP])
	{
		std::cout << LinePart << " ";
	}

	std::cout << std::endl;
	getchar();
}

Registers Run(const StringVectorVector & Lines, InstructionVector & Instructions, Registers InitialState) 
{
	if (Instructions.size() < Lines.size())
	{
		Instructions.resize(Lines.size(), nullptr);
	}

	Registers & State = InitialState;

	while (State.IP < Instructions.size())
	{
		if (!Instructions[State.IP])
		{
			Instructions[State.IP] = ParseAssembly(Lines[State.IP]);
		}

		//PrintDebug(Lines, State);

		Instructions[State.IP](State); 
		State.IP++;
	}

	return State;
}

Instruction ParseAssembly(const StringVector & Line)
{
	auto Parser = AssemblyParserMap.find(Line[0]);
	if (Parser == AssemblyParserMap.end())
	{
		std::cout << "Unknwon Assembly: " << Line[0] << std::endl;
		__debugbreak();
		return NOOP;
	}
	else
	{
		return Parser->second(Line);
	}
}

Instruction MakeFetchAndExecuteInstruction(const std::string RegisterIdentifier, std::function<void(size_t&, Registers &)> Execute)
{
	auto RegisterFetch = RegisterFetchMap.find(RegisterIdentifier);
	if (RegisterFetch != RegisterFetchMap.end())
	{
		FetchFunction RegisterFetchFunction = RegisterFetch->second;

		return [=](Registers & State) { Execute(RegisterFetchFunction(State), State); };
	}
	else
	{
		std::cout << "Unknown Register: " << RegisterIdentifier << std::endl;
		return NOOP;
	}
}

FetchValueFunction GetFetchValueFunction(const std::string & Identifier)
{
	auto RegisterFetch = RegisterFetchMap.find(Identifier);
	if (RegisterFetch == RegisterFetchMap.end())
	{
		signed Value = std::stoi(Identifier);
		return [Value](Registers & State) { return Value; };
	}
	else
	{
		FetchFunction RegisterFetchFunction = RegisterFetch->second;
		return [RegisterFetchFunction](Registers & State) { return static_cast<signed>(RegisterFetchFunction(State)); };
	}
}

Instruction ParseInc(const StringVector & Line)
{
	return MakeFetchAndExecuteInstruction(Line[1], [](size_t & Reference, Registers & State) { Reference++; });
}

Instruction ParseDec(const StringVector & Line)
{
	return MakeFetchAndExecuteInstruction(Line[1], [](size_t & Reference, Registers & State) { Reference--; });
}

Instruction ParseCpy(const StringVector & Line)
{
	// Get First Argument as Value
	FetchValueFunction FetchValue = GetFetchValueFunction(Line[1]);

	return MakeFetchAndExecuteInstruction(Line[2], [FetchValue](size_t & Reference, Registers & State) { Reference = FetchValue(State); });
}

Instruction ParseJnz(const StringVector & Line)
{
	// Get Second Argument as Value
	FetchValueFunction FetchTestValue = GetFetchValueFunction(Line[1]);
	FetchValueFunction FetchJumpValue = GetFetchValueFunction(Line[2]);

	return [=](Registers & State) { State.IP += (FetchTestValue(State) != 0) ? FetchJumpValue(State) - 1 : 0; };
}
