// Day16.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int main()
{
	const StringVector & Lines = GetFileLines("Input.txt");

	for(const auto & Line : Lines)
	{
		std::istringstream LineStream(Line);
		std::string Move;
		std::string Programs(16, '0');
		std::iota(Programs.begin(), Programs.end(), 'a');
		size_t Front = 0;

		while (std::getline(LineStream, Move, ','))
		{
			switch (Move[0])
			{
			case 's':
				Front += Programs.size() - std::stoull(Move.substr(1));
				Front %= Programs.size();
				break;
			case 'x':
			{
				size_t FirstArg = (Front + std::stoull(Move.substr(1))) % Programs.size();
				size_t SecondArg = (Front + std::stoull(Move.substr(Move.find('/') + 1))) % Programs.size();
				std::swap(Programs[FirstArg], Programs[SecondArg]);
				break;
			}
			case 'p':
				std::swap(Programs[Programs.find(Move[1])], Programs[Programs.find(Move[3])]);
				break;
			}
		}

		std::rotate(Programs.begin(), Programs.begin() + Front, Programs.end());
		std::cout << Programs << std::endl;
	}

	system("pause");
    return 0;
}

