// Day07.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

bool IsSupportingTLS(const std::string & IP);
bool ContainsABBA(const std::string & Sequence);

int main()
{
	StringVector Lines = GetFileLines("Input.txt");
	unsigned TLSSupportCount = 0;

	for (const std::string & Line : Lines)
	{
		if (IsSupportingTLS(Line))
		{
			TLSSupportCount++;
		}
	}

	std::cout << "Supported IPs: " << TLSSupportCount << std::endl;

	system("pause");
}

bool IsSupportingTLS(const std::string & IP)
{
	size_t Start = 0;
	size_t End = 0;
	bool IsHypernet = false;
	bool ABBAFound = false;

	while(End != std::string::npos)
	{
		Start = End;
		End = IP.find(IsHypernet ? ']' : '[', Start);
		size_t Count = (End == std::string::npos) ? std::string::npos : End - Start;

		if (IsHypernet)
		{
			if (ContainsABBA(IP.substr(Start, Count)))
			{
				return false;
			}
		}
		else
		{
			if (!ABBAFound)
			{
				ABBAFound = ContainsABBA(IP.substr(Start, Count));
			}
		}

		IsHypernet = !IsHypernet;
		End = (End == std::string::npos) ? std::string::npos : End + 1;
	}

	return ABBAFound;
}

bool ContainsABBA(const std::string & Sequence)
{
	for (std::string::const_iterator It = Sequence.begin(); It != (Sequence.end() - 3); It++)
	{
		if ((*It) == (*(It + 3)))
		{
			if ((*(It + 1)) == (*(It + 2)) && ((*It) != (*(It + 1))))
			{
				return true;
			}
		}
	}

	return false;
}