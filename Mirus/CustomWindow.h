/******************************************************************************
CustonWindow.h

Created for: Tetris

Copyright (C) 2006-2007 Oliver Zheng.
All rights reserved. No part of this software may be used or reproduced in
any form by any means without prior written permission.

The more specific class inherited from WindowManager.
After initialization, Frame() is run in every frame of the main program loop.
keyDown() passes the key presses to Tetris to allow processing.
******************************************************************************/

#include "DirectXManager.h"
#include "Tetris.h"

// Custom derived class
class CustomWindow : public WindowManager
{
	DirectXScreen		Screen;
	Tetris				tetris;

public:
	// Constructor & Destructor
	CustomWindow (void) {};
	~CustomWindow (void) {};

	void AfterCreate(void);

	// Window manipulation functions
	Bool32 Frame (void);
	static void keyDown(HWND hWindow, WPARAM k);
};
