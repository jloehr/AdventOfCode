// Day09.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

uint64_t GetSinglePassDecompressionLength(const std::string & String);

int main()
{
	std::string Input = GetFileLines("Input.txt")[0];

	std::cout << "Decompressed Length: " << GetSinglePassDecompressionLength(Input) << std::endl;

	system("pause");
}

uint64_t GetSinglePassDecompressionLength(const std::string & String)
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
				Length += SequenceLength * Times;
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
