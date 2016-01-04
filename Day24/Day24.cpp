// Day24.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

typedef std::vector<size_t> IntVector;
typedef std::vector<IntVector::const_iterator> IntIterVector;
typedef std::function<bool(const IntIterVector&, const IntIterVector&)> IntIterVectorComparatorFunction;
typedef std::multiset<IntIterVector, IntIterVectorComparatorFunction> IntIterVectorSet;


static IntVector GetPackages(const std::string & InputFile);
static size_t GetQuantumEntanglementOfBestConfig(const size_t Groups);
static size_t GetBestQuantumEntanglemenForPackageCount(const size_t PackageCount, const size_t PassengerWeight, const size_t Groups);

static size_t CalculateTotalWeight();
static IntIterVectorSet GenerateConfigurationForPackageCount(size_t PackageCount, size_t PassengerWeight);
static void RecousiveGenerateConfigurationForPackageCount(size_t PackagesLeft, const size_t ConfigurationWeight, const size_t PassengerWeight, IntVector::const_iterator LastPackage, IntIterVector & CurrentConfiguration, IntIterVectorSet & Configurations);

static bool TryToFindEquilibrium(const size_t PassengerWeight, const IntIterVector & UsedPackages, const size_t Groups);
static bool TryToFindEquilibriumRecoursive(const size_t PassengerWeight, IntIterVector::const_iterator UsedPackagesIter, IntIterVector::const_iterator UsedPackagesIterEnd, IntVector & OtherGroups, IntVector::const_iterator CurrentPackage);

static size_t CalculateQuantEnt(const IntIterVector & Packages);
static bool SortByQuantEnt(const IntIterVector & A, const IntIterVector & B);

static bool AllTheSame(const IntVector & Groups);
static bool AnyGreaterThan(const IntVector & Groups, const size_t Max);

static const IntVector Packages = GetPackages("Input.txt");

int main()
{
	size_t QuantumEntanglement = GetQuantumEntanglementOfBestConfig(3);
	std::cout << "Quantum Entanglement (Part One): " << QuantumEntanglement << std::endl;

	QuantumEntanglement = GetQuantumEntanglementOfBestConfig(4);
	std::cout << "Quantum Entanglement (Part Two): " << QuantumEntanglement << std::endl;

	system("pause");

    return 0;
}

static IntVector GetPackages(const std::string & InputFile)
{
	StringVector Input = GetFileLines(InputFile);
	IntVector Packages;
	Packages.reserve(Input.size());

	for (const std::string & Line : Input)
	{
		Packages.push_back(std::atoi(Line.c_str()));
	}

	return IntVector(Packages.rbegin(), Packages.rend());
}

static size_t GetQuantumEntanglementOfBestConfig(const size_t Groups)
{
	size_t BestQuantumEntanglement = SIZE_MAX;
	size_t PassengerWeight = CalculateTotalWeight() / Groups;

	for (size_t PackageCountInPassengerCompartement = 1; (PackageCountInPassengerCompartement < Packages.size()) && (BestQuantumEntanglement == SIZE_MAX); PackageCountInPassengerCompartement++)
	{
		BestQuantumEntanglement = GetBestQuantumEntanglemenForPackageCount(PackageCountInPassengerCompartement, PassengerWeight, Groups - 1);
	}

	return BestQuantumEntanglement;
}

static size_t GetBestQuantumEntanglemenForPackageCount(const size_t PackageCount, const size_t PassengerWeight, const size_t Groups)
{
	IntIterVectorSet AllGroupOneConfigurations = GenerateConfigurationForPackageCount(PackageCount, PassengerWeight);

	size_t BestQuantEnt = SIZE_MAX;

	for (const IntIterVector & Configuration : AllGroupOneConfigurations)
	{
		size_t ConfigWeight = 0;

		for (auto & Number : Configuration)
		{
			ConfigWeight += (*Number);
		}

		bool EquilibriumFound = TryToFindEquilibrium(ConfigWeight, Configuration, Groups);

		if (EquilibriumFound)
		{
			return CalculateQuantEnt(Configuration);
		}
	}

	return SIZE_MAX;
}

static size_t CalculateTotalWeight()
{
	size_t Weight = 0;

	for (const size_t & Package : Packages)
	{
		Weight += Package;
	}

	return Weight;
}

