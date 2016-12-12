// Day11.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define ParTwo

#ifndef ParTwo 
constexpr size_t ElementCount = 5;
#else
constexpr size_t ElementCount = 7;
#endif
constexpr size_t FloorCount = 4;

typedef std::bitset<ElementCount> ComponentStatus;
struct Floor;
typedef std::array<Floor, FloorCount> Compound;
struct Configuration;
typedef std::vector<Configuration> ConfigurationVector;
typedef std::unordered_set<size_t> HashSet;

struct Floor
{
	ComponentStatus Generators;
	ComponentStatus Chips;
};

struct Configuration
{
	Compound Facility;
	size_t Elevator;

	std::shared_ptr<Configuration> Previous;

	Configuration(Compound Facility, size_t Elevator, std::shared_ptr<Configuration> Previous)
		:Facility(Facility), Elevator(Elevator), Previous(Previous)
	{
	}

	size_t Hash()
	{
		constexpr size_t size_t_bits = sizeof(size_t) * 8;
		constexpr size_t ull_bits = sizeof(unsigned long long) * 8;
		constexpr size_t required_bits = 4 * 2 * FloorCount + 8;

		static_assert(ull_bits >= required_bits, "unsigned long long can't hold enought bits!");
		static_assert(size_t_bits >= required_bits, "size_t can't hold enought bits!");

		std::bitset<size_t_bits> Hash;

		for (const Floor & Floor : Facility)
		{
			Hash <<= 4;
			Hash |= Floor.Chips.count();
			Hash <<= 4;
			Hash |= Floor.Generators.count();
		}

		Hash <<= 8;
		Hash |= static_cast<uint8_t>(Elevator);

		return static_cast<size_t>(Hash.to_ullong());
	}
};


Compound CreateCompund(const StringVectorVector & Lines);
uint64_t Backtrack(const Compound & Facility, std::shared_ptr<Configuration> & FinalConfig);
void PrintSteps(std::shared_ptr<Configuration> FinalConfig);

int main()
{
	std::shared_ptr<Configuration> FinalConfig;
	StringVectorVector Lines = GetFileLineParts("Input.txt");

	Compound Facility = CreateCompund(Lines);

#ifdef ParTwo 
	Facility[0].Generators.set(5);
	Facility[0].Chips.set(5);
	Facility[0].Generators.set(6);
	Facility[0].Chips.set(6);
#endif

	uint64_t Steps = Backtrack(Facility, FinalConfig);

	std::cout << "Steps: " << Steps << std::endl;
	PrintSteps(FinalConfig);

	system("pause");
}

Compound CreateCompund(const StringVectorVector & Lines)
{
	Compound Facility;
	size_t FloorIndex = 0;
	std::map<std::string, size_t> ComponendIndex;
	size_t NextIndex = 0;

	for (const StringVector & Line : Lines)
	{
		if (Line[4] != "nothing")
		{
			Floor & CurrentFloor = Facility[FloorIndex];
			for (StringVector::const_iterator It = Line.begin() + 5; It != Line.end();)
			{
				size_t Delimiter = It->find('-');
				std::string Element = It->substr(0, Delimiter);
				bool IsGenerator = (Delimiter == std::string::npos);

				auto Result = ComponendIndex.find(Element);
				size_t Index;
				if (Result == ComponendIndex.end())
				{
					Index = NextIndex++;
					ComponendIndex[Element] = Index;
				}
				else
				{
					Index = Result->second;
				}

				ComponentStatus & Component = IsGenerator ? CurrentFloor.Generators : CurrentFloor.Chips;
				Component.set(Index);

				It++;
				if ((*(It->rbegin())) == '.')
					break;
				It+= 2;
				if (*It == "a")
					It++;
			}
		}

		FloorIndex++;
	}

	return Facility;
}

bool IsValid(const Compound & Facility)
{
	size_t GeneratorCount = 0;
	size_t ChipCount = 0;

	for (const Floor & Fl : Facility)
	{
		for (int i = 0; i < ElementCount; i++)
		{
			if (Fl.Chips.test(i))
			{
				if (Fl.Generators.any() && !Fl.Generators.test(i))
				{
					return false;
				}
			}
		}

		GeneratorCount += Fl.Generators.count();
		ChipCount += Fl.Chips.count();
	}

	if ((GeneratorCount > ElementCount) || (ChipCount > ElementCount))
	{
		return false;
	}

	return true;

}

