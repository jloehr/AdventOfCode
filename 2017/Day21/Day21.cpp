// Day21.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

enum class Pixel {Off, On};

struct Grid
{
	Grid(size_t Size)
		:Data(Size * Size), Size(Size) {}

	Grid(const Grid& Other)
		:Data(Other.Data), Size(Other.Size) {}

	Grid(Grid&& Other)
		:Size(Other.Size)
	{
		std::swap(Data, Other.Data);
		Other.Size = 0;
	}

	Grid(const std::string & Input)
	{
		size_t Delim = Input.find('/');
		Size = Delim;
		Data.resize(Size * Size);
		auto It = std::begin(Input);

		for (size_t YPos = 0; YPos < Size; ++YPos)
		{
			for (size_t XPos = 0; XPos < Size; ++XPos)
				Set(XPos, YPos, ((*It++) == '.') ? Pixel::Off : Pixel::On);

			if(It != std::end(Input))
				std::advance(It, 1);
		}
	}

	Grid & operator= (const Grid & Other)
	{
		if (this != &Other)
		{
			Data = Other.Data;
			Size = Other.Size;
		}

		return *this;
	}

	Grid & operator= (Grid && Other)
	{
		if (this != &Other)
		{
			std::swap(Data, Other.Data);
			Size = Other.Size;

			Other.Size = 0;
			Other.Data.clear();
		}

		return *this;
	}

	bool operator== (const Grid & Other) const
	{
		return (Size == Other.Size) && (Data == Other.Data);
	}

	size_t GetSize() const { return Size; }

	size_t GetPixelCount() const
	{
		return std::count(std::begin(Data), std::end(Data), Pixel::On);
	}

	Pixel Get(size_t X, size_t Y) const
	{
		if (X >= Size || Y >= Size)
			return Pixel::Off;
		return Data[GetIndex(X, Y)];
	}

	void Set(size_t X, size_t Y, Pixel Value)
	{
		if (X >= Size || Y >= Size)
			return;

		Data[GetIndex(X, Y)] = Value;
	}

	Grid Read(size_t Size, size_t X, size_t Y) const
	{
		Grid New(Size);
		for (size_t YPos = 0; YPos < Size; ++YPos)
			for (size_t XPos = 0; XPos < Size; ++XPos)
				New.Set(XPos, YPos, Get(X + XPos, Y + YPos));

		return New;
	}

	void Write(const Grid & Other, size_t X, size_t Y)
	{
		for (size_t YPos = 0; YPos < Other.Size; ++YPos)
			for (size_t XPos = 0; XPos < Other.Size; ++XPos)
				Set(X + XPos, Y + YPos, Other.Get(XPos, YPos));
	}

	Grid Flipped()
	{
		Grid New(this->Size);

		for (size_t YPos = 0; YPos < Size; ++YPos)
			for (size_t XPos = 0; XPos < Size; ++XPos)
				New.Set(Size - XPos - 1, YPos, Get(XPos, YPos));

		return New;
	}

	Grid Rotated()
	{
		Grid New(this->Size);

		for (size_t YPos = 0; YPos < Size; ++YPos)
			for (size_t XPos = 0; XPos < Size; ++XPos)
				New.Set(YPos, Size - XPos - 1, Get(XPos, YPos));

		return New;
	}

	void Print() const
	{
		for (size_t YPos = 0; YPos < Size; ++YPos)
		{
			for (size_t XPos = 0; XPos < Size; ++XPos)
				std::cout << ((Get(XPos, YPos) == Pixel::On) ? '#' : '.');

			if(YPos != (Size - 1))
				std::cout << '/';
		}

	}

private:
	std::vector<Pixel> Data;
	size_t Size;

	size_t GetIndex(size_t X, size_t Y) const
	{
		return Y * Size + X;
	}
};
struct GridHash
{
	size_t operator()(const Grid & Grid) const noexcept
	{
		size_t Hash = 0;

		for (size_t YPos = 0; YPos < Grid.GetSize(); ++YPos)
			for (size_t XPos = 0; XPos < Grid.GetSize(); ++XPos)
			{
				Hash |= (Grid.Get(XPos, YPos) == Pixel::Off) ? 0 : 1;
				Hash <<= 1;
			}

		return Hash;
	}
};

Grid Iterate(const Grid & Image, const std::unordered_map<Grid, Grid, GridHash> & Rules, size_t SubImageSize)
{
	size_t SubImageCount = Image.GetSize() / SubImageSize;
	Grid New(SubImageCount * (SubImageSize + 1));

	for (size_t Y = 0; Y < SubImageCount; ++Y)
		for (size_t X = 0; X < SubImageCount; ++X)
		{
			Grid Tile = Image.Read(SubImageSize, X * SubImageSize, Y * SubImageSize);
			auto Result = Rules.find(Tile);
			if (Result == std::end(Rules))
				std::cout << "Panic! Subtile not found in Rules." << std::endl;

			New.Write(Result->second, X * (SubImageSize + 1), Y * (SubImageSize + 1));
		}

	return New;
}

int main()
{
	const StringVectorVector Lines = GetFileLineParts("Input.txt");
	std::unordered_map<Grid, Grid, GridHash> Rules;
	
	for (const auto & Line : Lines)
	{
		Grid Rule(Line[0]);
		Grid Flipped = Rule.Flipped();
		Grid Replacement(Line[2]);

		Rules.insert(std::make_pair(Rule, Replacement));
		Rules.insert(std::make_pair(Flipped, Replacement));

		for (auto i = 0; i < 4; ++i)
		{
			Rule = Rule.Rotated();
			Flipped = Rule.Flipped();
			Rules.insert(std::make_pair(Rule, Replacement));
			Rules.insert(std::make_pair(Flipped, Replacement));
		}
	}
	
	Grid Image(".#./..#/###");
	for (auto Interation = 1; Interation <= 18; ++Interation)
	{
		Image = Iterate(Image, Rules, ((Image.GetSize() % 2) == 0) ? 2 : 3);
		std::cout << "Iteration " << Interation << " Pixel count: " << Image.GetPixelCount() << std::endl;
	}

	system("pause");
    return 0;
}

