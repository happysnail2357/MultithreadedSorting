/**
*  Stopwatch.hpp
*
*  Declares a stopwatch timer class
*/

#ifndef STOPWATCH_HPP_MULTITHREADED_SORTING
#define STOPWATCH_HPP_MULTITHREADED_SORTING


#include <chrono>
#include <string>


class Stopwatch
{
public:
	
	void start();
	void stop();
	void reset();
	
	std::string getFormattedTime();
	
private:
	
	std::chrono::time_point<std::chrono::system_clock> start_t, end_t;
	std::chrono::duration<double> duration;
	
	bool running = false;
};


#endif
