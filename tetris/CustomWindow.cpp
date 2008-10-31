/******************************************************************************
CustomWindow.cpp

Created for: Tetris

Copyright (C) 2006-2007 Oliver Zheng.
All rights reserved. No part of this software may be used or reproduced in
any form by any means without prior written permission.

CustomWindow inherits from WindowManager. It handles the Windows messages and
each frame of the game.
******************************************************************************/

#include "CustomWindow.h"

Bool32 CustomWindow::Frame(void) {
	tetris.update();
	Sleep(5);

	return EN_TRUE;
}

void CustomWindow::keyDown(HWND hWindow, WPARAM k) {
	CustomWindow * pkWindow = (CustomWindow *) GetWindowLong (hWindow, GWL_USERDATA);
	switch (k) { 
		case VK_LEFT: 
			pkWindow->tetris.keyDown(LEFT);
			break; 
		case VK_RIGHT: 
			pkWindow->tetris.keyDown(RIGHT);
			break; 
		case VK_UP: 
			pkWindow->tetris.keyDown(ROTATE);
			break; 
		case VK_DOWN: 
			pkWindow->tetris.keyDown(DOWN);
			break;  
		case VK_SPACE: 
			pkWindow->tetris.keyDown(DROP);
			break;  
		default: 
			break; 
	}
}

void CustomWindow::AfterCreate(void)
{
	Screen.Init (m_hWindow);
	Screen.SetMode (false, 0, 0, 0, true);
	tetris.addScreen(&Screen);
}
