/******************************************************************************
Block.h

Created for: Tetris

Copyright (C) 2006-2007 Oliver Zheng.
All rights reserved. No part of this software may be used or reproduced in
any form by any means without prior written permission.

The block class defines each block in the Tetris game.
The block has attributes of color, and x&y coordinates for onscreen rendering.
The block can be initialized with or without a color. Each property can be
retrieved and set with the get/set functions. The static width/height const
int's are the width/height of the block graphic to be rendered.
******************************************************************************/

#include "TetrisTypes.h"

#pragma once

class Block
{
protected:
	blockColor	color;
	int			position_x;
	int			position_y;

public:
	Block(void);
	Block(blockColor c);

	static const int width = 24;
	static const int height = 24;

	void		setColor(blockColor c);
	blockColor	getColor(void);

	int			getX(void);
	int			getY(void);

	void		setX(int x);
	void		setY(int y);
};