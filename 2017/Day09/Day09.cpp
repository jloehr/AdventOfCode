// Day09.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int main()
{
	const StringVector Lines = GetFileLines("Input.txt");

	for (const auto & Line : Lines)
	{
		uint64_t TotalScore = 0;
		uint64_t CurrentScore = 0;
		size_t Cursor = 0;

		while ((Cursor = Line.find_first_of("{<}", Cursor)) != std::string::npos)
		{
			// Find Start of Comment, Group or End of Group
			switch (Line[Cursor])
			{
			case '{':
				++CurrentScore;
				break;
			case '<':
				{	
					// Find end of comment
					do
					{
						Cursor = Line.find_first_of("!>", Cursor);
						Cursor += (Line[Cursor] == '!') ? 2 : 0;
					} while (Line[Cursor] != '>'); 
				}
				break;
			case '}':
				TotalScore += CurrentScore;
				--CurrentScore;
				break;
			}
			++Cursor;
		}

		std::cout << "Total Score: " << TotalScore << std::endl;
	}

	system("pause");
	return 0;
}

