// Day08.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

constexpr uint8_t DisplayHeight = 6;
constexpr uint8_t DisplayWidth = 50;

typedef std::array<std::bitset<DisplayHeight>, DisplayWidth> BitArray;

void Rectangle(const StringVector & Line, BitArray & Display);
void RotateRow(const StringVector & Line, BitArray & Display);
void RotateColumn(const StringVector & Line, BitArray & Display);
void PrintDisplay(const BitArray & Display);

int main()
{
	StringVectorVector Lines = GetFileLineParts("Input.txt");
	BitArray Display = { 0 };

	for (const StringVector & Line : Lines)
	{
		if(Line[0] == "rect")
		{
			Rectangle(Line, Display);
		}
		else if (Line[1] == "row")
		{
			RotateRow(Line, Display);
		}
		else
		{
			RotateColumn(Line, Display);
		}
	}

	size_t BitCount = 0;

	for (auto & Column : Display)
	{
		BitCount += Column.count();
	}

	std::cout << "Pixel Count: " << BitCount << std::endl;

	PrintDisplay(Display);

	system("pause");

    return 0;
}

void GetRectangleSize(const StringVector & Line, size_t & X, size_t & Y)
{
	X = std::atoi(Line[1].c_str());
	size_t DelimiterPosition = Line[1].find('x');
	Y = std::atoi(Line[1].c_str() + DelimiterPosition + 1);
}

void Rectangle(const StringVector & Line, BitArray & Display)
{
	static const std::array<uint8_t, (DisplayHeight + 1)> RowLookup = { 0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F };

	size_t X;
	size_t Y;

	GetRectangleSize(Line, X, Y);

	for (int i = 0; i < X; i++)
	{
		Display[i] |= RowLookup[Y];
	}
}

void GetRotateArguments(const StringVector & Line, size_t & Position, size_t & Count)
{
	Position = std::atoi(Line[2].c_str() + 2);
	Count = std::atoi(Line[4].c_str());
}

void RotateRow(const StringVector & Line, BitArray & Display)
{
	std::bitset<DisplayWidth> Row = 0;
	size_t Position;
	size_t Count;

	GetRotateArguments(Line, Position, Count);
	Count %= Display.size();

	// Create Row Bitset
	for (int i = 0; i < Display.size(); i++)
	{
		Row[i] = Display[i].test(Position) ? 1 : 0;
	}

	// Do Rotation
	Row = (Row << Count | Row >> (Display.size() - Count));

	// Assign Row to Display
	for (int i = 0; i < Display.size(); i++)
	{
		Display[i][Position] = Row.test(i) ? 1 : 0;
	}
}

void RotateColumn(const StringVector & Line, BitArray & Display)
{
	size_t Position;
	size_t Count;

	GetRotateArguments(Line, Position, Count);

	Count %= DisplayHeight;

	Display[Position] = (Display[Position] << Count | Display[Position] >> (DisplayHeight - Count));
}

void PrintDisplay(const BitArray & Display)
{
	for (int y = 0; y < DisplayHeight; y++)
	{
		for (int x = 0; x < DisplayWidth; x++)
		{
			std::cout << (Display[x][y] ? "#" : " ");
		}

		std::cout << std::endl;
	}

	std::cout << std::endl;
}
