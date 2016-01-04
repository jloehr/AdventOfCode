// Day24.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

typedef std::vector<size_t> IntVector;
typedef std::vector<IntVector::const_iterator> IntIterVector;
typedef std::function<bool(const IntIterVector&, const IntIterVector&)> IntIterVectorComparatorFunction;
typedef std::multiset<IntIterVector, IntIterVectorComparatorFunction> IntIterVectorSet;


static IntVector GetPackages(const std::string & InputFile);
static size_t GetQuantumEntanglementOfBestConfig();
static size_t GetBestQuantumEntanglemenForPackageCount(size_t PackageCount, size_t PassengerWeight);

static size_t CalculateTotalWeight();
static IntIterVectorSet GenerateConfigurationForPackageCount(size_t PackageCount, size_t PassengerWeight);
static void RecousiveGenerateConfigurationForPackageCount(size_t PackagesLeft, const size_t ConfigurationWeight, const size_t PassengerWeight, IntVector::const_iterator LastPackage, IntIterVector & CurrentConfiguration, IntIterVectorSet & Configurations);

static bool TryToFindEquilibrium(const size_t PassengerWeight, const IntIterVector & UsedPackages); 
static bool TryToFindEquilibriumRecoursive(const size_t PassengerWeight, IntIterVector::const_iterator UsedPackagesIter, IntIterVector::const_iterator UsedPackagesIterEnd, size_t Group2, size_t Group3, IntVector::const_iterator CurrentPackage);

static size_t CalculateQuantEnt(const IntIterVector & Packages);
static bool SortByQuantEnt(const IntIterVector & A, const IntIterVector & B);

static const IntVector Packages = GetPackages("Input.txt");

int main()
{
	size_t QuantumEntanglement = GetQuantumEntanglementOfBestConfig();

	std::cout << "Quantum Entanglement: " << QuantumEntanglement << std::endl;

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

static size_t GetQuantumEntanglementOfBestConfig()
{
	size_t BestQuantumEntanglement = SIZE_MAX;
	size_t PassengerWeight = CalculateTotalWeight() / 3;

	for (size_t PackageCountInPassengerCompartement = 1; (PackageCountInPassengerCompartement < Packages.size()) && (BestQuantumEntanglement == SIZE_MAX); PackageCountInPassengerCompartement++)
	{
		BestQuantumEntanglement = GetBestQuantumEntanglemenForPackageCount(PackageCountInPassengerCompartement, PassengerWeight);
	}

	return BestQuantumEntanglement;
}

static size_t GetBestQuantumEntanglemenForPackageCount(size_t PackageCount, size_t PassengerWeight)
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

		bool EquilibriumFound = TryToFindEquilibrium(ConfigWeight, Configuration);

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

static bool TryToFindEquilibrium(const size_t PassengerWeight, const IntIterVector & UsedPackages)
{
	return TryToFindEquilibriumRecoursive(PassengerWeight, UsedPackages.begin(), UsedPackages.end(), 0, 0, Packages.begin());
}

static bool TryToFindEquilibriumRecoursive(const size_t PassengerWeight, IntIterVector::const_iterator UsedPackagesIter, IntIterVector::const_iterator UsedPackagesIterEnd, size_t Group2, size_t Group3, IntVector::const_iterator CurrentPackage)
{
	if (CurrentPackage == Packages.end())
	{
		return (Group2 == Group3);
	}

	if ((Group2 > PassengerWeight) || (Group3 > PassengerWeight))
	{
		return false;
	}

	if ((UsedPackagesIter != UsedPackagesIterEnd) && (CurrentPackage == (*UsedPackagesIter)))
	{
		return TryToFindEquilibriumRecoursive(PassengerWeight, UsedPackagesIter + 1, UsedPackagesIterEnd, Group2, Group3, CurrentPackage + 1);
	}

	if (Group3 < Group2)
	{
		std::swap(Group2, Group3);
	}

	if(TryToFindEquilibriumRecoursive(PassengerWeight, UsedPackagesIter, UsedPackagesIterEnd, Group2 + (*CurrentPackage), Group3, CurrentPackage + 1))
	{
		return true;
	}

	return TryToFindEquilibriumRecoursive(PassengerWeight, UsedPackagesIter, UsedPackagesIterEnd, Group2, Group3 + (*CurrentPackage), CurrentPackage + 1);
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