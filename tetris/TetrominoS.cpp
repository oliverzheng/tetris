/******************************************************************************
TetrominoS.cpp

Created for: Tetris

Copyright (C) 2006-2007 Oliver Zheng.
All rights reserved. No part of this software may be used or reproduced in
any form by any means without prior written permission.

  - - 
  |
- -

Inherited from Tetromino
******************************************************************************/

#include "TetrominoS.h"
#include "Tetris.h"

// Absolute positions in the Tetris screen that the blocks should be setup.
int TetrominoS::setup_positions_x[] = {Tetris::getWidth() / 2 - 1,
									   Tetris::getWidth() / 2,
									   Tetris::getWidth() / 2,
									   Tetris::getWidth() / 2 + 1};
int TetrominoS::setup_positions_y[] = {3, 3, 2, 2};

// Relative positions that should be displayed in the Tetris NEXT box
int TetrominoS::next_positions_x[] = {0, 1, 1, 2};
int TetrominoS::next_positions_y[] = {1, 1, 0, 0};

// relative positions that each block should move to
int TetrominoS::rotation_positions_x[][block_count] = {{0, -1, 0, -1},
													   {0, 1, 0, 1}};
int TetrominoS::rotation_positions_y[][block_count] = {{-2, -1, 0, 1},
													   {2, 1, 0, -1}};


/******************************************************************************
TetrominoS
Constructor - allocates the blocks and buffer blocks & sets the designated color.

Input Parameters:
	none

Output:
	None
******************************************************************************/
TetrominoS::TetrominoS() : rotation_state(0) {
	blocks = new Block[block_count];
	blocks_buffer = new Block[block_count];
	
	for(int i = 0; i < block_count; i++) {
		blocks[i].setColor(block_color);
	}
	restore_buffer();
}

/******************************************************************************
TetrominoS
Delete the allocated blocks & buffer blocks

Input Parameters:
	none

Output:
	none
******************************************************************************/
TetrominoS::~TetrominoS() {
	delete []blocks;
	delete []blocks_buffer;
}

/******************************************************************************
setup()
Sets up each block at its origin locations. The setup may not be successful
due to the existing blocks on the Tetris screen. (This would be game over).

Input Parameters:
	none

Output:
	True	- if setup was successful
	False	- if unsuccessful
******************************************************************************/
bool TetrominoS::setup() {
	for(int i = 0; i < block_count; i++) {
		blocks[i].setX(setup_positions_x[i]);
		blocks[i].setY(setup_positions_y[i]);
		restore_buffer();
	}
	if(check()) {
		return true;
	} else {
		return false;
	}
}

/******************************************************************************
rotate_map
Rotates each block in the Tetromino blocks buffer. Does not actually change
anything in the real blocks.

Input Parameters:
	none

Output:
	none
******************************************************************************/
void TetrominoS::rotate_map() {
	for(int i = 0; i < block_count; i++) {
		blocks_buffer[i].setX(blocks_buffer[i].getX() + rotation_positions_x[rotation_state][i]);
		blocks_buffer[i].setY(blocks_buffer[i].getY() + rotation_positions_y[rotation_state][i]);
	}
}

/******************************************************************************
nextSetup
Sets up the positions of the real (non-buffer) blocks for display on NEXT.

Input Parameters:
	none

Output:
	none
******************************************************************************/
void TetrominoS::nextSetup(void) {
	for(int i = 0; i < block_count; i++) {
		blocks[i].setX(next_positions_x[i]);
		blocks[i].setY(next_positions_y[i]);
	}
}
