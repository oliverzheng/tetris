/******************************************************************************
Tetris.cpp

Created for: Tetris

Copyright (C) 2006-2007 Oliver Zheng.
All rights reserved. No part of this software may be used or reproduced in
any form by any means without prior written permission.

Tetris is the main class for handling operations.
Tetris contains:
- the blocks that are dead on the screen
- the DirectDraw screens so that other related classes can access them and draw

The general gist of this class is that after setup, it is looped inside the
CustomWindow class. All messages are directed to this class, and then passed
down to the appropriete methods.
******************************************************************************/
#include "Tetris.h"

// DirectDraw surfaces setup
DirectXScreen*	Tetris::Screen = NULL;
RGBAImage	Tetris::images[] = {};
DirectXSurface	Tetris::block_surfaces[][width] = {};
DirectXSurface	Tetris::misc_surfaces[] = {};
DirectXSurface	Tetris::next_surfaces[] = {};
DirectXSurface	Tetris::score_surfaces[] = {};
DirectXSurface	Tetris::lines_surfaces[] = {};
DirectXSurface	Tetris::level_surfaces[] = {};
Block		Tetris::blocks[][width] = {};

/******************************************************************************
Tetris
Constructor - seeds rand.

Input Parameters:
	none

Output:
	None
******************************************************************************/
Tetris::Tetris() : timer(0), game_over(false) {

	srand((unsigned)time(NULL));
	rand();
	setupBlocks();
}

Tetris::~Tetris() {}

/******************************************************************************
setupBlocks
Sets up the blocks so they are all CLEAR - invisible and non-operational on
the screen.

Input Parameters:
	none

Output:
	None
******************************************************************************/
void Tetris::setupBlocks() {
	for(int i = 0; i < height; i++) {
		for(int j = 0; j < width; j++) {
			blocks[i][j].setColor(CLEAR);
			blocks[i][j].setY(i);
			blocks[i][j].setX(j);
		}
	}
}

/******************************************************************************
addScreen
Loads the images from resources.
Sets up the background and the gameover image.

Input Parameters:
	none

Output:
	None
******************************************************************************/
void Tetris::addScreen(DirectXScreen* s) {
	Screen = s;

	Tetromino::generate();
	new_tetromino();

	static int image_resource_ids[] =  {IDI_TETRIS_BG,
										IDI_TETRIS_BLOCK_RED,
										IDI_TETRIS_BLOCK_YELLOW,
										IDI_TETRIS_BLOCK_MAGENTA,
										IDI_TETRIS_BLOCK_BLUE,
										IDI_TETRIS_BLOCK_CYAN,
										IDI_TETRIS_BLOCK_GREEN,
										IDI_TETRIS_BLOCK_ORANGE,
										IDI_TETRIS_NUMBER_0,
										IDI_TETRIS_NUMBER_1,
										IDI_TETRIS_NUMBER_2,
										IDI_TETRIS_NUMBER_3,
										IDI_TETRIS_NUMBER_4,
										IDI_TETRIS_NUMBER_5,
										IDI_TETRIS_NUMBER_6,
										IDI_TETRIS_NUMBER_7,
										IDI_TETRIS_NUMBER_8,
										IDI_TETRIS_NUMBER_9,
										IDI_TETRIS_GAMEOVER
									   };

	for(int i = 0; i < image_count; i++) {
		images[i].LoadFromTargaResource(image_resource_ids[i]);
	}

	misc_surfaces[0].Create (&images[0]);
	misc_surfaces[1].Create (&images[18]);
}

