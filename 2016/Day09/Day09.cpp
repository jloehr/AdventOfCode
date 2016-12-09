// Day09.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

uint64_t GetDecompressionLength(std::string::const_iterator Start, std::string::const_iterator End, bool SinglePass);

int main()
{
	std::string Input = GetFileLines("Input.txt")[0];

	std::cout << "Part One: " << GetDecompressionLength(Input.begin(), Input.end(), true) << std::endl;
	std::cout << "Part Two: " << GetDecompressionLength(Input.begin(), Input.end(), false) << std::endl;

	system("pause");
}

uint64_t GetDecompressionLength(std::string::const_iterator It, std::string::const_iterator End, bool SinglePass)
{
	uint64_t Length = 0;
	uint64_t SequenceLength = 0;
	uint64_t Times = 0;
	bool InsideMarker = false;

	for (; It != End; It++)
	{
		switch (*It)
		{
		case '(':
			InsideMarker = true;
			SequenceLength = std::stoull(&(*(It + 1)));
			break;
		case 'x':
			Times = std::stoull(&(*(It + 1)));
			break;
		case ')':
		{
			Length += Times * (SinglePass ? SequenceLength : GetDecompressionLength(It + 1, It + SequenceLength + 1, false));
			It += SequenceLength;
			InsideMarker = false;
			break;
		}
		default:
			if (!InsideMarker)
			{
				Length++;
			}
		}
	}

	return Length;
}

