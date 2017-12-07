// Day07.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

class Node;
using PNode = std::shared_ptr<Node>;
using NodeVector = std::vector<PNode>;
using NodeMap = std::map<std::string, PNode>;
using NodeSet = std::set<PNode>;

class Node 
{
public:
	Node(const std::string Name)
		:Name(Name), Weight(0), OverallWeight(0), Parent(nullptr) {}

	void AddChild(PNode & Node)
	{
		Nodes.push_back(Node);
	}

	void SetParent(PNode & Node)
	{
		Parent = Node;
	}

	void SetWeight(unsigned int Value)
	{
		Weight = Value;
		OverallWeight = Weight;

		for (const PNode & Node : Nodes)
			OverallWeight += Node->OverallWeight;

		PNode Node = Parent;
		while (Node)
		{
			Node->OverallWeight += OverallWeight;
			Node = Node->Parent;
		}
	}

	const std::string & GetName() const
	{
		return Name;
	}

	unsigned int GetOverallWeight() const
	{
		return OverallWeight;
	}

	unsigned int CalculateNewWeight(unsigned int TargetWeight) const
	{
		return TargetWeight - static_cast<unsigned int>((Nodes.size() * Nodes[0]->OverallWeight));
	}

	PNode GetUnbalancedNode(unsigned int & BalanceWeight) const
	{
		auto Result = std::find_if(Nodes.begin() + 1, Nodes.end(), [&](auto Node) -> bool { return Node->GetOverallWeight() != (*(Nodes.begin()))->GetOverallWeight(); });
		if (Result == Nodes.end())
		{
			return nullptr;
		}
		else
		{
			if ((Result == Nodes.begin() + 1) && (Nodes[1]->OverallWeight == Nodes[2]->OverallWeight))
			{
				BalanceWeight = Nodes[1]->OverallWeight;
				return Nodes[0];
			}
			else
			{
				BalanceWeight = Nodes[0]->OverallWeight;
				return *Result;
			}
		}
	}

private:
	std::string Name;
	unsigned int Weight;
	unsigned int OverallWeight;
	PNode Parent;
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
		unsigned int Weight = std::stoul(Line[1].substr(1, Line[1].length() - 2));

		auto Result = Nodes.emplace(Name, std::make_shared<Node>(Node(Name)));
		PNode NewNode = Result.first->second;

		if (Childrens.find(NewNode) == Childrens.end())
			PossibleRoots.insert(NewNode);

		if (Line.size() > 2)
		{
			for (auto ChildIt = Line.begin() + 3; ChildIt != Line.end(); ++ChildIt)
			{
				std::string ChildName = (*(ChildIt->rbegin()) == ',') ? ChildIt->substr(0, ChildIt->length() - 1) : *ChildIt;
				Result = Nodes.emplace(ChildName, std::make_shared<Node>(Node(ChildName)));
				PNode ChildNode = Result.first->second;

				NewNode->AddChild(ChildNode);
				ChildNode->SetParent(NewNode);
				Childrens.insert(ChildNode);
				PossibleRoots.erase(ChildNode);
			}
		}

		NewNode->SetWeight(Weight);
	}

	std::cout << "Root Nodes: " << PossibleRoots.size() << std::endl;
	for (const auto & Root : PossibleRoots)
	{
		std::cout << Root->GetName() << std::endl;

		PNode Current = Root;
		unsigned int BalancedWeight = 0;
		while (true) 
		{
			PNode Unbalanced = Current->GetUnbalancedNode(BalancedWeight);
			if (Unbalanced)
			{
				Current = Unbalanced;
			}
			else
			{
				std::cout << "New Weight: " << Current->CalculateNewWeight(BalancedWeight) << std::endl;
				break;
			}
		}
	}

	system("pause");
    return 0;
}

