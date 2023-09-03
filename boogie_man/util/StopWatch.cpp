#include "StopWatch.h"


void stopwatch_t::stopwatch()
{
	seconds = 0;
	mins = 0;
	hours = 0;
	clockchecker.set();
	start = std::chrono::steady_clock::now();
}

template<typename T>
T stopwatch_t::returncurrenttime()
{
	return duration_cast<T>((std::chrono::steady_clock::now() - start));
}

int stopwatch_t::returnseconds()
{
	auto duration = std::chrono::steady_clock::now() - start;
	seconds = std::chrono::duration_cast<std::chrono::seconds>(duration).count() % 60;
	return seconds;
}

int stopwatch_t::returnminutes()
{
	auto duration = std::chrono::steady_clock::now() - start;
	int totalSeconds = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
	mins = (totalSeconds / 60) % 60;
	return mins;
}


int stopwatch_t::returnhours()
{
	auto duration = std::chrono::steady_clock::now() - start;
	int totalSeconds = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
	hours = totalSeconds / 3600; 
	return hours;
}


void stopwatch_t::resetstopwatch()
{
	stopwatch();
}