// Day16.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

constexpr size_t ProgramCount = 16;
constexpr size_t LoopCount = 100;
enum class MoveInstruction { Spin, Exchange, Partner};

struct Move
{
	MoveInstruction Instruction;
	char Arg1;
	char Arg2;

	Move(MoveInstruction Instruction, char Arg1, char Arg2)
		:Instruction(Instruction), Arg1(Arg1), Arg2(Arg2) {}
};

std::vector<Move> ParseMoves(const std::string & Input)
{
	std::istringstream LineStream(Input);
	std::string MoveString;
	std::vector<Move> Moves;

	while (std::getline(LineStream, MoveString, ','))
	{
		switch (MoveString[0])
		{
		case 's':
			Moves.emplace_back(MoveInstruction::Spin, static_cast<char>(ProgramCount - std::stoi(MoveString.substr(1))), 0);
			break;
		case 'x':
			Moves.emplace_back(MoveInstruction::Exchange, static_cast<char>(std::stoi(MoveString.substr(1))), static_cast<char>(std::stoi(MoveString.substr(MoveString.find('/') + 1))));
			break;
		case 'p':
			Moves.emplace_back(MoveInstruction::Partner, MoveString[1], MoveString[3]);
			break;
		}
	}

	return Moves;
}

void DoDance(const std::vector<Move> & Moves, std::string & Programs)
{
	size_t Front = 0;

	std::for_each(std::begin(Moves), std::end(Moves), [&](const Move & Move) {
		switch (Move.Instruction)
		{
		case MoveInstruction::Spin:
			Front = (Front + Move.Arg1) % ProgramCount;
			break;
		case MoveInstruction::Exchange:
			std::swap(Programs[(Front + Move.Arg1) % ProgramCount], Programs[(Front + Move.Arg2) % ProgramCount]);
			break;
		case MoveInstruction::Partner:
			std::swap(Programs[Programs.find(Move.Arg1)], Programs[Programs.find(Move.Arg2)]);
			break;
		}
	});

	std::rotate(Programs.begin(), Programs.begin() + Front, Programs.end());
}

int main()
{
	const StringVector & Lines = GetFileLines("Input.txt");

	for(const auto & Line : Lines)
	{
		const auto Moves = ParseMoves(Line);
		std::string Programs(16, '0');
		std::iota(Programs.begin(), Programs.end(), 'a');
		std::vector<std::string> Configurations;
		std::unordered_map<std::string, size_t> UniqueConfigurations;
		size_t Index = SIZE_MAX;

		for (size_t i = 0; i < LoopCount; ++i)
		{
			Configurations.emplace_back(Programs);
			auto Result = UniqueConfigurations.insert(std::make_pair(Programs, i));
			if (!Result.second)
			{
				size_t InitialIndex = Result.first->second;
				size_t Interval = i - InitialIndex;
				Index = ((LoopCount - InitialIndex) % Interval) + InitialIndex;
				break;
			}
			DoDance(Moves, Programs);
		}

		std::cout << "Part one: " << Configurations[1] << std::endl;
		std::cout << "Part two: " << ((Index != SIZE_MAX) ? Configurations[Index] : Programs) << std::endl;
	}

	system("pause");
    return 0;
}

