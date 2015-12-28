#pragma once

#include "stdafx.h"

typedef std::vector<std::string> StringVector;
typedef std::vector<StringVector> StringVectorVector;

StringVector GetFileLines(const std::string & InputFileName);
StringVectorVector GetFileLineParts(const std::string & InputFileName);