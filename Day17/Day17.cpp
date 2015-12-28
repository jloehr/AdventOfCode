// Day17.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

typedef std::vector<size_t> UIntVector;

static UIntVector GetSortedBuckets(const std::string & FileName);
static UIntVector GetRemainingBucketSizes(const UIntVector & Buckets);
static size_t GetAmountOfCombinations(const UIntVector & Buckets, const UIntVector & RemainingBucketSize, size_t LitersOfEggnog);
static size_t RecoursiveAmountOfCombinations(const UIntVector & Buckets, const UIntVector & RemainingBucketSize, size_t LitersOfEggnogLeft, size_t BucketIndex);

int main()
{
	UIntVector Buckets = GetSortedBuckets("Input.txt");
	UIntVector RemainingBucketSize = GetRemainingBucketSizes(Buckets);

	std::cout << "Amount of Combinations: " << GetAmountOfCombinations(Buckets, RemainingBucketSize, 150) << std::endl;

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

static size_t GetAmountOfCombinations(const UIntVector & Buckets, const UIntVector & RemainingBucketSize, size_t LitersOfEggnog)
{
	return RecoursiveAmountOfCombinations(Buckets, RemainingBucketSize, LitersOfEggnog, 0);
}

static size_t RecoursiveAmountOfCombinations(const UIntVector & Buckets, const UIntVector & RemainingBucketSize, size_t LitersOfEggnogLeft, size_t BucketIndex)
{
	if (LitersOfEggnogLeft == 0)
	{
		return 1;
	}

	if ((BucketIndex >= Buckets.size()) || (LitersOfEggnogLeft > RemainingBucketSize[BucketIndex]))
	{
		return 0;
	}

	size_t AmountOfCombinations = RecoursiveAmountOfCombinations(Buckets, RemainingBucketSize, LitersOfEggnogLeft, BucketIndex + 1);

	if (LitersOfEggnogLeft >= Buckets[BucketIndex])
	{
		AmountOfCombinations += RecoursiveAmountOfCombinations(Buckets, RemainingBucketSize, LitersOfEggnogLeft - Buckets[BucketIndex], BucketIndex + 1);
	}
	
	return AmountOfCombinations;
}