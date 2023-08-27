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
	seconds = duration_cast<int>((std::chrono::steady_clock::now() - start)) % 61;
	return seconds;
}

int stopwatch_t::returnminutes()
{

	if ((seconds == 60) && (clockchecker[0] == 1))
	{
		mins++;
		clockchecker[0] = false;
	}
	if (seconds == 59)
	{
		clockchecker[0] = true;
	}

	return mins;
}

int stopwatch_t::returnhours()
{
	if ((mins == 60) && (clockchecker[1] == 1))
	{
		hours++;
		mins = 0;
		clockchecker[1] = false;
	}
	if (mins == 59)
	{
		clockchecker[1] = true;
	}

	return hours;
}

void stopwatch_t::resetstopwatch()
{
	stopwatch();
}