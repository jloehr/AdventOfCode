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

	bool operator==(const Vector3 & other) const
	{
		return ((X == other.X) && (Y == other.Y) && (Z == other.Z));
	}

	bool operator!=(const Vector3 & other) const
	{
		return !(*this == other);
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

	void Update()
	{
		Velocity += Acceleration;
		Position += Velocity;
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

bool AccelerationSorter(const Particle & lhs, const Particle & rhs)
{
	if (lhs.Acceleration.Magnitude() != rhs.Acceleration.Magnitude())
		return (lhs.Acceleration.Magnitude() < rhs.Acceleration.Magnitude());

	if (lhs.Velocity.Magnitude() != rhs.Velocity.Magnitude())
		return (lhs.Velocity.Magnitude() < rhs.Velocity.Magnitude());

	return (lhs.Position.Magnitude() < rhs.Position.Magnitude());
}

bool PositionSorter(const Particle & lhs, const Particle & rhs)
{
	if (lhs.Position.X != rhs.Position.X)
		return lhs.Position.X < rhs.Position.X;
	if (lhs.Position.Y != rhs.Position.Y)
		return lhs.Position.Y < rhs.Position.Y;

	return lhs.Position.Z < rhs.Position.Z;
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

	std::sort(std::begin(Particles), std::end(Particles), AccelerationSorter);

	std::cout << "Long Term nearest: " << Particles[0].Index << std::endl;

	for(size_t Counter = 0; Counter < 10; ++Counter)
	{
		std::for_each(std::begin(Particles), std::end(Particles), [](auto & Particle) { Particle.Update(); });
		std::sort(std::begin(Particles), std::end(Particles), PositionSorter);
		std::vector<Particle> Buffer;

		for (auto It = std::begin(Particles); It != std::end(Particles); std::advance(It, 1))
		{
			if (((It == std::begin(Particles)) || (It->Position != (It - 1)->Position)) && ((It == (std::end(Particles) - 1)) || (It->Position != (It + 1)->Position)))
			{
				Buffer.push_back(*It);
			}
		}

		Particles.swap(Buffer);

		if (Particles.size() != Buffer.size())
			Counter = 0;
	}

	std::cout << "Particle count: " << Particles.size() << std::endl;
	system("pause");
    return 0;
}

