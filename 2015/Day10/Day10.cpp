// Day10.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

static void AppendCurrentCharacter(std::string & Buffer, char CurrentCharacter, uint32_t AmountOfCurrentCharacter);

int main()
{
	const std::vector<size_t> Steps = { 40, 50 };
	auto CurrentStep = Steps.begin();
	std::string Input("3113322113");
	std::cout << Input << std::endl;

	for (size_t i = 1; i <= *(Steps.end() - 1); i++)
	{
		std::string Buffer;

		char CurrentCharacter = 0;
		uint32_t AmountOfCurrentCharacter = 0;

		for (char Character : Input)
		{
			if (Character != CurrentCharacter)
			{
				AppendCurrentCharacter(Buffer, CurrentCharacter, AmountOfCurrentCharacter);

				CurrentCharacter = Character;
				AmountOfCurrentCharacter = 1;
			}
			else
			{
				AmountOfCurrentCharacter++;
			}
		}

		AppendCurrentCharacter(Buffer, CurrentCharacter, AmountOfCurrentCharacter);

		Input.swap(Buffer);

		if ((CurrentStep != Steps.end()) && (i == *CurrentStep))
		{
			std::cout << "Length(" << i << "): " << Input.size() << std::endl;
			CurrentStep++;
		}

		std::cout << i << std::endl;
	}

	system("pause");

    return 0;
}

static void AppendCurrentCharacter(std::string & Buffer, char CurrentCharacter, uint32_t AmountOfCurrentCharacter)
{
	if (AmountOfCurrentCharacter != 0)
	{
		std::stringstream Converter;
		Converter << AmountOfCurrentCharacter;
		Buffer += Converter.str();
		Buffer += CurrentCharacter;
	}
}