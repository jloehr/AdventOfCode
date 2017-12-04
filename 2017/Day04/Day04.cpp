// Day04.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int main()
{
	const StringVectorVector FileParts = GetFileLineParts("Input.txt");
	uint64_t ValidPassphrases = 0;

	for (const auto & Line : FileParts)
	{
		std::unordered_set<std::string> Words;
		bool Valid = true;

		for (const auto & Word : Line)
		{
			auto Result = Words.insert(Word);
			if (!Result.second)
			{
				Valid = false;
				break;
			}
		}

		if (Valid)
			ValidPassphrases++;
	}

	std::cout << "Valid passwords: " << ValidPassphrases << std::endl;

	system("pause");
    return 0;
}

