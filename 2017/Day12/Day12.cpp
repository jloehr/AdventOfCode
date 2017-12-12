// Day12.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int main()
{
	const StringVectorVector Lines = GetFileLineParts("Input.txt");
	std::set<unsigned long> GroupZero;
	std::queue<unsigned long> Queue;
	Queue.push(0);

	while (!Queue.empty())
	{
		auto Result = GroupZero.insert(Queue.front());
		if (Result.second)
		{
			const auto & Line = Lines[Queue.front()];
			for (auto It = std::begin(Line) + 2; It != std::end(Line); std::advance(It, 1))
			{
				Queue.push(std::stoul(*It));
			}
		}

		Queue.pop();
	}

	std::cout << "Group 0: " << GroupZero.size() << std::endl;

	system("pause");
    return 0;
}

