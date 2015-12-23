// Day10.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

static void AppendCurrentCharacter(std::string & Buffer, char CurrentCharacter, uint32_t AmountOfCurrentCharacter);

int main()
{
	const size_t Steps = 40;
	std::string Input("3113322113");
	std::cout << Input << std::endl;

	for (size_t i = 0; i < Steps; i++)
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

		std::cout << i << std::endl;
	}

	std::cout << "Length: " << Input.size() << std::endl;

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