static IntIterVectorSet GenerateConfigurationForPackageCount(size_t PackageCount, size_t PassengerWeight)
{
	if (PackageCount == 0)
	{
		return IntIterVectorSet();
	}

	IntIterVectorSet Configurations(SortByQuantEnt);

	for (IntVector::const_iterator FirstPackage = Packages.begin(); (FirstPackage + (PackageCount - 1)) != Packages.end(); ++FirstPackage)
	{
		IntIterVector NewConfiguration({ FirstPackage });
		RecousiveGenerateConfigurationForPackageCount(PackageCount - 1, (*FirstPackage), PassengerWeight, FirstPackage, NewConfiguration, Configurations);
	}

	return Configurations;
}

static void RecousiveGenerateConfigurationForPackageCount(size_t PackagesLeft, const size_t ConfigurationWeight, const size_t PassengerWeight, IntVector::const_iterator LastPackage, IntIterVector & CurrentConfiguration, IntIterVectorSet & Configurations)
{
	if (ConfigurationWeight > PassengerWeight)
	{
		return;
	}

	if (PackagesLeft == 0)
	{
		if (ConfigurationWeight == PassengerWeight)
		{
			Configurations.insert(CurrentConfiguration);
		}
		return;
	}

	for (IntVector::const_iterator NextPackage = LastPackage + 1; (NextPackage + (PackagesLeft - 1)) != Packages.end(); ++NextPackage)
	{
		CurrentConfiguration.push_back(NextPackage);
		RecousiveGenerateConfigurationForPackageCount(PackagesLeft - 1, ConfigurationWeight + (*NextPackage), PassengerWeight, NextPackage, CurrentConfiguration, Configurations);
		CurrentConfiguration.pop_back();
	}
}

static bool TryToFindEquilibrium(const size_t PassengerWeight, const IntIterVector & UsedPackages, const size_t Groups)
{
	IntVector OtherGroups(Groups, 0);
	return TryToFindEquilibriumRecoursive(PassengerWeight, UsedPackages.begin(), UsedPackages.end(), OtherGroups, Packages.begin());
}

static bool TryToFindEquilibriumRecoursive(const size_t PassengerWeight, IntIterVector::const_iterator UsedPackagesIter, IntIterVector::const_iterator UsedPackagesIterEnd, IntVector & OtherGroups, IntVector::const_iterator CurrentPackage)
{
	if (CurrentPackage == Packages.end())
	{
		return AllTheSame(OtherGroups);
	}

	if (AnyGreaterThan(OtherGroups, PassengerWeight))
	{
		return false;
	}

	if ((UsedPackagesIter != UsedPackagesIterEnd) && (CurrentPackage == (*UsedPackagesIter)))
	{
		return TryToFindEquilibriumRecoursive(PassengerWeight, UsedPackagesIter + 1, UsedPackagesIterEnd, OtherGroups, CurrentPackage + 1);
	}

	for (IntVector::iterator Group = OtherGroups.begin(); Group != OtherGroups.end(); ++Group)
	{
		(*Group) += (*CurrentPackage);
		if (TryToFindEquilibriumRecoursive(PassengerWeight, UsedPackagesIter, UsedPackagesIterEnd, OtherGroups, CurrentPackage + 1))
		{
			return true;
		}	
		(*Group) -= (*CurrentPackage);
	}

	return false;
}

static size_t CalculateQuantEnt(const IntIterVector & Packages)
{
	size_t QuantEnt = 1;

	for (auto & Number : Packages)
	{
		QuantEnt *= (*Number);
	}

	return QuantEnt;
}

static bool SortByQuantEnt(const IntIterVector & A, const IntIterVector & B)
{
	return (CalculateQuantEnt(A) < CalculateQuantEnt(B));
}

static bool AllTheSame(const IntVector & Groups)
{
	for (IntVector::const_iterator Group = Groups.begin(); (Group + 1) != Groups.end(); ++Group)
	{
		if ((*Group) != (*(Group + 1)))
		{
			return false;
		}
	}

	return true;
}

static bool AnyGreaterThan(const IntVector & Groups, const size_t Max)
{
	for (size_t Value : Groups)
	{
		if (Value > Max)
		{
			return true;
		}
	}

	return false;
}