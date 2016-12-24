// Day22.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

struct Node
{
	uint16_t Size;
	uint16_t Avail;
	uint16_t Used;
	size_t X, Y;
};

typedef std::vector<Node> NodeVector;
typedef std::vector<NodeVector> NodeVectorVector;

typedef std::pair<size_t, size_t> Position;
typedef std::vector<bool> BoolVector;
typedef std::vector<BoolVector> BoolVectorVector;

NodeVectorVector ParseInput(std::string InputFile, uint32_t & ViablePairsCount);
size_t GetStepsToAccess(const NodeVectorVector & Nodes);

int main()
{
	uint32_t ViablePairsCount;

	NodeVectorVector Nodes = ParseInput("Input.txt", ViablePairsCount);

	std::cout << "Viable Pairs: " << ViablePairsCount << std::endl;
	std::cout << "Steps: " << GetStepsToAccess(Nodes) << std::endl;

	/*
	for (size_t Y = 0; Y < Nodes[0].size(); Y++)
	{
		for (size_t X = 0; X < Nodes.size(); X++)
		{
			std::cout << ((Nodes[X][Y].Size - Nodes[X][Y].Avail) <= 92) ? "_" : "X";
			//std::cout << std::setw(3) << Nodes[X][Y].Avail;

			if (X < (Nodes.size() - 1))
			{
				std::cout << " - ";
			}
		}
		std::cout << std::endl;
	}*/

	system("pause");

    return 0;
}

NodeVectorVector ParseInput(std::string InputFile, uint32_t & ViablePairsCount)
{
	NodeVector ParsedNodes;
	NodeVectorVector Nodes;
	const StringVectorVector Lines = GetFileLineParts(InputFile);

	ViablePairsCount = 0;

	for (const StringVector & Line : Lines)
	{
		std::smatch StringMatch;
		if (std::regex_match(Line[0], StringMatch, std::regex("/dev/grid/node-x[[:digit:]]+-y[[:digit:]]+")))
		{
			
			size_t X = std::stoull(Line[0].substr(Line[0].find('x') + 1));
			size_t Y = std::stoull(Line[0].substr(Line[0].find('y') + 1));

			if (Nodes.size() < (X + 1))
			{
				Nodes.resize(X + 1);
			}

			if (Nodes[X].size() < (Y + 1))
			{
				Nodes[X].resize(Y + 1);
			}

			Nodes[X][Y].Size = static_cast<uint16_t>(std::stoull(Line[1]));
			Nodes[X][Y].Used = static_cast<uint16_t>(std::stoull(Line[2]));
			Nodes[X][Y].Avail = static_cast<uint16_t>(std::stoull(Line[3]));
			Nodes[X][Y].X = X;
			Nodes[X][Y].Y = Y;


			for (const Node & Node : ParsedNodes)
			{
				uint16_t NodeAUsed = Nodes[X][Y].Size - Nodes[X][Y].Avail;
				uint16_t NodeBUsed = Node.Size - Node.Avail;

				if (((NodeAUsed > 0) && (NodeAUsed <= Node.Avail)) ||
					((NodeBUsed > 0) && (NodeBUsed <= Nodes[X][Y].Avail)))
				{
					ViablePairsCount++;
				}
			}

			ParsedNodes.push_back(Nodes[X][Y]);
		}
	}

	return  Nodes;
}

Position GetEmptyNode(const NodeVectorVector & Nodes)
{
	for (size_t X = 0; X < Nodes.size(); X++)
	{
		for (size_t Y = 0; Y < Nodes[X].size(); Y++)
		{
			if (Nodes[X][Y].Used == 0)
				return{ X, Y };
		}
	}

	return { Nodes.size() - 1, 0 };
}

size_t GetF(const Position & Destination, const std::pair<Position, size_t> & Path)
{
	return Path.second + (std::abs(static_cast<int>(Destination.first) - static_cast<int>(Path.first.first))) + (std::abs(static_cast<int>(Destination.second) - static_cast<int>(Path.first.second)));
}

void Add(const NodeVectorVector & Nodes, std::set<Position> & ClosedList, std::map<size_t, std::pair<Position, size_t>> & OpenList, const Position & PreviousNode, const Position & NewNode, size_t Steps, const Position & Destination)
{
	if (ClosedList.find(NewNode) != ClosedList.end())
		return;

	if (Nodes[NewNode.first][NewNode.second].Used > Nodes[PreviousNode.first][PreviousNode.second].Size)
		return;

	std::pair<Position, size_t> OpenNode = { NewNode, Steps };

	OpenList.insert({ GetF(Destination, OpenNode), OpenNode });
}

size_t FindPath(const NodeVectorVector & Nodes, const Position & Start, const Position & Destination)
{
	std::set<Position> ClosedList;
	std::map<size_t, std::pair<Position, size_t>> OpenList;

	std::pair<Position, size_t> Node = { Start, 0 };

	OpenList.insert({ GetF(Destination, Node), Node });

	while (!OpenList.empty())
	{
		Node = OpenList.begin()->second;
		OpenList.erase(OpenList.begin());
		auto Result = ClosedList.insert(Node.first);
		if (!Result.second)
			continue;

		if (Node.first == Destination)
		{
			return Node.second;
		}

		if (Node.first.first > 0)
			Add(Nodes, ClosedList, OpenList, Node.first, { Node.first.first - 1 , Node.first.second }, Node.second + 1, Destination);

		if (Node.first.first < Nodes.size() - 1)
			Add(Nodes, ClosedList, OpenList, Node.first, { Node.first.first + 1 , Node.first.second }, Node.second + 1, Destination);

		if (Node.first.second > 0)
			Add(Nodes, ClosedList, OpenList, Node.first, { Node.first.first, Node.first.second - 1}, Node.second + 1, Destination);

		if (Node.first.second < Nodes[0].size() - 1)
			Add(Nodes, ClosedList, OpenList, Node.first, { Node.first.first, Node.first.second + 1}, Node.second + 1, Destination);

	}

	return 0;
}

size_t GetStepsToAccess(const NodeVectorVector & Nodes)
{
	constexpr Position Destination = { 0, 0 };
	Position Data = { Nodes.size() - 1, 0 };
	Position Empty = GetEmptyNode(Nodes);

	size_t EmptyToData = FindPath(Nodes, Empty, Data);
	Data.first--;

	constexpr size_t CycleSteps = 5;
	
	return Data.first * CycleSteps + EmptyToData;
}