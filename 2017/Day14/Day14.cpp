// Day14.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

std::array<uint8_t, 16> KnotHash(const std::vector<uint8_t> & Input)
{
	const static std::vector<uint8_t> Salt{ 17, 31, 73, 47, 23 };

	std::vector<uint8_t> Lengths(Input.begin(), Input.end());
	Lengths.insert(Lengths.end(), Salt.begin(), Salt.end());

	std::array<uint8_t, 256> List;
	std::iota(List.begin(), List.end(), 0);

	size_t Cursor = 0;
	uint8_t SkipSize = 0;

	for (auto i = 0; i < 64; ++i)
	{
		for (auto Length : Lengths)
		{
			for (auto i = 0; i < Length / 2; ++i)
				std::swap(List[(Cursor + i) % List.size()], List[(Cursor + Length - 1 - i) % List.size()]);

			Cursor = (Cursor + Length + SkipSize++) % List.size();
		}
	}
	
	std::array<uint8_t, 16> Hash;
	auto HashIt = std::begin(Hash);
	for (auto It = std::begin(List); It != std::end(List); std::advance(It, 16))
		*(HashIt++) = std::accumulate(It, std::next(It, 16), 0, std::bit_xor<void>());

	return Hash;
}

int main()
{
	const std::string Input = "ugkiagan";
	size_t SquaresUsed = 0;

	for (size_t i = 0; i < 128; ++i)
	{
		std::ostringstream RowStream;
		RowStream << Input << '-' << i;
		auto Row = RowStream.str();
		auto Hash = KnotHash(std::vector<uint8_t>(std::begin(Row), std::end(Row)));
		SquaresUsed += std::bitset<64>(reinterpret_cast <uint64_t*>(Hash.data())[0]).count();
		SquaresUsed += std::bitset<64>(reinterpret_cast <uint64_t*>(Hash.data())[1]).count();
	}

	std::cout << "Squares Used: " << SquaresUsed << std::endl;

	system("pause");
    return 0;
}

