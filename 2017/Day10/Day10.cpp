// Day10.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

const std::vector<uint8_t> Salt{ 17, 31, 73, 47, 23 };
const std::string Input = "147,37,249,1,31,2,226,0,161,71,254,243,183,255,30,70";

std::vector<uint8_t> Convert(const std::string & Input)
{
	std::istringstream InputStream(Input);
	std::vector<uint8_t> Lengths;
	std::string Buffer;

	while (std::getline(InputStream, Buffer, ','))
	{
		Lengths.push_back(static_cast<uint8_t>(std::stoul(Buffer)));
	}

	return Lengths;
}

void KnotHash(std::array<uint8_t, 256> & List, const std::vector<uint8_t> & Lengths, size_t & Cursor, uint8_t & SkipSize)
{
	for (auto Length : Lengths)
	{
		for (auto i = 0; i < Length / 2; ++i)
			std::swap(List[(Cursor + i) % List.size()], List[(Cursor + Length - 1 - i) % List.size()]);

		Cursor = (Cursor + Length + SkipSize++) % List.size();
	}
}

uint64_t PartOne()
{
	size_t Cursor = 0;
	uint8_t SkipSize = 0;
	std::vector<uint8_t> Lengths = Convert(Input);
	std::array<uint8_t, 256> List;
	std::iota(List.begin(), List.end(), 0);

	KnotHash(List, Lengths, Cursor, SkipSize);
	
	return List[0] * List[1];
}

std::string PartTwo()
{
	size_t Cursor = 0;
	uint8_t SkipSize = 0;
	std::vector<uint8_t> Lengths(Input.begin(), Input.end());
	Lengths.insert(Lengths.end(), Salt.begin(), Salt.end());
	std::array<uint8_t, 256> SparseHash;
	std::iota(SparseHash.begin(), SparseHash.end(), 0);

	for(auto i = 0; i < 64; ++i)
		KnotHash(SparseHash, Lengths, Cursor, SkipSize);

	std::array<uint8_t, 16> DenseHash;
	for (auto i = 0; i < DenseHash.size(); ++i)
	{
		DenseHash[i] = 0;
		for (auto j = 0; j < 16; ++j)
			DenseHash[i] ^= SparseHash[i * 16 + j];
	}

	std::stringstream Hash;
	for (auto Hex : DenseHash)
		Hash << std::hex << std::setw(2) << std::setfill('0') << static_cast<uint16_t>(Hex);

	return Hash.str();
}

int main()
{

	std::cout << "Part One: " << PartOne() << std::endl;
	std::cout << "Part Two: " << PartTwo() << std::endl;

	system("pause");
    return 0;
}

