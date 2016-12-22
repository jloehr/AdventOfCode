// Day22.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

struct Node
{
	uint32_t Size;
	uint32_t Used;
	uint32_t Avail;
	uint8_t Use;
};

typedef std::vector<Node> NodeVector;
typedef std::vector<NodeVector> NodeVectorVector;

NodeVectorVector ParseInput(std::string InputFile, uint32_t & ViablePairsCount);

int main()
{
	uint32_t ViablePairsCount;

	NodeVectorVector Nodes = ParseInput("Input.txt", ViablePairsCount);

	std::cout << "Viable Pairs: " << ViablePairsCount << std::endl;

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
		// "/dev/grid/node-x[[:digit:]]+-y[[:digit:]]+\s+[[:digit:]]+T\s+[[:digit:]]+T\s+[[:digit:]]+T\s+[[:digit:]]+%"
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

			Nodes[X][Y].Size = static_cast<uint32_t>(std::stoull(Line[1]));
			Nodes[X][Y].Used = static_cast<uint32_t>(std::stoull(Line[2]));
			Nodes[X][Y].Avail = static_cast<uint32_t>(std::stoull(Line[3]));
			Nodes[X][Y].Use = static_cast<uint8_t>(std::stoull(Line[4]));

			for (const Node & Node : ParsedNodes)
			{
				if (((Nodes[X][Y].Used > 0) && (Nodes[X][Y].Used <= Node.Avail)) ||
					((Node.Used > 0) && (Node.Used <= Nodes[X][Y].Avail)))
					ViablePairsCount++;
			}

			ParsedNodes.push_back(Nodes[X][Y]);
		}
	}

	return  Nodes;
}
