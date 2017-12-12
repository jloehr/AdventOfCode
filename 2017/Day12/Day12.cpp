// Day12.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int main()
{
	const StringVectorVector Lines = GetFileLineParts("Input.txt");
	std::vector<size_t> GroupAffiliation(Lines.size(), SIZE_MAX);
	std::map<size_t, size_t> Groups;

	for (size_t i = 0; i < GroupAffiliation.size(); ++i)
	{
		if (GroupAffiliation[i] == SIZE_MAX)
		{
			std::set<size_t> Group;
			std::queue<size_t> Queue;
			Queue.push(i);

			while (!Queue.empty())
			{
				auto Result = Group.insert(Queue.front());
				if (Result.second)
				{
					const auto & Line = Lines[Queue.front()];
					for (auto It = std::begin(Line) + 2; It != std::end(Line); std::advance(It, 1))
					{
						Queue.push(static_cast<size_t>(std::stoull(*It)));
					}

					GroupAffiliation[Queue.front()] = i;
				}

				Queue.pop();
			}

			Groups[i] = Group.size();
		}
	}

	std::cout << Groups.size() << " Groups" << std::endl;
	std::cout << "Group 0: " << Groups[0] << std::endl;

	system("pause");
    return 0;
}

