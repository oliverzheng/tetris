/******************************************************************************
TetrisTypes.h

Created for: Tetris

Copyright (C) 2006-2007 Oliver Zheng.
All rights reserved. No part of this software may be used or reproduced in
any form by any means without prior written permission.

Defines the color (also states) for each block in the Tetris game.
"CLEAR" not only signifies the color, but also the inavailability of the block
keyPress is used for the user input
******************************************************************************/
#include <cstdlib>

using namespace std;

#pragma once

enum blockColor {
	RED = 1,
	YELLOW,
	MAGENTA,
	BLUE,
	CYAN,
	GREEN,
	ORANGE,
	CLEAR,
};

enum keyPress {
	ROTATE,
	LEFT,
	RIGHT,
	DOWN,
	DROP,
	EMPTY,
};