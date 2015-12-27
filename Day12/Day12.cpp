// Day12.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

const static std::string NumericCharacters("-0123456789");
const static std::string Brackets("{[]}");
static const std::string Red("\"red\"");

static int64_t GetValue();
static int64_t GetNonRedValue();
static int64_t GetValueInRange(size_t Position, size_t End);

static size_t GetClosingBracketPosition(size_t Start);

static std::string InputString;

int main()
{
	std::ifstream Input("Input.txt");

	if (!Input.is_open())
	{
		std::cout << "Error opening File!" << std::endl;
		return 0;
	}

	InputString.assign((std::istreambuf_iterator<char>(Input)),
		(std::istreambuf_iterator<char>()));

	Input.close();

	std::cout << "Part one: " << GetValue() << std::endl;

	std::cout << "Part two: " << GetNonRedValue() << std::endl;


	system("pause");

	return 0;
}

static int64_t GetValue()
{
	return GetValueInRange(0, InputString.npos);
}

static int64_t GetNonRedValue()
{
	struct Object {
		int64_t Value;
		bool IsArray;
	};

	std::stack<Object> ValueStack({ { 0, true} });

	size_t CurrentPosition = 0;
	size_t BracketPosition = InputString.find('{');
	size_t RedPosition = InputString.find(Red);

	while (BracketPosition != InputString.npos)
	{
		bool ObjectIncludesRed = (RedPosition < BracketPosition);

		if ((!ValueStack.top().IsArray) && ObjectIncludesRed)
		{
			switch (InputString[BracketPosition])
			{
			case '[': // Push
			case '{': // Push
				BracketPosition = GetClosingBracketPosition(BracketPosition);
			case ']': // Pop
			case '}': // Pop
				ValueStack.pop();
				break;
			}
		}
		else
		{
			size_t Value = GetValueInRange(CurrentPosition, BracketPosition);

			switch (InputString[BracketPosition])
			{
			case '[': // Push
			case '{': // Push
				ValueStack.top().Value += Value;
				ValueStack.push({ 0, (InputString[BracketPosition] == '[') });
				break;
			case ']': // Pop
			case '}': // Pop
				size_t ValueBuffer = ValueStack.top().Value + Value;
				ValueStack.pop();
				ValueStack.top().Value += ValueBuffer;
				break;
			}
		}

		CurrentPosition = BracketPosition + 1;
		BracketPosition = InputString.find_first_of(Brackets, CurrentPosition);

		if (ObjectIncludesRed)
		{
			RedPosition = InputString.find(Red, CurrentPosition);
		}
	}

	return ValueStack.top().Value;
}

static int64_t GetValueInRange(size_t Position, size_t End)
{
	if (Position >= End)
	{
		return 0;
	}

	int64_t Sum = 0;

	Position = InputString.find_first_of(NumericCharacters, Position);

	while (Position < End)
	{
		size_t NextStart = InputString.find_first_not_of(NumericCharacters, Position);

		Sum += std::atoi(InputString.c_str() + Position);

		Position = InputString.find_first_of(NumericCharacters, NextStart);
	}

	return Sum;
}

static size_t GetClosingBracketPosition(size_t Start)
{
	uint32_t BracketLevel = 0;
	size_t BracketPosition = InputString.find_first_of(Brackets, Start);

	while (BracketPosition != InputString.npos)
	{
		switch (InputString[BracketPosition])
		{
		case ']':
		case '}':
			if (BracketLevel == 0)
			{
				return BracketPosition;
			}
			else
			{
				--BracketLevel;
			}
			break;
		case '[':
		case '{':
			++BracketLevel;
			break;
		}

		BracketPosition = InputString.find_first_of(Brackets, BracketPosition + 1);
	}

	return InputString.npos;
}