/******************************************************************************
keyDown
Called when Tetris gets the keyDown message in the loop method.
Passes down the appropriete keys. Checks for line completion & game over.

Input Parameters:
	none

Output:
	None
******************************************************************************/
void Tetris::keyDown(keyPress k) {
	if(!game_over) {
		last_action = k;
		if(k == DOWN) {
			score.addDown();
			timer = 1;
			if(!current_tetromino->down()) {
				checkLines();
				new_tetromino();
			}
		} else if (k == ROTATE) {
			current_tetromino->rotate();
		} else if (k == LEFT) {
			current_tetromino->left();
		} else if (k == RIGHT) {
			current_tetromino->right();
		} else if (k == DROP) {
			timer = 1;
			score.addDown(current_tetromino->drop());
			checkLines();
			new_tetromino();
		}
	} else {
		if (k == DROP) {
			game_over = false;
			setupBlocks();
			new_tetromino();
			score.reset();
		}
	}
}

/******************************************************************************
update
The loop function:
- When the time is up, automatically move down the Tetromino. (So it won't be
stuck in air and not moving at all.)
- Each dead block is rendered.

Input Parameters:
	none

Output:
	None
******************************************************************************/
void Tetris::update() {

	timer = (timer + 1) % UINT_MAX;

	if(!game_over) {
		if((last_action != DOWN) && ((timer % score.getSpeed()) == 0)) {
			if(!current_tetromino->down()) {
				checkLines();
				new_tetromino();
			}
		}
		last_action = EMPTY;
	}


	// Render
	Screen->Clear (0, 0, 0, 255);
	Screen->StartFrame();
	
	// background render
	POINT kPosition = {0, 0};
	misc_surfaces[0].Render (&kPosition);

	// render current blocks
	for(int i = 0; i < height; i++) {
		for(int j = 0; j < width; j++) {
			if(blocks[i][j].getColor() != CLEAR) {
				if(blocks[i][j].getY() >= hidden_lines) {
					block_surfaces[i][j].Create (&images[blocks[i][j].getColor()]);
					POINT kPosition = {container_x + blocks[i][j].getX() * Block::width, container_y + (blocks[i][j].getY() - hidden_lines) * Block::height};
					block_surfaces[i][j].Render (&kPosition);
				}
			}
		}
	}
	Tetromino::renderNext();
	score.updateScreen();

	if(!game_over) {
		current_tetromino->update();
	} else {
		POINT kPosition = {gameover_x, gameover_y};
		misc_surfaces[1].Render(&kPosition);
	}

	Screen->EndFrame ();
}

Block*	Tetris::getBlocks(void) {
	return &blocks[0][0];
}

DirectXScreen* Tetris::getScreen(void) {
	return Screen;
}

int Tetris::getWidth(void) {
	return width;
}

int	Tetris::getHeight(void) {
	return height;
}

/******************************************************************************
checkLines
Checks for completion of lines, makes those blocks Clear and shifts upper blocks

Input Parameters:
	none

Output:
	Int	-	the number of lines that were completed
******************************************************************************/
int	Tetris::checkLines() {
	int lines = 0;
	for(int i = 0; i < height; i++) {
		for(int j = 0; j < width; j++) {
			if(blocks[i][j].getColor() == CLEAR) {
				break;
			} else if(j == (width - 1)) {
				lines++;
				for(int k = i; k >= 0; k--) {
					for(int l = 0; l < width; l++) {
						if(k == 0) {
							blocks[k][l].setColor(CLEAR);
						} else {
							blocks[k][l].setColor(blocks[k-1][l].getColor());
						}
					}
				}
			}
		}
	}
	score.addLines(lines);
	return lines;
}

/******************************************************************************
new_tetromino
Generates a new tetromino and triggers gameover if unsuccessful.

Input Parameters:
	none

Output:
	None
******************************************************************************/
void Tetris::new_tetromino() {
	if(current_tetromino != NULL) {
		delete current_tetromino;
	}
	current_tetromino = Tetromino::generate();

	if(!current_tetromino->setup()) {
		gameOver();
	}
}

/******************************************************************************
gameOver
Sets gameover flat to be true.

Input Parameters:
	none

Output:
	None
******************************************************************************/
void Tetris::gameOver() {
	game_over = true;
}