// Day24.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

struct Node;
typedef std::pair<size_t, size_t> Vector2D;
typedef std::shared_ptr<Node> NodePtr;
typedef std::vector<NodePtr> NodeVector;
typedef std::pair<NodePtr, size_t> NodeDistance;
typedef std::vector<NodeDistance> NodeDistanceVector;

struct Node 
{
	size_t Number;
	NodeDistanceVector Neighbors;
};

NodeVector GetNodes(const std::string & InputFile); 
void FloodFill(Vector2D Start, NodePtr StartNode, NodeVector & Nodes, const StringVector & Map);
uint64_t GetShortestRoute(const NodeVector & Nodes);

int main()
{
	NodeVector Nodes = GetNodes("Input.txt");
	
	/*
	for (auto & Node : Nodes)
	{
		std::cout << Node->Number << ": ";
		for (auto & Neighbor : Node->Neighbors)
		{
			std::cout << Neighbor.first->Number << "->" << std::setw(5) << Neighbor.second << "    ";
		}

		std::cout << std::endl;
	}
	*/

	std::cout << "Steps: " << GetShortestRoute(Nodes) << std::endl;

	system("pause");

    return 0;
}

NodeVector GetNodes(const std::string & InputFile)
{
	const StringVector Lines = GetFileLines(InputFile);
	std::map<size_t, Vector2D> Points;

	for (size_t Y = 0; Y < Lines.size(); Y++)
	{
		const std::string & Line = Lines[Y];

		for (size_t X = 0; X < Line.size(); X++)
		{
			if (Line[X] == '.' || Line[X] == '#')
				continue;

			Points.insert({ static_cast<size_t>(Line[X] - '0'), { X, Y } });
		}
	}

	NodeVector Nodes(Points.size());
	std::generate(Nodes.begin(), Nodes.end(), std::make_shared<Node>);

	for (auto Point : Points)
	{
		Nodes[Point.first]->Number = Point.first;
		FloodFill(Point.second, Nodes[Point.first], Nodes, Lines);
	}

	return Nodes;
}

void Add(Vector2D Position, std::set<Vector2D> & VisitedPosition, std::list<Vector2D> & NewPositions, NodePtr & Node, NodeVector & Nodes, const StringVector & Map, size_t Distance)
{
	char Tile = Map[Position.second][Position.first];

	if (Tile == '#')
		return;

	auto Result = VisitedPosition.insert(Position);
	if (!Result.second)
		return;

	NewPositions.push_back(Position);

	if (Tile != '.')
	{
		size_t NodeId = Tile - '0';
		NodePtr OtherNode = Nodes[NodeId];

		for (auto Neighbor : Node->Neighbors)
		{
			if (Neighbor.first == OtherNode)
				return;
		}

		Node->Neighbors.emplace_back(OtherNode, Distance);
		OtherNode->Neighbors.emplace_back(Node, Distance);
	}
}

void DebugPrint(const std::set<Vector2D> & VisitedPositions, const std::list<Vector2D> & New, const std::list<Vector2D> & Open, const StringVector & Map)
{
	constexpr char Visited = '0';
	constexpr char NewChar = '.';
	constexpr char OpenChar = 'X';

	system("cls");

	for (size_t Y = 0; Y < Map.size(); ++Y)
	{
		for (size_t X = 0; X < Map[Y].size(); ++X)
		{
			Vector2D Position = { X ,Y };

			if (std::find(New.begin(), New.end(), Position) != New.end())
			{
				std::cout << NewChar;
				continue;
			}

			if (std::find(Open.begin(), Open.end(), Position) != Open.end())
			{
				std::cout << OpenChar;
				continue;
			}

			if (VisitedPositions.find(Position) != VisitedPositions.end())
			{
				std::cout << Visited;
				continue;
			}

			switch(Map[Y][X])
			{
			case '#':
				std::cout << '+';
				break;
			case '.':
				std::cout << ' ';
				break;
			default:
				std::cout << Map[Y][X];
			}
		}

		std::cout << std::endl;
	}

	system("pause");
}

void FloodFill(Vector2D Start, NodePtr StartNode, NodeVector & Nodes, const StringVector & Map)
{
	std::list<Vector2D> NewPositions({ Start });
	std::list<Vector2D> OpenPositions;
	std::set<Vector2D> VisitedPosition({ Start });

	size_t Distance = 0;

	do {
		//DebugPrint(VisitedPosition, NewPositions, OpenPositions, Map);
		std::swap(NewPositions, OpenPositions);
		Distance++;

		while (!OpenPositions.empty())
		{
			Vector2D Pos = OpenPositions.front();
			OpenPositions.pop_front();

			if (Pos.first > 0)
				Add({ Pos.first - 1, Pos.second}, VisitedPosition, NewPositions, StartNode, Nodes, Map, Distance);

			if (Pos.first < Map[0].size())
				Add({ Pos.first + 1, Pos.second}, VisitedPosition, NewPositions, StartNode, Nodes, Map, Distance);

			if (Pos.second > 0)
				Add({ Pos.first, Pos.second - 1}, VisitedPosition, NewPositions, StartNode, Nodes, Map, Distance);

			if (Pos.second < Map.size())
				Add({ Pos.first, Pos.second + 1}, VisitedPosition, NewPositions, StartNode, Nodes, Map, Distance);


		}
	} while (!NewPositions.empty());
}

uint64_t GetShortestRouteRec(const NodePtr Node, std::vector<NodePtr> VisitedNodes,const uint64_t RouteDistance, uint64_t ShortestRouteSoFar, const size_t NodeCount)
{
	if (RouteDistance > ShortestRouteSoFar)
		return ShortestRouteSoFar;

	VisitedNodes.push_back(Node);

	if (VisitedNodes.size() == NodeCount)
	{
		/*
		for (auto & PathNode : VisitedNodes)
		{
			std::cout << PathNode->Number << " -> ";
		}
		std::cout << std::endl;
		*/

		return RouteDistance;
	}

	for (const NodeDistance & OtherNode : Node->Neighbors)
	{
		if (std::find(VisitedNodes.begin(), VisitedNodes.end(), OtherNode.first) != VisitedNodes.end())
			continue;

		ShortestRouteSoFar = std::min(ShortestRouteSoFar, GetShortestRouteRec(OtherNode.first, VisitedNodes, RouteDistance + OtherNode.second, ShortestRouteSoFar, NodeCount));
	}

	return ShortestRouteSoFar;
}

uint64_t GetShortestRoute(const NodeVector & Nodes)
{
	uint64_t ShortestRouteDistance = UINT64_MAX;

	ShortestRouteDistance = GetShortestRouteRec(Nodes[0], {}, 0, ShortestRouteDistance, Nodes.size());

	return ShortestRouteDistance;
}