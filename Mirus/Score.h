/******************************************************************************
Score.h

Created for: Tetris

Copyright (C) 2006-2007 Oliver Zheng.
All rights reserved. No part of this software may be used or reproduced in
any form by any means without prior written permission.

The Score class stores and updates the score, level, and number of lines
in a Tetris game. Score and # of lines have add functions, while level is
automatically increased according to the number of lines per level.

The updateScreen function uses the static surfaces in the Tetris class to
update the scores. 
******************************************************************************/

#include <cstdlib>
#include <cmath>

using namespace std;

#pragma once

class Score
{
	int m_iScore;
	int m_iLevel;
	int m_iLines;

	static const int m_iScoreDown		= 1;
	static int m_ScorePerLine[];

	static const int m_iLinesPerLevel	= 10;
	static const int m_iMaxLevel		= 20;

	static const int m_iDigitalWidth	= 10;

	int getNumberOfDigits(int number);
	int getDigitValue(int number, int digit);

public:
	Score(void) { reset(); };
	
	void addLines(int iAddLines);
	void addDown(int iDowns = 1);

	void updateScreen(void);
	void reset(void);

	int getSpeed(void) { return ((m_iMaxLevel - m_iLevel) * 5); };
};
