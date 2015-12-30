// Day19.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

typedef std::pair<std::string, std::string> StringPair;
typedef std::vector<StringPair> StringPairVector;
typedef std::set<std::string> StringSet;
typedef std::map<std::string, size_t> StringIntMap;

static void ParseFile(std::string InputFile, StringPairVector & Replacements, std::string & MedicineMolecule);
static size_t GetDistinctMoleculeCountAfterReplacements(const StringPairVector & Replacements, const std::string & MedicineMolecule);
static size_t GetStepsToProduceMolecule(const StringPairVector & Replacements, const std::string & MedicineMolecule);

static const std::string Electron("e");

int main()
{
	StringPairVector Replacements;
	std::string MedicineMolecule;

	ParseFile("Input.txt", Replacements, MedicineMolecule);

	size_t MoleculeCount = GetDistinctMoleculeCountAfterReplacements(Replacements, MedicineMolecule);
	size_t StepsToProduce = GetStepsToProduceMolecule(Replacements, MedicineMolecule);

	std::cout << "Distinct Molecules: " << MoleculeCount << std::endl;
	std::cout << "Steps to produce: " << StepsToProduce << std::endl;

	system("pause");

    return 0;
}

static void ParseFile(std::string InputFile, StringPairVector & Replacements, std::string & MedicineMolecule)
{
	StringVectorVector InputParts = GetFileLineParts(InputFile);

	for (const StringVector & Line : InputParts)
	{
		if (Line.size() < 3)
		{
			break;
		}

		Replacements.push_back(std::make_pair(Line[0], Line[2]));
	}

	MedicineMolecule = InputParts[InputParts.size() - 1][0];
}

static size_t GetDistinctMoleculeCountAfterReplacements(const StringPairVector & Replacements, const std::string & MedicineMolecule)
{
	StringSet Molecules;
	std::string Buffer;
	Buffer.reserve(MedicineMolecule.size());

	for (const StringPair & Replacement : Replacements)
	{
		size_t ReplacementPosition = MedicineMolecule.find(Replacement.first);

		while(ReplacementPosition != MedicineMolecule.npos)
		{
			Buffer.assign(MedicineMolecule.begin(), MedicineMolecule.begin() + ReplacementPosition);
			Buffer.append(Replacement.second);
			Buffer.append(MedicineMolecule.begin() + ReplacementPosition + Replacement.first.size(), MedicineMolecule.end());

			Molecules.insert(Buffer);

			ReplacementPosition = MedicineMolecule.find(Replacement.first, ReplacementPosition + 1);
		}
	}

	return Molecules.size();
}

static bool IsLowerCase(char Character)
{
	return (Character >= 'a') && (Character <= 'z');
}

static size_t GetStepsToProduceMolecule(const StringPairVector & Replacements, const std::string & MedicineMolecule)
{
	StringIntMap Atoms;
	size_t AtomCount = 0;

	std::string::const_iterator AtomStart = MedicineMolecule.begin();

	for (std::string::const_iterator Character = MedicineMolecule.begin() + 1; Character != MedicineMolecule.end(); ++Character)
	{
		if (IsLowerCase(*Character))
		{
			continue;
		}

		std::string Name(AtomStart, Character);
		++Atoms[Name];
		++AtomCount;
		AtomStart = Character;
	}

	// Seems like "Rn" Atoms, "Ar" Atoms and "Y" Atoms can only be generated through specific Atoms
	// They also are not replaced by anything else. 
	// "Y" only appears inbetween "Rn" and "Ar" in combination with another "normal" Atom
	return AtomCount - Atoms["Rn"] - Atoms["Ar"] - (2 * Atoms["Y"]);
}