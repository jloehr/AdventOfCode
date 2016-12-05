#include "stdafx.h"
#include "TriangleTest.h"


TriangleTest::TriangleTest()
	:SideCount(0), BiggestSide(0), OtherSides(0)
{
}

bool TriangleTest::PushSide(signed Side)
{
	SideCount++;

	if (Side > BiggestSide)
	{
		OtherSides += BiggestSide;
		BiggestSide = Side;
	}
	else
	{
		OtherSides += Side;
	}


	if (SideCount >= 3)
	{
		bool Result = (OtherSides > BiggestSide);

		SideCount = 0;
		BiggestSide = 0;
		OtherSides = 0;

		return Result;
	}
	else
	{
		return false;
	}
}

