/******************************************************************************
Tetromino.cpp

Created for: Tetris

Copyright (C) 2006-2007 Oliver Zheng.
All rights reserved. No part of this software may be used or reproduced in
any form by any means without prior written permission.

Tetromino is the floating piece that can be rotated and moved in a Tetris game
The Tetromino is an abstract class and cannot be instantiated.
The blocks such as the long I or square O have to inherit from this class.
However, this class can generate a new Tetromino of any kind and return the
pointer to a Tetris game.

The controls (rotate, down, left, right) are defined to allow inheritance.
******************************************************************************/

#include "Tetromino.h"
#include "TetrominoI.h"
#include "TetrominoJ.h"
#include "TetrominoL.h"
#include "TetrominoO.h"
#include "TetrominoS.h"
#include "TetrominoT.h"
#include "TetrominoZ.h"
#include "Tetris.h"

Tetromino* Tetromino::next_piece = NULL;

/******************************************************************************
generate
returns a pointer to a randomly generated Tetromino child class
and sets up the next piece.

Input Parameters:
	none

Output:
	pointer to a child Tetromino class
******************************************************************************/
Tetromino* Tetromino::generate() {
	Tetromino* tetro;
	Tetromino* temp;

	int number = rand() % 7;

	switch(number) {
		case 0:
			tetro = new TetrominoI;
			break;
		case 1:
			tetro = new TetrominoJ;
			break;
		case 2:
			tetro = new TetrominoL;
			break;
		case 3:
			tetro = new TetrominoO;
			break;
		case 4:
			tetro = new TetrominoS;
			break;
		case 5:
			tetro = new TetrominoT;
			break;
		case 6:
		default:
			tetro = new TetrominoZ;
			break;
	}

	temp = next_piece;
	next_piece = tetro;

	return temp;
}

/******************************************************************************
left
First shifts the buffer (a copy of the current blocks) positions to the left.
If it doesn't touch the border, do the same for the current blocks;
Otherwise, don't; reset the buffer to the original blocks

Input Parameters:
	None

Output:
	Bool:
		True if successfully moved to the left
		False if cannot move to the left (because of either the left border
			  or blocks on the left
******************************************************************************/
bool Tetromino::left(void) {
	for(int i = 0; i < getBlockCount(); i++) {
		getBlocksBuffer()[i].setX(getBlocksBuffer()[i].getX() - 1);
	}

	if((check_border() == 0) && check()) {
		copy_buffer();
		return true;
	} else {
		restore_buffer();
		return false;
	}
}

/******************************************************************************
right
First shifts the buffer (a copy of the current blocks) positions to the right.
If it doesn't touch the border, do the same for the current blocks;
Otherwise, don't; reset the buffer to the original blocks

Input Parameters:
	None

Output:
	Bool:
		True if successfully moved to the right
		False if cannot move to the right (because of either the right border
			  or blocks on the right
******************************************************************************/
bool Tetromino::right(void) {
	for(int i = 0; i < getBlockCount(); i++) {
		getBlocksBuffer()[i].setX(getBlocksBuffer()[i].getX() + 1);
	}

	if((check_border() == 0) && check()) {
		copy_buffer();
		return true;
	} else {
		restore_buffer();
		return false;
	}
}

/******************************************************************************
merge
Takes the blocks of the Tetris game and sets up the ones of the same position
as the current blocks of the floating tetromino - making those color the same
as the current color.

This essentially initializes the blocks in the Tetris game and makes them visible

Input Parameters:
	None

Output:
	None
******************************************************************************/
void Tetromino::merge() {
	for(int i = 0; i < getBlockCount(); i++) {
		Tetris::getBlocks()[getBlocks()[i].getY() * Tetris::getWidth() +
			getBlocks()[i].getX()].setColor(getBlockColor());
	}
}

/******************************************************************************
check_border
Checks to see if the buffer blocks of the current floating piece of Tetromino
is touching the left border or touching the right border or neither.

Input Parameters:
	None

Output:
	Int:
		0 if not touching left border or right border
		1 if touching the left border
		2 if touching the right border
******************************************************************************/
int Tetromino::check_border() {
	for(int i = 0; i < getBlockCount(); i++) {
		if(getBlocksBuffer()[i].getX() < 0) {
			return 1;
		} else if(getBlocksBuffer()[i].getX() >= Tetris::getWidth()) {
			return 2;
		}
	}
	return 0;
}

/******************************************************************************
check
Check to see if the block buffer is touching the blocks of the Tetris game

Input Parameters:
	None

Output:
	Bool:
		True if it's not touching anyhting
		False if it's touching something
******************************************************************************/
bool Tetromino::check() {
	for(int i = 0; i < getBlockCount(); i++) {
		if(Tetris::getBlocks()[getBlocksBuffer()[i].getY() * Tetris::getWidth() +
			getBlocksBuffer()[i].getX()].getColor() != CLEAR) {
			return false;
		}
	}
	return true;
}

