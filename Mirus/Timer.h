#include "DataTypes.h"
#include <windows.h>
#include <time.h>

#pragma once

class Timer
{
protected:
	//Hardware timer variables
	LARGE_INTEGER	m_iFrequency;
	LARGE_INTEGER	m_iLastQuery;
	LARGE_INTEGER	m_iDelta;

	/* Time and date variables */
	tm *	m_pkTime;

public:
	/* Constructor / Destructor */
	Timer (void);
	~Timer (void);

	/* Update the time variables */
	void Update (void);

	/* Return the timer information */
	Real32 GetDelta (void);
	UInt32 GetSeconds (void);
	UInt32 GetMinutes (void);
	UInt32 GetHours (void);
	UInt32 GetDay (void);
	UInt32 GetMonth (void);
	UInt32 GetYear (void);
};