// Day03.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int main()
{
	signed ValidTriangles = 0;

	StringVectorVector Lines = GetFileLineParts("Input.txt");

	for (const StringVector & Line : Lines)
	{
		signed BiggestSide = 0;
		signed OtherSides = 0;

		for (int i = 0; i < Line.size(); i++)
		{
			signed Side = std::atoi(Line[i].c_str());

			if (Side > BiggestSide)
			{
				OtherSides += BiggestSide;
				BiggestSide = Side;
			}
			else
			{
				OtherSides += Side;
			}
		}

		if (OtherSides > BiggestSide)
		{
			ValidTriangles++;
		}
	}


	std::cout << "Valid Triangels: " << ValidTriangles << std::endl;

	system("pause");
}

