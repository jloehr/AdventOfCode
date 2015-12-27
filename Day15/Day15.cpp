// Day15.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

typedef std::array<int64_t, 5> Ingredient;
typedef std::array<Ingredient, 4> IngredientArray;

static uint64_t GetBestScore(const IngredientArray & Ingredients);
static uint64_t RecoursiceRecipe(const IngredientArray & Ingredients, size_t IngredientIndex, size_t TablespoonsLeft, Ingredient & AccumulatedIngredientScore);


int main()
{
	std::ifstream Input("Input.txt");

	if (!Input.is_open())
	{
		std::cout << "Error opening File!" << std::endl;
		return 0;
	}

	std::string Line;
	IngredientArray Ingredients;
	size_t IngredientIndex = 0;

	while (std::getline(Input, Line))
	{
		static const Ingredient InputMapping = { 2, 4, 6, 8, 10 };

		std::stringstream InputStream(Line);
		std::vector<std::string> InputParts;
		std::string Buffer;

		while (std::getline(InputStream, Buffer, ' '))
		{
			InputParts.push_back(Buffer);
		}

		for (size_t i = 0; i < InputMapping.size(); i++)
		{
			std::string & InputPart = InputParts[InputMapping[i]];
			std::string & ParsingString = (i == (InputMapping.size() - 1)) ? InputPart : InputPart.substr(0, InputPart.size() - 1);
			Ingredients[IngredientIndex][i] = std::atoi(InputPart.c_str());
		}

		++IngredientIndex;
	}

	Input.close();

	std::cout << "Best Score: " << GetBestScore(Ingredients) << std::endl;

	system("pause");

    return 0;
}

static uint64_t GetBestScore(const IngredientArray & Ingredients)
{
	return RecoursiceRecipe(Ingredients, 0, 100, Ingredient());

}

static uint64_t RecoursiceRecipe(const IngredientArray & Ingredients, size_t IngredientIndex, size_t TablespoonsLeft, Ingredient & AccumulatedIngredientScore)
{
	bool LastIngredient = ((IngredientIndex + 1) == Ingredients.size());
	if(LastIngredient)
	{
		uint64_t Score = 1;

		for (size_t i = 0; i < (AccumulatedIngredientScore.size() - 1); i++)
		{
			AccumulatedIngredientScore[i] += TablespoonsLeft * Ingredients[IngredientIndex][i];

			Score *= (AccumulatedIngredientScore[i] >= 0) ? AccumulatedIngredientScore[i] : 0;
		}

		return Score;
	}

	uint64_t BestScore = 0;
	for (size_t TableSpoons = 0; TableSpoons <= TablespoonsLeft; TableSpoons++)
	{
		Ingredient NewAccumulatedIngredientScore(AccumulatedIngredientScore);

		for (size_t i = 0; i < AccumulatedIngredientScore.size(); i++)
		{
			NewAccumulatedIngredientScore[i] += TableSpoons * Ingredients[IngredientIndex][i];
		}

		uint64_t Score = RecoursiceRecipe(Ingredients, IngredientIndex + 1, TablespoonsLeft - TableSpoons, NewAccumulatedIngredientScore);

		BestScore = (BestScore < Score) ? Score : BestScore;
	}
	
	return BestScore;
}