// Day04.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <regex>
#include <unordered_set>
#include <unordered_map>
#include <functional>

#include "../../Common/Common.h"

std::unordered_set<std::string> ValidEyeColors = { "amb", "blu", "brn", "gry", "grn", "hzl", "oth" };

int main()
{
	std::regex FieldMatching("([a-z]{3}):(\\S+)", std::regex_constants::optimize);
	StringVector Lines = GetFileLines("Input.txt");
	Lines.push_back(""); // Add empty line, so processing will finish last passport

	size_t FieldCount = 0;
	size_t ValidFields = 0;
	size_t PartOne = 0;
	size_t PartTwo = 0;

	std::unordered_map <std::string, std::function<bool(const std::string&)>> FieldVerifier = {
		{"byr", [](const std::string& Value) -> bool { size_t year = std::stoull(Value); return ((year >= 1920) && (year <= 2002)); }},
		{"iyr", [](const std::string& Value) -> bool { size_t year = std::stoull(Value); return ((year >= 2010) && (year <= 2020)); }},
		{"eyr", [](const std::string& Value) -> bool { size_t year = std::stoull(Value); return ((year >= 2020) && (year <= 2030)); }},
		{"hgt", [](const std::string& Value) -> bool { 
			if (Value.size() < 3) return false;
			size_t height = std::stoull(Value);
			const std::string Unit = Value.substr(Value.size() - 2);
			if (Unit == "cm")
				return ((height >= 150) && (height <= 193));
			else if (Unit == "in")
				return ((height >= 59) && (height <= 76));
			else
				return false;
		}},
		{"hcl", [](const std::string& Value) -> bool { std::regex ColorMatching("#[a-z0-9]{6}"); return std::regex_match(Value, ColorMatching); }},
		{"ecl", [](const std::string& Value) -> bool { return (ValidEyeColors.find(Value) != ValidEyeColors.cend()); }},
		{"pid", [](const std::string& Value) -> bool { std::regex IdMatching("\\d{9}"); return std::regex_match(Value, IdMatching); }},
	};

	for (const std::string& Line : Lines)
	{
		if (Line.size() > 0)
		{
			for (std::sregex_iterator Match = std::sregex_iterator(Line.cbegin(), Line.cend(), FieldMatching); Match != std::sregex_iterator(); Match++)
			{
				const auto Verifier = FieldVerifier.find((*Match)[1]);
				if (Verifier != FieldVerifier.cend())
				{
					FieldCount++;
					if (Verifier->second((*Match)[2]))
						ValidFields++;
				}
			}
		}
		else
		{
			if (FieldCount == 7)
				PartOne++;
			if (ValidFields == 7)
				PartTwo++;

			FieldCount = 0; 
			ValidFields = 0;
		}
	}

	std::cout << "Part One:" << PartOne << std::endl;
	std::cout << "Part Two:" << PartTwo << std::endl;
}
