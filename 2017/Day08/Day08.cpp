// Day08.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

const static std::map<std::string, std::function<void(int64_t &, const int64_t)>> Instructions = {
	{ "inc", [](int64_t & Register, int64_t Value) { Register += Value; } },
	{ "dec", [](int64_t & Register, int64_t Value) { 	Register -= Value; } },
};

const static std::map<std::string, std::function<bool(const int64_t, const int64_t)>> Conditions = {
	{ "<", [](const int64_t Left, const int64_t Right) -> bool { return Left < Right; } },
	{ ">", [](const int64_t Left, const int64_t Right) -> bool { return Left > Right; } },
	{ "<=", [](const int64_t Left, const int64_t Right) -> bool { return Left <= Right; } },
	{ ">=", [](const int64_t Left, const int64_t Right) -> bool { return Left >= Right; } },
	{ "==", [](const int64_t Left, const int64_t Right) -> bool { return Left == Right; } },
	{ "!=", [](const int64_t Left, const int64_t Right) -> bool { return Left != Right; } },
};

int main()
{
	const auto ValueCompare = [](const auto & Left, const auto & Right) { return Left.second < Right.second; };
	const StringVector Lines = GetFileLines("Input.txt");
	std::map<std::string, int64_t> Registers;
	int64_t MaxValue = INT64_MIN;

	for (const auto & Line : Lines)
	{
		std::istringstream LineStream(Line);
		std::string Register;
		std::string OpCode;
		int64_t Value;
		std::string IfKeyword;
		std::string ConditionRegister;
		std::string ConditionOpCode;
		int64_t ConditionValue;

		LineStream >> Register >> OpCode >> Value;
		if (LineStream)
			LineStream >> IfKeyword >> ConditionRegister >> ConditionOpCode >> ConditionValue;

		if (IfKeyword.empty() || Conditions.find(ConditionOpCode)->second(Registers[ConditionRegister], ConditionValue))
		{
			Instructions.find(OpCode)->second(Registers[Register], Value);
		}

		MaxValue = std::max(MaxValue, std::max_element(Registers.begin(), Registers.end(), ValueCompare)->second);
	}
	
	auto MaxElement = std::max_element(Registers.begin(), Registers.end(), ValueCompare);
	std::cout << "Max Register " << MaxElement->first << ": " << MaxElement->second << std::endl;
	std::cout << "Max Value: " << MaxValue << std::endl;

	system("pause");
    return 0;
}

