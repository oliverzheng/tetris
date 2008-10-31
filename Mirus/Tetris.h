/******************************************************************************
Tetris.h

Created for: Tetris

Copyright (C) 2006-2007 Oliver Zheng.
All rights reserved. No part of this software may be used or reproduced in
any form by any means without prior written permission.

The Tetris class is the main class for the Tetris game.
Once initialized, the game will start and all graphics will be rendered
in the update function.
******************************************************************************/

#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Block.h"
#include "Tetromino.h"
#include "DirectXManager.h"
#include "resource.h"
#include "Score.h"

#pragma once

class Tetris
{
protected:
	// These setup the horizontal and vertical number of blocks for the game
	static const int width	= 10;
	static const int height	= 21;

	// flag variables
	unsigned int timer;
	keyPress last_action;
	bool	game_over;

	// the array of blocks for the game
	static Block blocks[height][width];

	// for on screen rendering: # of resource bitmaps & DirectX surface counts
	static const int image_count = 19;
	static const int next_surfaces_count = 8;
	static const int score_surfaces_count = 10;
	static const int lines_surfaces_count = 5;
	static const int level_surfaces_count = 2;

	// the generic DirectX screen for which the surfaces should draw on
	static DirectXScreen*	Screen;

	// the score of the game
	Score score;

	// pointer to the current floating piece of tetromino
	Tetromino*	current_tetromino;

public:
	Tetris(void);
	~Tetris(void);

	// 2 initialization functions called after the Window and DirectX setup
	void setupBlocks(void);
	void addScreen(DirectXScreen* s);

	// called upon windows message for a keypress
	void keyDown(keyPress k);

	// called every frame of the game (5 milliseconds / frame)
	void update(void);

	// the top 2 lines are hidden to allow rotation of the current piece
	static const int hidden_lines = 2;

	// graphic x/y positions for rendering locations
	static const int container_x = 284;
	static const int container_y = 40;
	static const int next_x = 610;
	static const int next_y = 95;
	static const int score_x = 653;
	static const int score_y = 204;
	static const int lines_x = 653;
	static const int lines_y = 296;
	static const int level_x = 653;
	static const int level_y = 390;
	static const int gameover_x = 284;
	static const int gameover_y = 40;

	// the images loaded from resource bitmaps
	static RGBAImage	images[image_count];

	// the surfaces to render on
	static DirectXSurface	block_surfaces[height][width];
	static DirectXSurface	misc_surfaces[2];
	static DirectXSurface	next_surfaces[next_surfaces_count];
	static DirectXSurface	score_surfaces[next_surfaces_count];
	static DirectXSurface	lines_surfaces[lines_surfaces_count];
	static DirectXSurface	level_surfaces[level_surfaces_count];

	// Get functions
	static Block*			getBlocks(void);
	static DirectXScreen*	getScreen(void);

	static int				getWidth(void);
	static int				getHeight(void);

	// checks if a line contains all blocks after every current tetromino dies
	int		checkLines(void);

	// if the current tetromino dies, a new one is generated
	void	new_tetromino(void);

	// called when a new tetromino cannot be generated = game over
	void	gameOver(void);
};