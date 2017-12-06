// Day06.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int main()
{
	auto Memory = GetValues<unsigned int>("Input.txt");
	std::map<std::vector<unsigned int>, size_t> Configurations = { { Memory, 0 } };
	size_t Steps = 0;
	
	while(Steps == (Configurations.size() - 1))
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

		++Steps;
		Configurations.insert({ Memory, Steps});
	}

	std::cout << "Steps: " << Steps << std::endl;
	std::cout << "Cycle: " << (Steps - Configurations[Memory]) << std::endl;

	system("pause");
    return 0;
}

