/******************************************************************************
Block.cpp

Created for: Tetris

Copyright (C) 2006-2007 Oliver Zheng.
All rights reserved. No part of this software may be used or reproduced in
any form by any means without prior written permission.

Block is the building block of Tetris. Block is each block represented in a
Tetromino or in the entire Tetris game.

The color of the block defines the state of the block. If the color is CLEAR,
then the block is invisible and will not affect calculations.
******************************************************************************/

#include "Block.h"

Block::Block() : position_x(0), position_y(0) {}

Block::Block(blockColor c) : color(c), position_x(0), position_y(0) {}

void Block::setColor(blockColor c) {
	color = c;
}

blockColor Block::getColor(void) {
	return color;
}

int Block::getX(void) {
	return position_x;
}

int Block::getY(void) {
	return position_y;
}

void Block::setX(int x) {
	position_x = x;
}

void Block::setY(int y) {
	position_y = y;
}
