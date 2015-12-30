// Day18.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

const static size_t FieldSize = 100;

typedef std::array<bool, FieldSize> Row;
typedef std::array<Row, FieldSize> Field;
typedef std::shared_ptr<Field> PField;

static PField CreateField(const std::string & InputFile);
static size_t GetTurnedOnLights(PField Lights, size_t Steps);

static bool GetNewLightSate(PField Lights, size_t X, size_t Y);
static size_t GetTurnedOnNeighbors(PField Lights, size_t X, size_t Y);

int main()
{
	PField Lights = CreateField("Input.txt");
	size_t LightsOn = GetTurnedOnLights(Lights, 100);

	std::cout << "Lights on: " << LightsOn << std::endl;

	system("pause");

    return 0;
}


static PField CreateField(const std::string & InputFile)
{
	PField Lights = std::make_shared<Field>();

	StringVector InputLines = GetFileLines(InputFile);
	Field::iterator RowIter = Lights->begin();

	for (const std::string & Line : InputLines)
	{
		Row::iterator LightIter = (RowIter++)->begin();
		for (const char & Char : Line)
		{
			(*(LightIter++)) = (Char == '#');
		}
	}

	return Lights;
}

static size_t GetTurnedOnLights(PField Lights, size_t Steps)
{
	PField Buffer = std::make_shared<Field>();
	size_t LightsOn = 0;

	for (size_t i = 0; i < Steps; i++)
	{
		LightsOn = 0;

		for (size_t x = 0; x < FieldSize; x++)
			for (size_t y = 0; y < FieldSize; y++)
			{
				bool NewState = GetNewLightSate(Lights, x, y);
				(*Buffer)[x][y] = NewState;

				if (NewState)
				{
					++LightsOn;
				}
			}

		Buffer.swap(Lights);
	}


	return LightsOn;
}

static bool GetNewLightSate(PField Lights, size_t X, size_t Y)
{
	size_t TurnedOnNeighbors = GetTurnedOnNeighbors(Lights, X, Y);

	if ((*Lights)[X][Y])
	{
		return ((TurnedOnNeighbors == 2) || (TurnedOnNeighbors == 3));
	}
	else
	{
		return (TurnedOnNeighbors == 3);
	}
}

static size_t GetTurnedOnNeighbors(PField Lights, size_t X, size_t Y)
{
	size_t LightsOn = 0;

	for (size_t TX = (X > 0) ? X - 1 : X; TX <= std::min(X + 1, FieldSize - 1); ++TX)
		for (size_t TY = (Y > 0) ? Y - 1 : Y; TY <= std::min(Y + 1, FieldSize - 1); ++TY)
		{
			if ((TX == X) && (TY == Y))
			{
				continue;
			}

			LightsOn += ((*Lights)[TX][TY]) ? 1 : 0;
		}

	return LightsOn;
}