/**
*  Stopwatch.cpp
*
*  Defines a stopwatch timer class
*/

#include "Stopwatch.hpp"

//#include <format>


void Stopwatch::start()
{
	if (!this->running)
	{
		this->running = true;
		
		this->start_t = std::chrono::system_clock::now();
	}
}

void Stopwatch::stop()
{
	if (this->running)
	{
		this->end_t = std::chrono::system_clock::now();
		
		duration += (end_t - start_t);
		
		this->running = false;
	}
}

void Stopwatch::reset()
{
	this->running = false;
	
	this->duration = std::chrono::seconds::zero();
}

std::string Stopwatch::getFormattedTime()
{
	//return std::format("{:%T}", this->duration);
	
	std::string time = std::to_string(this->duration.count());
	
	return time;
}
