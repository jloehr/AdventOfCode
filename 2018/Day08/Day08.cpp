// Day08.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

int main()
{
	const StringVector File = GetFileLineParts("Input.txt")[0];

	uint64_t CheckSum = 0;
	std::stack<std::pair<uint64_t, uint64_t>> NodeStack;

	auto It = File.cbegin();
	uint64_t AmountChildNodes = 1;
	uint64_t AmountMetaData = 0;

	while(It != File.cend())
	{
		if (AmountChildNodes > 0)
		{
			NodeStack.push({ AmountChildNodes, AmountMetaData });

			AmountChildNodes = std::atoi(It++->c_str());
			AmountMetaData = std::atoi(It++->c_str());
		}
		else
		{
			for (; AmountMetaData > 0; AmountMetaData--)
				CheckSum += std::atoi(It++->c_str());

			AmountChildNodes = NodeStack.top().first;
			AmountMetaData = NodeStack.top().second;
			NodeStack.pop();

			AmountChildNodes--;
		}
	}

	std::cout << "Check Sum: " << CheckSum << std::endl;
}
