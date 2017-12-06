#pragma once

#include "stdafx.h"

#include "MD5.h"

typedef std::vector<std::string> StringVector;
typedef std::vector<StringVector> StringVectorVector;

StringVector GetFileLines(const std::string & InputFileName);
StringVectorVector GetFileLineParts(const std::string & InputFileName);

template<typename T>
std::vector<T> GetValues(const std::string & InputFileName)
{
	const StringVector Lines = GetFileLines(InputFileName);
	std::vector<T> Values;

	for (const std::string & Line : Lines)
	{
		std::istringstream LineStream(Line);
		Values.insert(Values.end(), std::istream_iterator<T>(LineStream), std::istream_iterator<T>());
	}

	return Values;
}

template<typename T>
std::vector<std::vector<T>> GetLineValues(const std::string & InputFileName)
{
	const StringVector Lines = GetFileLines(InputFileName);
	std::vector<std::vector<T>> ValueLines;

	for (const std::string & Line : Lines)
	{
		std::istringstream LineStream(Line);
		ValueLines.emplace_back(std::istream_iterator<T>(LineStream), std::istream_iterator<T>());
	}

	return ValueLines;
}
