// Day20.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

template<typename T>
struct Vector3 {
public:
	T X;
	T Y;
	T Z;

	Vector3(T X = 0, T Y = 0, T Z = 0)
		:X(X), Y(Y), Z(Z) {}

	const T Magnitude() const
	{
		return std::abs(X) + std::abs(Y) + std::abs(Z);
	} 

	Vector3 & operator=(const Vector3 & other)
	{
		if (this != &other)
		{
			X = other.X;
			Y = other.Y;
			Z = other.Z;
		}

		return *this;
	}

	Vector3 & operator+=(const Vector3 & rhs)
	{
		X += rhs.X;
		Y += rhs.Y;
		Z += rhs.Z;

		return *this;
	}
};

using Vector3I = Vector3<int64_t>;

struct Particle
{
	size_t Index;
	Vector3I Position;
	Vector3I Velocity;
	Vector3I Acceleration;

	Particle(size_t Index, Vector3I Position, Vector3I Velocity, Vector3I Acceleration)
		:Index(Index), Position(Position), Velocity(Velocity), Acceleration(Acceleration) {}

	bool operator<(const Particle & rhs) const
	{
		if (Acceleration.Magnitude() != rhs.Acceleration.Magnitude())
			return (Acceleration.Magnitude() < rhs.Acceleration.Magnitude());

		if (Velocity.Magnitude() != rhs.Velocity.Magnitude())
			return (Velocity.Magnitude() < rhs.Velocity.Magnitude());

		return (Position.Magnitude() < rhs.Position.Magnitude());
	}
};

Vector3I ParseVector(const std::string & String)
{
	Vector3I Vector;
	Vector.X = std::stoll(String.substr(3));
	size_t Delim = String.find(',');
	Vector.Y = std::stoll(String.substr(Delim + 1));
	Delim = String.find(',', Delim + 1);
	Vector.Z = std::stoll(String.substr(Delim + 1));

	return Vector;
}

int main()
{
	const StringVectorVector Input = GetFileLineParts("Input.txt");
	std::vector<Particle> Particles;
	size_t Index = 0;

	for (const auto & Line : Input)
	{
		Vector3I Position = ParseVector(Line[0]);
		Vector3I Velocity = ParseVector(Line[1]);
		Vector3I Acceleration = ParseVector(Line[2]);
		Particles.emplace_back(Index++, Position, Velocity, Acceleration);
	}

	std::sort(std::begin(Particles), std::end(Particles));

	std::cout << Particles[0].Index << std::endl;

	system("pause");
    return 0;
}

