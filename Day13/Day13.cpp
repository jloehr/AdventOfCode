// Day13.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

typedef std::map<std::string, int32_t> StringIntMap;
typedef std::map<std::string, StringIntMap> StringMap;

static void ParseLine(const std::string & Line, StringMap & People, StringMap & ConnectionBuffer);
static int32_t GetBestSeating(const StringMap & People);
static int32_t RecursiveSeating(const StringMap & People, const std::string & StartPerson, const StringIntMap & CurrentPerson, std::set<std::string> & SeatedPeople, int32_t CurrentHappiness);


int main()
{
	std::ifstream Input("Input.txt");

	if (!Input.is_open())
	{
		std::cout << "Error opening File!" << std::endl;
		return 0;
	}

	std::string Line;

	StringMap People;
	StringMap ConnectionBuffer;

	while (std::getline(Input, Line))
	{
		ParseLine(Line, People, ConnectionBuffer);
	}


	Input.close();

	std::cout << "Happiness: " << GetBestSeating(People) << std::endl;

	system("pause");

    return 0;
}

static void ParseLine(const std::string & Line, StringMap & People, StringMap & ConnectionBuffer)
{
	std::stringstream LineStream(Line);
	std::vector<std::string> LineParts;
	std::string Buffer;

	while (std::getline(LineStream, Buffer, ' '))
	{
		LineParts.push_back(Buffer);
	}
	
	std::string Person = LineParts[0];
	std::string Partner = LineParts[10].substr(0, LineParts[10].size() - 1);

	int32_t HappinessValue = std::atoi(LineParts[3].c_str());

	if (LineParts[2] == "lose")
	{
		HappinessValue *= -1;
	}

	StringIntMap & PersonsHappinessRelations = ConnectionBuffer[Person];
	PersonsHappinessRelations.insert(std::make_pair(Partner, HappinessValue));

	StringIntMap & PartnerHappinessRelations = ConnectionBuffer[Partner];
	auto PartnerHappiness = PartnerHappinessRelations.find(Person);
	if (PartnerHappiness != PartnerHappinessRelations.end())
	{
		int32_t CoupleHappiness = HappinessValue + PartnerHappiness->second;
		People[Partner].insert(std::make_pair(Person, CoupleHappiness));
		People[Person].insert(std::make_pair(Partner, CoupleHappiness));
	}
}

static int32_t GetBestSeating(const StringMap & People)
{
	auto Start = People.begin();
	return RecursiveSeating(People, Start->first, Start->second, std::set<std::string>({ Start->first }), 0);
}

static int32_t RecursiveSeating(const StringMap & People, const std::string & StartPerson, const StringIntMap & CurrentPerson, std::set<std::string> & SeatedPeople, int32_t CurrentHappiness)
{
	// If all are seated, get Happines between last and first seated and abort recoursion
	if (SeatedPeople.size() == People.size())
	{
		return (CurrentHappiness + CurrentPerson.at(StartPerson));
	}

	int32_t BestHappiness = INT32_MIN;

	for (const auto & Connection : CurrentPerson)
	{
		// Check if person is on Table
		if (SeatedPeople.find(Connection.first) != SeatedPeople.end())
		{
			continue;
		}

		// Seat him
		SeatedPeople.insert(Connection.first);

		// Make Recoursive Call
		int32_t OverallHappiness = RecursiveSeating(People, StartPerson, People.at(Connection.first), SeatedPeople, CurrentHappiness + Connection.second);

		// Unseat him
		SeatedPeople.erase(Connection.first);

		if (BestHappiness < OverallHappiness)
		{
			BestHappiness = OverallHappiness;
		}
	}

	return BestHappiness;
}