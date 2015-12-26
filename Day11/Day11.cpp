// Day11.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Password.h"

int main()
{
	Password<8> SantasPassword(std::string("hepxcrrq"));

	std::cout << SantasPassword << std::endl;

	++SantasPassword;

	std::cout << SantasPassword << std::endl;

	system("pause");

    return 0;
}