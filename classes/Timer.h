#pragma once
#include<time.h>
using namespace std;
class CTimer
{
public: // everything is public for ease of access
		// begin/end variables
	clock_t Begin;
	clock_t End;
	// variable declarations used for time calculation
	double elapTicks;
	double elapMilli, elapSeconds, elapMinutes;
	// constructor: the timer will begin as soon as a timer is created
	CTimer(); 
	// call myTimer.Stop () to stop the timer
	void Stop();
	// call getTimes
	void getTimes();
	~CTimer();
	
};

