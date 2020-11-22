// Day10.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <regex>

#include "../../Common/Common.h"

constexpr int64_t Spacing = 1;

struct Point
{
	int64_t X;
	int64_t Y;
	int64_t VelocityX;
	int64_t VelocityY;
};

typedef std::vector<Point> PointList;

struct Bounds
{
	int64_t Left;
	int64_t Top;
	int64_t Right;
	int64_t Bottom;
};

bool TickPoints(PointList& Points, const Bounds LastBounds, Bounds& NewBounds);

int main()
{
	const StringVector File = GetFileLines("Input.txt");
	std::regex InputMatching("position=<\\s*(-?\\d+),\\s*(-?\\d+)> velocity=<\\s*(-?\\d+),\\s*(-?\\d+)>", std::regex_constants::optimize);

	std::vector<Point> Points = std::vector<Point>();
	Points.reserve(Points.size());

	for (const std::string& Line : File)
	{
		std::smatch Matches;
		if (!std::regex_match(Line, Matches, InputMatching))
		{
			std::cout << "Line \"" << Line << "\" did not match the regex!" << std::endl;
			return 1;
		}

		Points.push_back({ std::stoll(Matches[1]), std::stoll(Matches[2]), std::stoll(Matches[3]), std::stoll(Matches[4]) });
	}

	PointList LastState = Points;
	Bounds LastBounds = { INT64_MIN, INT64_MIN, INT64_MAX, INT64_MAX };
	Bounds Bounds;

	while (TickPoints(Points, LastBounds, Bounds))
	{
		LastState = Points;
		LastBounds = Bounds;
	}

	for (int64_t Y = (LastBounds.Top - Spacing); Y <= (LastBounds.Bottom + Spacing); Y++)
	{
		for (int64_t X = (LastBounds.Left - Spacing); X <= (LastBounds.Right + Spacing); X++)
		{
			std::cout << (std::any_of(LastState.cbegin(), LastState.cend(), [=](const Point& Point) {return ((Point.X == X) && (Point.Y == Y)); }) ? '#' : '.');
		}
		std::cout << std::endl;
	}
}


bool TickPoints(PointList& Points, const Bounds LastBounds, Bounds& NewBounds)
{
	NewBounds = { INT64_MAX, INT64_MAX, INT64_MIN, INT64_MIN };

	for (auto& Point : Points)
	{
		Point.X += Point.VelocityX;
		Point.Y += Point.VelocityY;

		if ((Point.X < LastBounds.Left)
			|| (Point.X > LastBounds.Right)
			|| (Point.Y < LastBounds.Top)
			|| (Point.Y > LastBounds.Bottom))
		{
			return false;
		}

		NewBounds.Left = std::min(NewBounds.Left, Point.X);
		NewBounds.Right = std::max(NewBounds.Right, Point.X);
		NewBounds.Top = std::min(NewBounds.Top, Point.Y);
		NewBounds.Bottom = std::max(NewBounds.Bottom, Point.Y);
	}

	return true;
}
