// Day09.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

uint64_t GetDecompressionLength(const std::string & String, bool SinglePass);

int main()
{
	std::string Input = GetFileLines("Input.txt")[0];

	std::cout << "Part One: " << GetDecompressionLength(Input, true) << std::endl;
	std::cout << "Part Two: " << GetDecompressionLength(Input, false) << std::endl;

	system("pause");
}

uint64_t GetDecompressionLength(const std::string & String, bool SinglePass)
{
	uint64_t Length = 0;
	uint64_t SequenceLength = 0;
	std::string Temp;
	bool RecordTemp = false;

	for (auto It = String.begin(); It != String.end(); It++)
	{
		switch (*It)
		{
		case '(':
			RecordTemp = true;
			break;
		case 'x':
			SequenceLength = std::stoull(Temp);
			Temp.clear();
			break;
		case ')':
		{
			uint64_t Times = std::stoull(Temp);
			Temp.clear();
			Length += Times * (SinglePass ? SequenceLength : GetDecompressionLength(std::string(It + 1, It + SequenceLength + 1), false));
			It += SequenceLength;
			RecordTemp = false;;
			break;
		}
		default:
			if (RecordTemp)
			{
				Temp += *It;
			}
			else
			{
				Length++;
			}
		}
	}

	return Length;
}

