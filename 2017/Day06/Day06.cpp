// Day06.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int main()
{
	auto Memory = GetValues<unsigned int>("Input.txt");
	std::set<std::vector<unsigned int>> Configurations = { Memory };
	size_t Steps = 0;

	for (; Steps == (Configurations.size() - 1); ++Steps)
	{
		auto Bank = std::max_element(Memory.begin(), Memory.end());
		unsigned int Blocks = *Bank;
		*Bank = 0;

		for (; Blocks > 0; --Blocks)
		{
			if(++Bank == Memory.end())
				Bank = Memory.begin();

			++(*Bank);
		}

		Configurations.insert(Memory);
	}

	std::cout << "Steps: " << Steps << std::endl;

	system("pause");
    return 0;
}

