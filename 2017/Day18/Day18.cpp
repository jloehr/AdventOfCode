// Day18.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

constexpr size_t RegisterCount = 'z' - 'a';

struct State;

using RegisterType = int64_t;
using Instruction = std::function<bool(State &)>;
using Registers = std::array<RegisterType, RegisterCount>;
using Queue = std::queue<RegisterType>;

struct State
{
	size_t InstructionPointer;
	Registers Registers;
	bool ReceiveFlag;
	RegisterType LastSendValue;
	size_t SendCounter;
	Queue SendBuffer;
	Queue * ReceiveBuffer;
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
		return [RegisterID](State & State)->const RegisterType { return State.Registers[RegisterID]; };
	}
	else
	{
		const int Value = std::stoi(String);
		return [Value](State &)->const RegisterType { return Value; };
	}
}

const std::unordered_map<std::string, std::function<Instruction(const StringVector &)>> CompilerMap{
	{"set", [](const StringVector & Line)->Instruction
		{
			auto Arg1 = FetchRegisterReference(Line[1]);
			auto Arg2 = FetchValue(Line[2]);
			return [=](State & State)->bool { Arg1(State) = Arg2(State); State.InstructionPointer++; return true; };
		}
	},
	{"add", [](const StringVector & Line)->Instruction
		{
			auto Arg1 = FetchRegisterReference(Line[1]);
			auto Arg2 = FetchValue(Line[2]);
			return [=](State & State)->bool { Arg1(State) += Arg2(State); State.InstructionPointer++; return true; };
		}
	},
	{ "mul", [](const StringVector & Line)->Instruction
		{
			auto Arg1 = FetchRegisterReference(Line[1]);
			auto Arg2 = FetchValue(Line[2]);
			return [=](State & State)->bool { Arg1(State) *= Arg2(State); State.InstructionPointer++; return true; };
		}
	},
	{ "mod", [](const StringVector & Line)->Instruction
		{
			auto Arg1 = FetchRegisterReference(Line[1]);
			auto Arg2 = FetchValue(Line[2]);
			return [=](State & State)->bool { Arg1(State) %= Arg2(State); State.InstructionPointer++; return true; };
		}
	},
	{ "snd", [](const StringVector & Line)->Instruction
		{
			auto Arg1 = FetchRegisterReference(Line[1]);
			return [=](State & State)->bool {
				State.LastSendValue = Arg1(State);
				State.SendBuffer.push(State.LastSendValue);
				State.SendCounter++;
				State.InstructionPointer++;
				return true;
			};
		}
	},
	{ "rcv", [](const StringVector & Line)->Instruction
		{
			auto Arg1 = FetchRegisterReference(Line[1]);
			return [=](State & State)->bool {
				RegisterType & Register = Arg1(State);

				if (State.ReceiveBuffer == nullptr)
				{
					if (Register != 0)
					{
						Register = State.LastSendValue;
						State.ReceiveFlag = true;
					}
				}
				else
				{
					if (State.ReceiveBuffer->size() <= 0)
						return false;

					Register = State.ReceiveBuffer->front();
					State.ReceiveBuffer->pop();
				}

				State.InstructionPointer++; 
				return true;
			};
		}
	},
	{ "jgz", [](const StringVector & Line)->Instruction
		{
			auto Arg1 = FetchValue(Line[1]);
			auto Arg2 = FetchValue(Line[2]);
			return [=](State & State)->bool {
				State.InstructionPointer += (Arg1(State) > 0) ? Arg2(State) : 1;
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
		if (PartOne.ReceiveFlag)
		{
			std::cout << "Last Frequency: " << PartOne.LastSendValue << std::endl;
			break;
		}
	}
}

void PartTwo(const std::vector<Instruction> & Instructions)
{
	State ThreadZero{ 0 };
	State ThreadOne{ 0 };
	ThreadZero.ReceiveBuffer = &ThreadOne.SendBuffer;
	ThreadOne.ReceiveBuffer = &ThreadZero.SendBuffer;

	ThreadZero.Registers['p' - 'a'] = 0;
	ThreadOne.Registers['p' - 'a'] = 1;

	while ((ThreadZero.InstructionPointer < Instructions.size()) && (ThreadOne.InstructionPointer < Instructions.size()))
	{
		bool ThradZeroAdvanced = Instructions[ThreadZero.InstructionPointer](ThreadZero);
		bool ThradOneAdvanced = Instructions[ThreadOne.InstructionPointer](ThreadOne);

		if (!(ThradZeroAdvanced || ThradOneAdvanced))
			break;
	}

	std::cout << "Thread 0: " << ThreadZero.SendCounter << std::endl;
	std::cout << "Thread 1: " << ThreadOne.SendCounter << std::endl;
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
	PartTwo(Instructions);

	system("pause");
    return 0;
}

