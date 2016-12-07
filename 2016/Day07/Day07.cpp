// Day07.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

typedef std::set<uint16_t> UIntSet;

void CheckSupportingProtocols(const std::string & IP, bool & TLS, bool & SSL);
bool ContainsABBA(const std::string & Sequence);
bool FindABA(const std::string & Sequence, std::unique_ptr<UIntSet> & ActiveSet, std::unique_ptr<UIntSet> & PassiveSet, bool Invert);
uint16_t MakeUInt(char A, char B, bool Invert);

int main()
{
	StringVector Lines = GetFileLines("Input.txt");
	unsigned TLSSupportCount = 0;
	unsigned SSLSupportCount = 0;

	for (const std::string & Line : Lines)
	{
		bool TLS = false;
		bool SSL = false;

		CheckSupportingProtocols(Line, TLS, SSL);

		TLSSupportCount += TLS ? 1 : 0;
		SSLSupportCount += SSL ? 1 : 0;
	}

	std::cout << "TLS IPs: " << TLSSupportCount << std::endl;
	std::cout << "SSL IPs: " << SSLSupportCount << std::endl;

	system("pause");
}

void CheckSupportingProtocols(const std::string & IP, bool & TLS, bool & SSL)
{
	TLS = false;
	SSL = false;

	size_t Start = 0;
	size_t End = 0;

	bool IsHypernet = false;
	bool TLSFailed = false;

	std::unique_ptr<UIntSet> ActiveSet = std::make_unique<UIntSet>();
	std::unique_ptr<UIntSet> PassiveSet = std::make_unique<UIntSet>();

	while((End != std::string::npos) && (!TLSFailed || !SSL))
	{
		Start = End;
		End = IP.find(IsHypernet ? ']' : '[', Start);
		size_t Count = (End == std::string::npos) ? std::string::npos : End - Start;
		const std::string Sequence = IP.substr(Start, Count);

		if (IsHypernet)
		{
			if (!TLSFailed && ContainsABBA(Sequence))
			{
				TLSFailed = true;
				TLS = false;
			}
		}
		else
		{
			if (!TLSFailed && !TLS)
			{
				TLS = ContainsABBA(Sequence);
			}
		}
	
		if (!SSL)
		{
			SSL = FindABA(Sequence, ActiveSet, PassiveSet, IsHypernet);
		}
		
		std::swap(ActiveSet, PassiveSet);
		IsHypernet = !IsHypernet;
		End = (End == std::string::npos) ? std::string::npos : End + 1;
	}
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

bool FindABA(const std::string & Sequence, std::unique_ptr<UIntSet> & ActiveSet, std::unique_ptr<UIntSet> & PassiveSet, bool Invert)
{
	for (std::string::const_iterator It = Sequence.begin(); It != (Sequence.end() - 2); It++)
	{
		if ((*It) == (*(It + 2)))
		{
			if ((*It) != (*(It + 1)))
			{
				uint16_t BitSequence = MakeUInt(*It, *(It + 1), Invert);

				auto Result = PassiveSet->find(BitSequence);

				if (Result != PassiveSet->end())
				{
					return true;
				}
				else
				{
					ActiveSet->insert(BitSequence);
				}
			}
		}

	}

	return false;
}

uint16_t MakeUInt(char A, char B, bool Invert)
{
	if (Invert)
	{
		std::swap(A, B);
	}
	
	return (0xFFFF & ((B << 8) | A));
}
