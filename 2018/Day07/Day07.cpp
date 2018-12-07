// Day07.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

struct Step;
typedef std::shared_ptr<Step> StepPtr;

struct Step
{
	char Name;
	bool Consumed;
	std::set<StepPtr> Prerequisites;
	std::set<StepPtr> FollowingStep;

	Step(char Name)
		:Name(Name), Consumed(false) {}
};

int main()
{
	const StringVector File = GetFileLines("Input.txt");
	std::map<char, StepPtr> InstructionNodes;
	std::set<StepPtr> StepQueue;
	std::string Instructions;

	for (const std::string & Line : File)
	{
		char Required = Line[5];
		char StepName = Line[36];

		StepPtr & RequiredNode = InstructionNodes[Required];
		if (RequiredNode == nullptr)
		{
			RequiredNode = std::make_shared<Step>(Required);
			StepQueue.insert(RequiredNode);
		}

		StepPtr & Node = InstructionNodes[StepName];
		if (Node == nullptr)
			Node = std::make_shared<Step>(StepName);

		Node->Prerequisites.insert(RequiredNode);
		RequiredNode->FollowingStep.insert(Node);
		StepQueue.erase(Node);
	}

	while (!StepQueue.empty())
	{
		auto NextStep = std::min_element(std::begin(StepQueue), std::end(StepQueue), [](const StepPtr & lhs, const StepPtr & rhs) { return (lhs->Name < rhs->Name); });
		StepPtr CurrentStep = *NextStep;
		StepQueue.erase(NextStep);

		Instructions += CurrentStep->Name;
		CurrentStep->Consumed = true;

		for (StepPtr Next : CurrentStep->FollowingStep)
		{
			if (std::any_of(std::begin(Next->Prerequisites), std::end(Next->Prerequisites), [](const StepPtr & Pre) { return !Pre->Consumed; }))
				continue;

			StepQueue.insert(Next);
		}
	}

	std::cout << "Part One: " << Instructions << std::endl;
}
