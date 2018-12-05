// Day05.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"


int main()
{
	std::ifstream Input("Input.txt");
	std::vector<char> Polymer;
	size_t Pointer = 0;

	char Character;
	while(Input.get(Character))
	{
		if ((Pointer == 0) || !((Character == (Polymer[Pointer - 1] + 32)) || (Character == (Polymer[Pointer - 1] - 32))))
		{
			if ((Pointer == Polymer.size()))
				Polymer.push_back(Character);
			else
				Polymer[Pointer] = Character;

			++Pointer;
		}
		else
			--Pointer;
	}

	std::cout << "Part One: " << Pointer << std::endl;
}