bool IsEndConfiguration(const Compound & Facility)
{
	if (!IsValid(Facility))
	{
		__debugbreak();
	}

	const Floor & LastFloor = Facility[Facility.size() - 1];

	return LastFloor.Chips.all() && LastFloor.Generators.all();
}

void AddToWorkQueue(const Compound & Facility, size_t Elevator, ConfigurationVector & WorkQueue, HashSet & Backtrack, std::shared_ptr<Configuration> Previous)
{
	Configuration NewConfig({ Facility, Elevator, Previous });

	auto Result = Backtrack.insert(NewConfig.Hash());

	if (Result.second)
	{
		WorkQueue.push_back(NewConfig);
	}
}

bool AnyUnprotectedChips(const Floor & Floor)
{
	if (Floor.Chips.any())
	{
		for (size_t ChipIndex = 0; ChipIndex < Floor.Chips.size(); ChipIndex++)
		{
			if (Floor.Chips.test(ChipIndex) && !Floor.Generators.test(ChipIndex))
			{
				return true;
			}
		}
	}

	return false;
}

void SingleChip(const Compound & Facility, size_t OldFloorIndex, size_t NewFloorIndex, ConfigurationVector & WorkQueue, HashSet & Backtrack, std::shared_ptr<Configuration> Previous)
{
	for (size_t ChipIndex = 0; ChipIndex < ElementCount; ChipIndex++)
	{
		if (!Facility[OldFloorIndex].Chips.test(ChipIndex))
			continue;

		Compound Copy(Facility);
		Copy[OldFloorIndex].Chips.reset(ChipIndex);
		Copy[NewFloorIndex].Chips.set(ChipIndex);

		if (IsValid(Copy))
			AddToWorkQueue(Copy, NewFloorIndex, WorkQueue, Backtrack, Previous);
	}
}

void DoubleChip(const Compound & Facility, size_t OldFloorIndex, size_t NewFloorIndex, ConfigurationVector & WorkQueue, HashSet & Backtrack, std::shared_ptr<Configuration> Previous)
{
	for (size_t ChipIndex = 0; ChipIndex < ElementCount; ChipIndex++)
	{
		if (!Facility[OldFloorIndex].Chips.test(ChipIndex))
			continue;

		Compound Copy(Facility);
		Copy[OldFloorIndex].Chips.reset(ChipIndex);
		Copy[NewFloorIndex].Chips.set(ChipIndex);

		SingleChip(Copy, OldFloorIndex, NewFloorIndex, WorkQueue, Backtrack, Previous);
	}
}

void SingleGenerator(const Compound & Facility, size_t OldFloorIndex, size_t NewFloorIndex, ConfigurationVector & WorkQueue, HashSet & Backtrack, std::shared_ptr<Configuration> Previous)
{
	for (size_t GeneratorIndex = 0; GeneratorIndex < ElementCount; GeneratorIndex++)
	{
		if (!Facility[OldFloorIndex].Generators.test(GeneratorIndex))
			continue;

		Compound Copy(Facility);
		Copy[OldFloorIndex].Generators.reset(GeneratorIndex);
		Copy[NewFloorIndex].Generators.set(GeneratorIndex);

		if (IsValid(Copy))
			AddToWorkQueue(Copy, NewFloorIndex, WorkQueue, Backtrack, Previous);
	}
}

void DoubleGenerator(const Compound & Facility, size_t OldFloorIndex, size_t NewFloorIndex, ConfigurationVector & WorkQueue, HashSet & Backtrack, std::shared_ptr<Configuration> Previous)
{
	for (size_t GeneratorIndex = 0; GeneratorIndex < ElementCount; GeneratorIndex++)
	{
		if (!Facility[OldFloorIndex].Generators.test(GeneratorIndex))
			continue;

		Compound Copy(Facility);
		Copy[OldFloorIndex].Generators.reset(GeneratorIndex);
		Copy[NewFloorIndex].Generators.set(GeneratorIndex);

		SingleGenerator(Copy, OldFloorIndex, NewFloorIndex, WorkQueue, Backtrack, Previous);
	}
}

