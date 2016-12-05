// Day03.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "TriangleTest.h"

int main()
{
	signed ValidRowTriangles = 0;
	signed ValidColumnTriangles = 0;

	StringVectorVector Lines = GetFileLineParts("Input.txt");

	TriangleTest RowTest;
	std::array<TriangleTest, 3> ColumnTest;

	for (const StringVector & Line : Lines)
	{
		for (int i = 0; i < Line.size(); i++)
		{
			signed Side = std::atoi(Line[i].c_str());

			if (RowTest.PushSide(Side))
			{
				ValidRowTriangles++;
			}

			if (ColumnTest[i].PushSide(Side))
			{
				ValidColumnTriangles++;
			}
		}
	}

	std::cout << "Valid Row Triangels: " << ValidRowTriangles << std::endl;
	std::cout << "Valid Column Triangels: " << ValidColumnTriangles << std::endl;

	system("pause");
}

