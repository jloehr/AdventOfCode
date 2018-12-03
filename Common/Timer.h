#pragma once

class Timer
{
public:
	Timer() = default;
	~Timer() = default;

	void Start();
	void Stop();
	void Print();

private:
	using Clock = std::chrono::high_resolution_clock;

	std::chrono::time_point<Clock> StartPoint;
	std::chrono::time_point<Clock> EndPoint;
};

