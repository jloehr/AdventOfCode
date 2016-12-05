#pragma once
class TriangleTest
{
public:
	TriangleTest();

	bool PushSide(signed Side);

private:
	unsigned SideCount;
	signed BiggestSide;
	signed OtherSides;
};