/******************************************************************************
copy_buffer
Copies the current data to buffer so left/right functions can process

Input Parameters:
	None

Output:
	None
******************************************************************************/
void Tetromino::copy_buffer() {
	for(int i = 0; i < getBlockCount(); i++) {
		getBlocks()[i].setX(getBlocksBuffer()[i].getX());
		getBlocks()[i].setY(getBlocksBuffer()[i].getY());
		getBlocks()[i].setColor(getBlocksBuffer()[i].getColor());
	}
}

/******************************************************************************
restore_buffer
Copies the buffer data to the current data to update the changes.
This usually happens when the buffered data has been tested for conditions.

Input Parameters:
	None

Output:
	None
******************************************************************************/
void Tetromino::restore_buffer() {
	for(int i = 0; i < getBlockCount(); i++) {
		getBlocksBuffer()[i].setX(getBlocks()[i].getX());
		getBlocksBuffer()[i].setY(getBlocks()[i].getY());
		getBlocksBuffer()[i].setColor(getBlocks()[i].getColor());
	}
}

/******************************************************************************
down
Moves down the blocks of the Tetromino.

Input Parameters:
	None

Output:
	True	- if down is successful
	False	- if down is unsuccessful: e.g. already at the bottom.
******************************************************************************/
bool Tetromino::down() {
	for(int i = 0; i < getBlockCount(); i++) {
		getBlocksBuffer()[i].setY(getBlocksBuffer()[i].getY() + 1);
	}
	if(check()) {
		copy_buffer();
		return true;
	} else {
		restore_buffer();
		merge();
		return false;
	}
}

/******************************************************************************
drop
Drops the Tetromino all the way down by repeating down() as many times as possible.

Input Parameters:
	None

Output:
	int	-	the number of times down was called. used for scoring purposes.
******************************************************************************/
int Tetromino::drop() {
	int counter = 0;
	while(down()) {
		counter++;
	}
	return counter;
}


/******************************************************************************
update
Uses the Tetris class to update the display of each of the blocks in the Tetromino
on screen.

Input Parameters:
	None

Output:
	None
******************************************************************************/
void Tetromino::update() {
	for(int i = 0; i < getBlockCount(); i++) {
		if(getBlocks()[i].getY() >= Tetris::hidden_lines) {
			Tetris::block_surfaces[getBlocks()[i].getY()]
				[getBlocks()[i].getX()].Create(&Tetris::images[getBlockColor()]);
			POINT kPosition = {Tetris::container_x + getBlocks()[i].getX() * Block::width,
				Tetris::container_y + (getBlocks()[i].getY() - Tetris::hidden_lines) * Block::height};
			Tetris::block_surfaces[getBlocks()[i].getY()][getBlocks()[i].getX()].Render(&kPosition);
		}
	}
}

/******************************************************************************
rotate
Rotates the blocks according to the configuration of rotation_positions_x and
rotation_positions_y.

Note: If the Tetromino is at the edge of the Tetris screen, a rotate that would
go out of bounds would cause the Tetromino to shift to back into the screen.
Thus the Tetromino is always rotated and will never be stuck, unless the bounding
edge is blocks already stuck in the Tetris screen.

Input Parameters:
	None

Output:
	None
******************************************************************************/
void Tetromino::rotate() {
	rotate_map();

	do {
		if((check_border() == 0) && !check()) {
			restore_buffer();
			return;
		} else if (check_border() != 0) {
			int border = (check_border() == 1) ? (1) : (-1);
			for(int i = 0; i < getBlockCount(); i++) {
				getBlocksBuffer()[i].setX(getBlocksBuffer()[i].getX() + border);
			}
		}
	} while(check_border() > 0);

	if(!check()) {
		restore_buffer();
		return;
	}

	copy_buffer();

	setRotationState((getRotationState() + 1) % getRotationCount());
}

/******************************************************************************
renderNext
Static function for the rendering the next piece of Tetromino on the Tetris screen.
The blocks are automatically centered onto the display blocks in Tetris.

Input Parameters:
	None

Output:
	None
******************************************************************************/
void Tetromino::renderNext() {
	next_piece->nextSetup();

	bool odd_width;
	bool odd_height;

	int max_width = 0;
	int max_height = 0;

	for(int i = 0; i < next_piece->getBlockCount(); i++) {
		if(next_piece->getBlocks()[i].getX() > max_width) {
			max_width = next_piece->getBlocks()[i].getX();
		}
		if(next_piece->getBlocks()[i].getY() > max_height) {
			max_height = next_piece->getBlocks()[i].getY();
		}
	}
	
	odd_width = (max_width % 2) ? false : true;
	odd_height = (max_height % 2) ? false : true;

	int starting_x = - (max_width + 1) * Block::width / 2;
	int starting_y = - (max_height + 1) * Block::width / 2;

	for(int i = 0; i < next_piece->getBlockCount(); i++) {
		Tetris::next_surfaces[i].Create (&Tetris::images[next_piece->getBlockColor()]);
		POINT kPosition = {Tetris::next_x + starting_x + next_piece->getBlocks()[i].getX() * Block::width,
			Tetris::next_y + starting_y + next_piece->getBlocks()[i].getY() * Block::height};
		Tetris::next_surfaces[i].Render (&kPosition);
	}
}
