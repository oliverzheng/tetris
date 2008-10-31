/* nrWindows.cpp */

/* Complement header file */
#include "Timer.h"

using namespace std;

/* Default constructor */
Timer::Timer (void)
{
	/* Get the hardware clock frequency and current count */
	QueryPerformanceFrequency (&m_iFrequency);
	Update ();
}

/* Default destructor */
Timer::~Timer (void)
{
	m_iFrequency.QuadPart = 0;
	m_iLastQuery.QuadPart = 0;
}

/* Update timer */
void Timer::Update (void)
{
	LARGE_INTEGER  kTempTimer;
	time_t         iTempTimeDate;

	/* Get current timer information and calculate difference */
	QueryPerformanceCounter (&kTempTimer);
	m_iDelta.QuadPart = kTempTimer.QuadPart - m_iLastQuery.QuadPart;

	/* Save current timer information */
	m_iLastQuery.QuadPart = kTempTimer.QuadPart;

	/* Get current time and date */
	time (&iTempTimeDate);
	m_pkTime = localtime (&iTempTimeDate);
}

/* Get delta time from last update */
Real32 Timer::GetDelta (void)
{
	/* Convert to float and calculate delta in seconds */
	return (Real32)(m_iDelta.QuadPart) / (Real32)(m_iFrequency.QuadPart);
}

/* Get system seconds */
UInt32 Timer::GetSeconds (void)
{
	return m_pkTime->tm_sec;
}

/* Get system minutes */
UInt32 Timer::GetMinutes (void)
{
	return m_pkTime->tm_min;
}

/* Get system hours */
UInt32 Timer::GetHours (void)
{
	return m_pkTime->tm_hour;
}

/* Get system day */
UInt32 Timer::GetDay (void)
{
	return m_pkTime->tm_mday;
}

/* Get system month */
UInt32 Timer::GetMonth (void)
{
	return m_pkTime->tm_mon;
}

/* Get system year */
UInt32 Timer::GetYear (void)
{
	return m_pkTime->tm_year;
}