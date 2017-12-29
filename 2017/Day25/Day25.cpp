// Day25.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

enum class Cell { Zero, One };
using CellVector = std::vector<Cell>;
using State = std::function<size_t(CellVector::iterator &)>;
using StateVector = std::vector<State>;

int main()
{
	const StringVectorVector Lines = GetFileLineParts("Input.txt");
	auto Line = std::begin(Lines);

	// Get initial state
	size_t StateIndex = (*Line++)[3][0] - 'A';
	uint64_t Steps = std::stoull((*Line++)[5]);

	StateVector States;

	while (Line != std::end(Lines))
	{
		++Line; // Blank space
		++Line; // In state $Foo:;
		++Line; // If value is 0:
		Cell ZeroValue = ((*Line++)[4][0] == '0') ? Cell::Zero : Cell::One; // Write value $Value;
		CellVector::iterator::difference_type ZeroMove = ((*Line++)[6] == "left.") ? 1 : -1; // Move one slot
		size_t ZeroState = (*Line++)[4][0] - 'A';
		++Line; // If value is 1:
		Cell OneValue = ((*Line++)[4][0] == '0') ? Cell::Zero : Cell::One; // Write value $Value;
		CellVector::iterator::difference_type OneMove = ((*Line++)[6] == "left.") ? 1 : -1; // Move one slot
		size_t OneState = (*Line++)[4][0] - 'A';

		States.push_back([=](CellVector::iterator & Cursor)->size_t {
			if (*Cursor == Cell::Zero)
			{
				*Cursor = ZeroValue;
				Cursor += ZeroMove;
				return ZeroState;
			}
			else
			{
				*Cursor = OneValue;
				Cursor += OneMove;
				return OneState;
			}
		});
	}

	CellVector Band(50, Cell::Zero);
	auto Cursor = std::begin(Band) + 25;

	while ((Steps--) > 0)
	{
		StateIndex = States[StateIndex](Cursor);
		if (Cursor == std::begin(Band))
		{
			size_t NewIndex = Band.size();
			CellVector NewBand(Band.size(), Cell::Zero);
			NewBand.insert(std::end(NewBand), std::begin(Band), std::end(Band));
			std::swap(Band, NewBand);
			Cursor = std::begin(Band) + NewIndex;
		}
		if (Cursor == std::end(Band))
		{
			size_t CursorIndex = Band.size();
			Band.resize(Band.size() * 2);
			Cursor = std::begin(Band) + CursorIndex;
		}
	}

	auto Ones = std::count(std::begin(Band), std::end(Band), Cell::One);

	std::cout << "Ones: " << Ones << std::endl;

	system("pause");
    return 0;
}

