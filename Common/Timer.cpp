#include "stdafx.h"
#include "Timer.h"

void Timer::Start()
{
	this->StartPoint = Clock::now();
}

void Timer::Stop()
{
	this->EndPoint = Clock::now();
}

void Timer::Print()
{
	std::chrono::duration<double, std::milli> ms = this->EndPoint - this->StartPoint;
	std::cout << "Timer: " << ms.count() << " ms" << std::endl;
}

