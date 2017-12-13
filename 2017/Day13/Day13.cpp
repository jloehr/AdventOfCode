// Day13.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int main()
{
	const StringVectorVector Lines = GetFileLineParts("Input.txt");
	size_t Severity = 0;

	for (const StringVector & Line : Lines)
	{
		size_t TimeStep = std::stoull(Line[0]);
		size_t Depth = std::stoull(Line[1]);
		if (TimeStep % ((Depth - 1) * 2) == 0)
			Severity += TimeStep * Depth;
	}

	std::cout << "Severity: " << Severity << std::endl;

	system("pause");
    return 0;
}

