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

std::pair<std::string, size_t> BuildInstruction(const std::set<StepPtr> & StartingSteps, size_t WorkerCount = 1);

int main()
{
	const StringVector File = GetFileLines("Input.txt");
	std::map<char, StepPtr> InstructionNodes;
	std::set<StepPtr> StartingSteps;

	for (const std::string & Line : File)
	{
		char Required = Line[5];
		char StepName = Line[36];

		StepPtr & RequiredNode = InstructionNodes[Required];
		if (RequiredNode == nullptr)
		{
			RequiredNode = std::make_shared<Step>(Required);
			StartingSteps.insert(RequiredNode);
		}

		StepPtr & Node = InstructionNodes[StepName];
		if (Node == nullptr)
			Node = std::make_shared<Step>(StepName);

		Node->Prerequisites.insert(RequiredNode);
		RequiredNode->FollowingStep.insert(Node);
		StartingSteps.erase(Node);
	}

	auto PartOne = BuildInstruction(StartingSteps);
	// Reset
	std::for_each(std::begin(InstructionNodes), std::end(InstructionNodes), [](auto & Element) { Element.second->Consumed = false; });
	auto PartTwo = BuildInstruction(StartingSteps, 5);

	std::cout << "Part One: " << PartOne.first << std::endl;
	std::cout << "Part Two: " << PartTwo.second << std::endl;
}

std::pair<std::string, size_t> BuildInstruction(const std::set<StepPtr> & StartingSteps, size_t WorkerCount)
{
	std::string Instructions;
	std::set<StepPtr> StepQueue(StartingSteps);

	size_t CurrentTimeStep = 0;
	std::vector<std::pair<size_t, StepPtr>> WorkerList(WorkerCount, { 0, nullptr });

	while (!StepQueue.empty() || std::any_of(std::begin(WorkerList), std::end(WorkerList), [](const auto & Worker) { return (Worker.second != nullptr); }))
	{
		auto NextFreeWorker = std::min_element(std::begin(WorkerList), std::end(WorkerList), [&](const auto & lhs, const auto & rhs)
		{ 
			if (!StepQueue.empty() || (!lhs.second == !rhs.second))
				return lhs.first < rhs.first;
			else
				return !rhs.second;
		});
		CurrentTimeStep = std::max(CurrentTimeStep, NextFreeWorker->first);

		if (StepPtr FinishedStep = NextFreeWorker->second)
		{
			Instructions += FinishedStep->Name;
			FinishedStep->Consumed = true;

			for (StepPtr Next : FinishedStep->FollowingStep)
			{
				if (std::any_of(std::begin(Next->Prerequisites), std::end(Next->Prerequisites), [](const StepPtr & Pre) { return !Pre->Consumed; }))
					continue;

				StepQueue.insert(Next);
			}
		}

		auto NextStep = std::min_element(std::begin(StepQueue), std::end(StepQueue), [](const StepPtr & lhs, const StepPtr & rhs) { return (lhs->Name < rhs->Name); });
		if (NextStep == std::end(StepQueue))
		{
			NextFreeWorker->second = nullptr; 
			continue;
		}

		NextFreeWorker->first = CurrentTimeStep + 61 + ((*NextStep)->Name - 'A');
		NextFreeWorker->second = *NextStep;
		StepQueue.erase(NextStep);
	}

	return { Instructions, CurrentTimeStep };
}
