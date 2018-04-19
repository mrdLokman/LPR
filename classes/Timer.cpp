#include "Timer.h"



CTimer::CTimer()
{
	 this->Begin = clock() * CLK_TCK; 
}
void CTimer::Stop() {
	this->End = clock() * CLK_TCK; getTimes();
}
void CTimer::getTimes() {
	this->elapTicks = this->End - this->Begin; // stop the timer, and calculete the time taken
	this->elapMilli = this->elapTicks / 1000; //milliseconds from Begin to End
	this->elapSeconds = this->elapMilli / 1000; //seconds from Begin to End
	this->elapMinutes = this->elapSeconds / 60; //minutes from Begin to End
}
CTimer::~CTimer()
{
}
