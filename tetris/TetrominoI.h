/******************************************************************************
TetrominoI.h

Created for: Tetris

Copyright (C) 2006-2007 Oliver Zheng.
All rights reserved. No part of this software may be used or reproduced in
any form by any means without prior written permission.

|
|
|
|

Inherited from Tetromino
******************************************************************************/

#include "Tetromino.h"

class TetrominoI : public Tetromino
{
	Block*	blocks;
	Block*	blocks_buffer;

	static const int block_count = 4;
	static const blockColor block_color = RED;

	int	rotation_state;
	static const int rotation_count = 2;

	static int setup_positions_x[block_count];
	static int setup_positions_y[block_count];

	static int next_positions_x[block_count];
	static int next_positions_y[block_count];

	// relative positions that each block should move to
	static int rotation_positions_x[rotation_count][block_count];
	static int rotation_positions_y[rotation_count][block_count];

public:
	TetrominoI();
	~TetrominoI();

	bool		setup(void);
	void		nextSetup(void);

	int			getBlockCount(void) const { return block_count; };
	Block*		getBlocks(void) const { return blocks; };
	Block*		getBlocksBuffer(void) const { return blocks_buffer; };
	blockColor	getBlockColor(void) const { return block_color; };

	int			getRotationState(void) const { return rotation_state; };
	int			getRotationCount(void) const { return rotation_count; };

	void		setRotationState(int s) { rotation_state = s; };

	void		rotate_map(void);
};