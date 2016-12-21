// Day21.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

void Swap(const StringVector & Line, std::string & Password);
void Rotate(const StringVector & Line, std::string & Password);
void Reverse(const StringVector & Line, std::string & Password);
void Move(const StringVector & Line, std::string & Password);

static const std::map<std::string, std::function<void(const StringVector &, std::string &)>> InstructionJumpTable = {
	{"swap", &Swap},
	{"rotate", &Rotate},
	{"reverse", &Reverse},
	{"move", &Move }
};

int main()
{
	const StringVectorVector Lines = GetFileLineParts("Input.txt");
	std::string Password = "abcdefgh";

	for (const StringVector & Line : Lines)
	{
		auto Function = InstructionJumpTable.find(Line[0]);
		if (Function == InstructionJumpTable.end())
		{
			std::cout << "Unknown Instruction: " << Line[0] << std::endl;
			break;
		}
		else
		{
			Function->second(Line, Password);
		}
	}

	std::cout << Password << std::endl;

	system("pause");

    return 0;
}

void Swap(const StringVector & Line, std::string & Password)
{
	size_t X = (Line[1] == "position") ? std::stoull(Line[2]) : Password.find(Line[2]);
	size_t Y = (Line[1] == "position") ? std::stoull(Line[5]) : Password.find(Line[5]);

	std::swap(Password[X], Password[Y]);
}

void Rotate(const StringVector & Line, std::string & Password)
{
	size_t Steps;
	if (Line[1] == "based")
	{
		Steps = Password.find(Line[6]);
		Steps += (Steps >= 4) ? 2 : 1;
	}
	else
	{
		Steps = std::stoull(Line[2]);
		if (Line[1] == "left")
		{
			Steps = Password.size() - Steps;
		}
	}

	Steps %= Password.size();

	Password = Password.substr(Password.size() - Steps) + Password.substr(0, Password.size() - Steps);
}

void Reverse(const StringVector & Line, std::string & Password)
{
	size_t X = std::stoull(Line[2]);
	size_t Y = std::stoull(Line[4]) + 1;

	std::reverse(Password.begin() + X, Password.begin() + Y);
}

void Move(const StringVector & Line, std::string & Password)
{
	size_t X = std::stoull(Line[2]);
	size_t Y = std::stoull(Line[5]);

	if (X == Y)
		return;

	char Character = Password[X];

	if (X < Y)
	{
		std::copy(Password.begin() + X + 1, Password.begin() + Y + 1, Password.begin() + X);
	}
	else
	{
		std::copy_backward(Password.begin() + Y, Password.begin() + X, Password.begin() + X + 1);
	}

	Password[Y] = Character;
}