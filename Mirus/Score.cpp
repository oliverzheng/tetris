/******************************************************************************
Score.cpp

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

#include "Score.h"
#include "Tetris.h"

// Assuming the maximum number of lines down is 4, the score given per level
int Score::m_ScorePerLine[] = {0, 10, 30, 90, 200};

/******************************************************************************
addLines
When the a number of lines are eliminated in the Tetris game, the number is
sent to this function to update the number of lines as well as scores and level

Input Parameters:
	iLines	-	number of lines to be added to the current number of lines.

Output:
	None
******************************************************************************/
void Score::addLines(int iLines) {
	m_iLines += iLines;
	if((m_iLines >= (m_iLinesPerLevel * m_iLevel)) && (m_iLevel < m_iMaxLevel)) {
		m_iLevel++;
	}
	m_iScore += m_ScorePerLine[iLines] * m_iLevel;
}

/******************************************************************************
addDown
When the user presses DOWN, the score is increased by m_iScoreDown.
When the user drops the floating piece, the number of downs that would have
occured is calculated and passed in as iDowns. Thus the default is 1.

Input Parameters:
	iDowns	-	number of downs pressed.

Output:
	None
******************************************************************************/
void Score::addDown(int iDowns) {
	m_iScore += m_iScoreDown * iDowns;
}

/******************************************************************************
updateScreen
Updates the DirectX screen (static variable inside Tetris) for the numbers.
For each of score, level, and lines, each digit of the number in base 10 is
displayed with a bitmap in the resource file that is loaded by the class Tetris

Input Parameters:
	none

Output:
	None
******************************************************************************/
void Score::updateScreen(void) {
	for(int i = 0; i < getNumberOfDigits(m_iScore); i++) {
		int digit = getDigitValue(m_iScore, i);
		Tetris::score_surfaces[i].Create (&Tetris::images[digit + 8]);
		POINT kPosition = {Tetris::score_x - (i + 1) * m_iDigitalWidth, Tetris::score_y};
		Tetris::score_surfaces[i].Render (&kPosition);
	}
	for(int i = 0; i < getNumberOfDigits(m_iLines); i++) {
		int digit = getDigitValue(m_iLines, i);
		Tetris::lines_surfaces[i].Create (&Tetris::images[digit + 8]);
		POINT kPosition = {Tetris::lines_x - (i + 1) * m_iDigitalWidth, Tetris::lines_y};
		Tetris::lines_surfaces[i].Render (&kPosition);
	}
	for(int i = 0; i < getNumberOfDigits(m_iLevel); i++) {
		int digit = getDigitValue(m_iLevel, i);
		Tetris::level_surfaces[i].Create (&Tetris::images[digit + 8]);
		POINT kPosition = {Tetris::level_x - (i + 1) * m_iDigitalWidth, Tetris::level_y};
		Tetris::level_surfaces[i].Render (&kPosition);
	}
}

/******************************************************************************
getNumberOfDigits
Calculates the number of digits in base 10 of the input parameter number

Input Parameters:
	number	-	the integer that we are trying to find the length of

Output:
	digits	-	the number of digits of number.
******************************************************************************/
int Score::getNumberOfDigits(int number) {
	int digits = 0;
	do {
		digits++;
	} while ((number /= 10) > 0);
	return digits;
}

/******************************************************************************
getDigitValue
Calculates the value of a certain digit in a number of base 10.

Input Parameters:
	number	-	the number whose digit we are trying to find
	digit	-	the position of the digit starting with 0 at the ones digit.

Output:
	the integer digit
******************************************************************************/
int Score::getDigitValue(int number, int digit) {
	return ((number / (int)pow(10.0, digit)) % 10);
}

/******************************************************************************
reset
Resets the score, level, and lines.
Used when initializing the game or restarting the game.

Input Parameters:
	none

Output:
	None
******************************************************************************/
void Score::reset() {
	m_iScore = 0;
	m_iLevel = 1;
	m_iLines = 0;
}