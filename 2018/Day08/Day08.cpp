// Day08.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

typedef std::vector<uint64_t> IntVector;
typedef std::shared_ptr<IntVector> IntVectorPtr;

struct Node
{
	uint64_t AmountChildNodes;
	uint64_t AmountMetaData;
	IntVectorPtr ChildNodesValues;
};

int main()
{
	const StringVector File = GetFileLineParts("Input.txt")[0];

	uint64_t CheckSumPartOne = 0;
	std::stack<Node> NodeStack;

	auto It = File.cbegin();
	uint64_t AmountChildNodes = 1;
	uint64_t AmountMetaData = 0;
	IntVectorPtr ChildNodesValues = std::make_shared<IntVector>();

	while(It != File.cend())
	{
		if (AmountChildNodes > 0)
		{
			NodeStack.push({ AmountChildNodes, AmountMetaData, ChildNodesValues });

			AmountChildNodes = std::atoi(It++->c_str());
			AmountMetaData = std::atoi(It++->c_str()); 
			ChildNodesValues = (AmountChildNodes > 0) ? std::make_shared<IntVector>() : nullptr;
		}
		else
		{
			uint64_t MetaDataSum = 0;
			uint64_t NodeValue = 0;
			for (; AmountMetaData > 0; AmountMetaData--)
			{
				uint64_t MetaDataValue = std::atoi(It++->c_str());
				MetaDataSum += MetaDataValue;
				if (ChildNodesValues && ((MetaDataValue - 1) < ChildNodesValues->size()))
					NodeValue += ChildNodesValues->operator[](MetaDataValue - 1);
			}

			NodeValue = ChildNodesValues ? NodeValue : MetaDataSum;

			AmountChildNodes = NodeStack.top().AmountChildNodes;
			AmountMetaData = NodeStack.top().AmountMetaData;
			ChildNodesValues = NodeStack.top().ChildNodesValues;
			NodeStack.pop();

			CheckSumPartOne += MetaDataSum;
			ChildNodesValues->push_back(NodeValue);

			AmountChildNodes--;
		}
	}

	std::cout << "Part One: " << CheckSumPartOne << std::endl;
	std::cout << "Part Two: " << ChildNodesValues->operator[](0) << std::endl;
}
