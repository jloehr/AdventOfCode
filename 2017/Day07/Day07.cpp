// Day07.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

struct Node;
using PNode = std::shared_ptr<Node>;
using NodeVector = std::vector<PNode>;
using NodeMap = std::map<std::string, PNode>;
using NodeSet = std::set<PNode>;

struct Node 
{
	std::string Name;
	unsigned int Value;
	NodeVector Nodes;
};

int main()
{
	const StringVectorVector FileParts = GetFileLineParts("Input.txt");
	NodeMap Nodes;
	NodeSet PossibleRoots;
	NodeSet Childrens;

	for (const auto & Line : FileParts)
	{
		const auto & Name = Line[0];
		unsigned int Value = std::stoul(Line[1].substr(1, Line[1].length() - 2));

		auto Result = Nodes.emplace(Name, std::make_shared<Node>(Node { Name, Value }));
		PNode LineNode = Result.first->second;
		if (!Result.second)
		{
			LineNode->Value = Value;
		}

		if (Childrens.find(LineNode) == Childrens.end())
			PossibleRoots.insert(LineNode);

		if (Line.size() > 2)
		{
			for (auto ChildIt = Line.begin() + 3; ChildIt != Line.end(); ++ChildIt)
			{
				std::string Child = (*(ChildIt->rbegin()) == ',') ? ChildIt->substr(0, ChildIt->length() - 1) : *ChildIt;
				Result = Nodes.emplace(Child, std::make_shared<Node>(Node{ Child }));
				PNode ChildNode = Result.first->second;
				LineNode->Nodes.push_back(ChildNode);
				Childrens.insert(ChildNode);
				PossibleRoots.erase(ChildNode);
			}
		}
	}

	std::cout << "Root Nodes: " << PossibleRoots.size() << std::endl;
	for(const auto & Node : PossibleRoots)
		std::cout << Node->Name << std::endl;

	system("pause");
    return 0;
}