void ChipAndGenerator(const Compound & Facility, size_t OldFloorIndex, size_t NewFloorIndex, ConfigurationVector & WorkQueue, HashSet & Backtrack, std::shared_ptr<Configuration> Previous)
{
	for (size_t ChipIndex = 0; ChipIndex < ElementCount; ChipIndex++)
	{
		if (!Facility[OldFloorIndex].Chips.test(ChipIndex))
			continue;

		Compound Copy(Facility);
		Copy[OldFloorIndex].Chips.reset(ChipIndex);
		Copy[NewFloorIndex].Chips.set(ChipIndex);

		SingleGenerator(Copy, OldFloorIndex, NewFloorIndex, WorkQueue, Backtrack, Previous);
	}
}


void EnumerateConfigurations(const Compound & Facility, size_t OldFloorIndex, size_t NewFloorIndex, ConfigurationVector & WorkQueue, HashSet & Backtrack, std::shared_ptr<Configuration> Previous)
{
	ChipAndGenerator(Facility, OldFloorIndex, NewFloorIndex, WorkQueue, Backtrack, Previous);
	DoubleChip(Facility, OldFloorIndex, NewFloorIndex, WorkQueue, Backtrack, Previous);
	DoubleGenerator(Facility, OldFloorIndex, NewFloorIndex, WorkQueue, Backtrack, Previous);
	SingleChip(Facility, OldFloorIndex, NewFloorIndex, WorkQueue, Backtrack, Previous);
	SingleGenerator(Facility, OldFloorIndex, NewFloorIndex, WorkQueue, Backtrack, Previous);
}

bool ConsumeConfiguration(const Configuration & Config, ConfigurationVector & WorkQueue, HashSet & Backtrack)
{
	if (!IsValid(Config.Facility))
	{
		__debugbreak();
	}

	if (IsEndConfiguration(Config.Facility))
	{
		return true;
	}

	std::shared_ptr<Configuration> Previous = std::make_shared<Configuration>(Config);

	if ((Config.Elevator + 1) < Config.Facility.size())
	{
		EnumerateConfigurations(Config.Facility, Config.Elevator, Config.Elevator + 1, WorkQueue, Backtrack, Previous);
	}

	if (Config.Elevator > 0)
	{
		EnumerateConfigurations(Config.Facility, Config.Elevator, Config.Elevator - 1, WorkQueue, Backtrack, Previous);
	}

	return false;
}

uint64_t Backtrack(const Compound & Facility, std::shared_ptr<Configuration> & FinalConfig)
{
	uint64_t Steps = 0;

	ConfigurationVector CurrentWorkQueue;
	ConfigurationVector NextWorkQueue;
	HashSet Backtrack;

	CurrentWorkQueue.emplace_back(Facility, 0, std::shared_ptr<Configuration>(nullptr));
	Backtrack.insert(CurrentWorkQueue.front().Hash());

	do {
		Steps++;

		std::cout << "Step " << Steps << " with " << CurrentWorkQueue.size() << " Jobs" << std::endl;

		for (const Configuration & Config : CurrentWorkQueue)
		{
			if (ConsumeConfiguration(Config, NextWorkQueue, Backtrack))
			{
				FinalConfig = std::make_shared<Configuration>(Config);
				return Steps - 1;
			}
		}

		std::swap(CurrentWorkQueue, NextWorkQueue);
		NextWorkQueue.clear();

	} while (!CurrentWorkQueue.empty());

	return 0;
}

void PrintSteps(std::shared_ptr<Configuration> FinalConfig)
{
	std::vector<std::shared_ptr<Configuration>> RHistory;
	std::shared_ptr<Configuration> Config = FinalConfig;

	while (Config)
	{
		RHistory.push_back(Config);
		Config = Config->Previous;
	}

	std::vector<std::shared_ptr<Configuration>> History(RHistory.rbegin(), RHistory.rend());
	
	for (std::shared_ptr<Configuration> Step : History)
	{
		for (size_t i = 1; i <= FloorCount; i++)
		{
			size_t FloorIndex = FloorCount - i;
			std::cout << FloorIndex << " ";
			if (FloorIndex == Step->Elevator)
				std::cout << "-> ";
			else
				std::cout << "   ";


			for (size_t Element = 0; Element < ElementCount; Element++)
			{
				std::cout << ((Step->Facility[FloorIndex].Chips.test(Element)) ? "C" : " ");
				std::cout << ((Step->Facility[FloorIndex].Generators.test(Element)) ? "#" : " ");
				std::cout << " ";
			}
			std::cout << std::endl;
		}

		std::cout << std::endl;
		getchar();
	}
}