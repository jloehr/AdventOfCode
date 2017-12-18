// Day17.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

constexpr size_t Input = 314;

int main()
{
	std::deque<size_t> List{ 0, 1 };
	size_t Position = 1;

	for (auto i = 2; i < 2018; ++i)
	{
		Position = ((Position + Input) % List.size()) + 1;
		auto It = std::begin(List) + Position; 
		List.emplace(It, i);
	}

	std::cout << *(std::begin(List) + ((Position + 1) % List.size())) << std::endl;
	system("pause");
    return 0;
}

