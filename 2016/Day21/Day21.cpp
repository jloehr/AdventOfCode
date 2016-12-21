// Day21.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

void Swap(const StringVector & Line, std::string & Password, bool Reverse);
void Rotate(const StringVector & Line, std::string & Password, bool Reverse);
void Reverse(const StringVector & Line, std::string & Password, bool Reverse);
void Move(const StringVector & Line, std::string & Password, bool Reverse);

static const std::map<std::string, std::function<void(const StringVector &, std::string &, bool)>> InstructionJumpTable = {
	{"swap", &Swap},
	{"rotate", &Rotate},
	{"reverse", &Reverse},
	{"move", &Move }
};

std::string ScramblePassword(const StringVectorVector & Lines, std::string Password);
std::string UnscramblePassword(const StringVectorVector & Lines, std::string Password);

int main()
{
	const StringVectorVector Lines = GetFileLineParts("Input.txt");

	std::cout << "Scrambled Password: " << ScramblePassword(Lines, "abcdefgh") << std::endl;
	std::cout << "Unscrambled Password: " << UnscramblePassword(Lines, "fbgdceah") << std::endl;

	system("pause");

    return 0;
}

bool ApplyInstruction(const StringVector & Line, std::string & Password, bool Reverse)
{
	auto Function = InstructionJumpTable.find(Line[0]);
	if (Function == InstructionJumpTable.end())
	{
		std::cout << "Unknown Instruction: " << Line[0] << std::endl;
		return false;
	}
	else
	{
		Function->second(Line, Password, Reverse);
	}

	return true;
}

std::string ScramblePassword(const StringVectorVector & Lines, std::string Password)
{
	for (const StringVector & Line : Lines)
	{
		if (!ApplyInstruction(Line, Password, false))
			break;
	}

	return Password;
}

std::string UnscramblePassword(const StringVectorVector & Lines, std::string Password)
{
	for (auto It = Lines.rbegin(); It != Lines.rend(); It++)
	{
		if (!ApplyInstruction(*It, Password, true))
			break;
	}

	return Password;
}

void Swap(const StringVector & Line, std::string & Password, bool Reverse)
{
	size_t X = (Line[1] == "position") ? std::stoull(Line[2]) : Password.find(Line[2]);
	size_t Y = (Line[1] == "position") ? std::stoull(Line[5]) : Password.find(Line[5]);

	std::swap(Password[X], Password[Y]);
}

void Rotate(const StringVector & Line, std::string & Password, bool Reverse)
{
	size_t Steps;
	if (Line[1] == "based")
	{
		Steps = Password.find(Line[6]);

		if (Reverse)
		{
			if (Steps % 2 == 1)
			{
				Steps = Password.size() - (Steps / 2) - 1;
			}
			else
			{
				Steps = ((Password.size() + 2) - (((Steps / 2) + 3) % 4));
			}
		}
		else
		{
			Steps += (Steps >= 4) ? 2 : 1;
		}

	}
	else
	{
		Steps = std::stoull(Line[2]);
		if (Line[1] == "left" ^ Reverse)
		{
			Steps = Password.size() - Steps;
		}
	}

	Steps %= Password.size();

	Password = Password.substr(Password.size() - Steps) + Password.substr(0, Password.size() - Steps);
}

void Reverse(const StringVector & Line, std::string & Password, bool Reverse)
{
	size_t X = std::stoull(Line[2]);
	size_t Y = std::stoull(Line[4]) + 1;

	std::reverse(Password.begin() + X, Password.begin() + Y);
}

void Move(const StringVector & Line, std::string & Password, bool Reverse)
{
	size_t X = std::stoull(Line[2]);
	size_t Y = std::stoull(Line[5]);

	if (X == Y)
		return;

	if (Reverse)
	{
		std::swap(X, Y);
	}

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