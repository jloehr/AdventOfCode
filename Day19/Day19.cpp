// Day19.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

typedef std::pair<std::string, std::string> StringPair;
typedef std::vector<StringPair> StringPairVector;
typedef std::set <std::string> StringSet;

static void ParseFile(std::string InputFile, StringPairVector & Replacements, std::string & MedicineMolecule);
static size_t GetDistinctMoleculeCountAfterReplacements(const StringPairVector & Replacements, const std::string & MedicineMolecule);


int main()
{
	StringPairVector Replacements;
	std::string MedicineMolecule;

	ParseFile("Input.txt", Replacements, MedicineMolecule);

	size_t MoleculeCount = GetDistinctMoleculeCountAfterReplacements(Replacements, MedicineMolecule);

	std::cout << "Distinct Molecules: " << MoleculeCount << std::endl;

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