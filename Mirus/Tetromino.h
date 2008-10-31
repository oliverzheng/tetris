/******************************************************************************
Tetromino.h

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

#include <cstdlib>
#include "Block.h"
#include "DirectXManager.h"

#pragma once

class Tetromino
{
protected:
	static Tetromino* next_piece;
public:
	static Tetromino* generate(void);

	static Tetromino* getNext(void) { return next_piece; };
	static void	renderNext(void);

	virtual void	update(void);
	virtual bool	setup(void)		= 0;
	virtual	void	nextSetup(void)	= 0;

	virtual int			getBlockCount(void) const	= 0;
	virtual Block*		getBlocks(void)	const		= 0;
	virtual Block*		getBlocksBuffer(void) const	= 0;
	virtual blockColor	getBlockColor(void)	const	= 0;

	virtual int			getRotationState(void) const	= 0;
	virtual int			getRotationCount(void) const	= 0;

	virtual void		setRotationState(int s)	= 0;

	virtual void	rotate(void);
	virtual void	rotate_map(void)	= 0;
	virtual bool	down(void);
	virtual int		drop(void);
	virtual bool	left(void);
	virtual bool	right(void);

	virtual void	restore_buffer(void);
	virtual void	copy_buffer(void);
	virtual bool	check(void);
	virtual int		check_border(void);
	virtual void	merge(void);
};