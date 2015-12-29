// Day17.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

typedef std::vector<size_t> UIntVector;

static UIntVector GetSortedBuckets(const std::string & FileName);
static UIntVector GetRemainingBucketSizes(const UIntVector & Buckets);
static size_t GetCountOfCombinations(const UIntVector & Buckets, const UIntVector & RemainingBucketSize, size_t LitersOfEggnog, size_t & MinBucketCount, size_t & MinBucketCountCombinations);
static size_t RecoursiveCountOfCombinations(const UIntVector & Buckets, const UIntVector & RemainingBucketSize, size_t LitersOfEggnogLeft, size_t BucketIndex, size_t BucketCount, size_t & MinBucketCount, size_t & MinBucketCountCombinations);

int main()
{
	UIntVector Buckets = GetSortedBuckets("Input.txt");
	UIntVector RemainingBucketSize = GetRemainingBucketSizes(Buckets);
	
	size_t MinBucketCount;
	size_t MinBucketCountCombinations;
	size_t CountOfOverallCombinations = GetCountOfCombinations(Buckets, RemainingBucketSize, 150, MinBucketCount, MinBucketCountCombinations);

	std::cout << "Overall Combinations: " << CountOfOverallCombinations << std::endl;
	std::cout << "Combinations for " << MinBucketCount << " Buckets: " << MinBucketCountCombinations << std::endl;

	system("pause");

    return 0;
}


static UIntVector GetSortedBuckets(const std::string & FileName)
{
	StringVector InputLines = GetFileLines(FileName);
	UIntVector Buckets;

	for (const std::string & Line : InputLines)
	{
		Buckets.push_back(std::atoi(Line.c_str()));
	}

	std::sort<UIntVector::iterator>(Buckets.begin(), Buckets.end(), std::greater<size_t>());

	return Buckets;
}

static UIntVector GetRemainingBucketSizes(const UIntVector & Buckets)
{
	UIntVector RemainingBucketSize(Buckets.size());

	size_t AccumulatedBucketSize = 0;

	UIntVector::const_reverse_iterator Bucket = Buckets.rbegin();
	UIntVector::reverse_iterator RemainingSize = RemainingBucketSize.rbegin();
	for (;Bucket != Buckets.rend(); ++Bucket, ++RemainingSize)
	{
		AccumulatedBucketSize += (*Bucket);
		(*RemainingSize) = AccumulatedBucketSize;
	}

	return RemainingBucketSize;
}

static size_t GetCountOfCombinations(const UIntVector & Buckets, const UIntVector & RemainingBucketSize, size_t LitersOfEggnog, size_t & MinBucketCount, size_t & MinBucketCountCombinations)
{
	MinBucketCount = SIZE_MAX;
	MinBucketCountCombinations = 0;

	return RecoursiveCountOfCombinations(Buckets, RemainingBucketSize, LitersOfEggnog, 0, 0, MinBucketCount, MinBucketCountCombinations);
}

static size_t RecoursiveCountOfCombinations(const UIntVector & Buckets, const UIntVector & RemainingBucketSize, size_t LitersOfEggnogLeft, size_t BucketIndex, size_t BucketCount, size_t & MinBucketCount, size_t & MinBucketCountCombinations)
{
	if (LitersOfEggnogLeft == 0)
	{
		if (BucketCount < MinBucketCount)
		{
			MinBucketCount = BucketCount;
			MinBucketCountCombinations = 1;
		}
		else if(BucketCount == MinBucketCount)
		{
			MinBucketCountCombinations++;
		}

		return 1;
	}

	if ((BucketIndex >= Buckets.size()) || (LitersOfEggnogLeft > RemainingBucketSize[BucketIndex]))
	{
		return 0;
	}

	size_t CountOfCombinations = RecoursiveCountOfCombinations(Buckets, RemainingBucketSize, LitersOfEggnogLeft, BucketIndex + 1, BucketCount, MinBucketCount, MinBucketCountCombinations);

	if (LitersOfEggnogLeft >= Buckets[BucketIndex])
	{
		CountOfCombinations += RecoursiveCountOfCombinations(Buckets, RemainingBucketSize, LitersOfEggnogLeft - Buckets[BucketIndex], BucketIndex + 1, BucketCount + 1, MinBucketCount, MinBucketCountCombinations);
	}
	
	return CountOfCombinations;
}