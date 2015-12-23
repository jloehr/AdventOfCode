// Day9.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


struct City;
struct Road;
struct PathNode;
typedef std::shared_ptr<City> PCity;
typedef std::shared_ptr<Road> PRoad;
typedef std::shared_ptr<PathNode> PPathNode;

struct Road {
	typedef std::function<bool(PRoad, PRoad)> CompareFunction;
	static const CompareFunction AscendingSort;
	static const CompareFunction DescendingSort;

	PCity Start;
	PCity End;
	uint32_t Distance;

	Road(PCity Start, PCity End, uint32_t Distance)
		:Start(Start), End(End), Distance(Distance)
	{}
};

typedef std::multiset<PRoad, Road::CompareFunction> RoadSet;

struct City {
	std::string Name;
	RoadSet Roads;

	City(const std::string & Name)
		:Name(Name), Roads(Road::AscendingSort)
	{}
};

typedef std::map<std::string, PCity> CityMap;
typedef std::set<PCity> CitySet;

struct PathNode {
	typedef std::function<bool(PPathNode, PPathNode)> CompareFunction;
	static const CompareFunction AscendingSort;
	static const CompareFunction DescendingSort;

	uint32_t Value;
	PCity CurrentCity;
	CitySet VisitedCities;

	std::vector<PRoad> RoadVector;

	PathNode(PRoad StartingRoad)
		:VisitedCities({ StartingRoad->Start, StartingRoad->End }), Value(StartingRoad->Distance), CurrentCity(StartingRoad->End)
	{
		RoadVector.push_back(StartingRoad);
	}

	PathNode(PPathNode Parent, PRoad NewRoad)
		:VisitedCities(Parent->VisitedCities), Value(Parent->Value + NewRoad->Distance), CurrentCity(NewRoad->End), RoadVector(Parent->RoadVector)
	{
		VisitedCities.insert(CurrentCity);
		RoadVector.push_back(NewRoad);
	}

	void PrintInfo()
	{
		std::cout << RoadVector[0]->Start->Name;
		for (PRoad Road : RoadVector)
		{
			std::cout << " -(" << Road->Distance << ")- " << Road->End->Name;
		}

		std::cout << ": " << Value << std::endl;
	} 
};

typedef std::multiset<PPathNode, PathNode::CompareFunction> PathSet;

const Road::CompareFunction Road::AscendingSort = [](PRoad A, PRoad B) -> bool { return (A->Distance < B->Distance); };
const Road::CompareFunction Road::DescendingSort = [](PRoad A, PRoad B) -> bool { return (A->Distance > B->Distance); };
const PathNode::CompareFunction PathNode::AscendingSort = [](PPathNode A, PPathNode B) -> bool { return (A->Value < B->Value); };
const PathNode::CompareFunction PathNode::DescendingSort = [](PPathNode A, PPathNode B) -> bool { return (A->Value > B->Value); };

static void ParseLine(const std::string & Line, CityMap & Cities);
static uint32_t CalculateShortestPath(const CityMap & Cities);

int main()
{
	std::ifstream Input("Input.txt");

	if (!Input.is_open())
	{
		std::cout << "Error opening File!" << std::endl;
		return 0;
	}

	std::string Line;
	CityMap Cities;

	while (std::getline(Input, Line))
	{
		ParseLine(Line, Cities);
	}

	Input.close();

	uint32_t ShortestRoute = CalculateShortestPath(Cities);

	std::cout << "Shartest Route: " << ShortestRoute << std::endl;

	system("pause");

	return 0;
}

void ParseLine(const std::string & Line, CityMap & Cities)
{
	std::stringstream LineStream(Line);
	std::vector<std::string> Input;
	std::string Buffer;

	while (std::getline(LineStream, Buffer, ' '))
	{
		Input.push_back(Buffer);
	}

	uint32_t Distance = std::stoi(Input[4]);

	if (!Cities[Input[0]])
	{
		Cities[Input[0]] = std::make_shared<City>(Input[0]);
	}

	if (!Cities[Input[2]])
	{
		Cities[Input[2]] = std::make_shared<City>(Input[2]);
	}

	PCity From = Cities[Input[0]];
	PCity To = Cities[Input[2]];

	PRoad NewRoad = std::make_shared<Road>(From, To, Distance);
	PRoad NewRoadReverse = std::make_shared<Road>(To, From, Distance);

	From->Roads.insert(NewRoad);
	To->Roads.insert(NewRoadReverse);
}

uint32_t CalculateShortestPath(const CityMap & Cities)
{
	PathSet PossiblePaths(PathNode::AscendingSort);

	// Set Up
	for (auto City : Cities)
	{
		for (PRoad Road : City.second->Roads)
		{
			PossiblePaths.insert(std::make_shared<PathNode>(Road));
		}
	}

	while (!PossiblePaths.empty()) // Not finished
	{
		auto CurrentBestIter = PossiblePaths.begin();
		PPathNode CurrentBest = (*CurrentBestIter);
		PossiblePaths.erase(CurrentBestIter);

		bool NotFinished = false;

		for (PRoad Road : CurrentBest->CurrentCity->Roads)
		{
			auto Result = CurrentBest->VisitedCities.find(Road->End);
			if (Result == CurrentBest->VisitedCities.end())
			{
				NotFinished = true;
				PossiblePaths.insert(std::make_shared<PathNode>(CurrentBest, Road));
			}
		}

		if (!NotFinished)
		{
			CurrentBest->PrintInfo();
			return CurrentBest->Value;
		}
	}

	return 0;

}