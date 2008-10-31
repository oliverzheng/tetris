/******************************************************************************
WindowManager.h

Created for: Tetris

Copyright (C) 2006-2007 Oliver Zheng.
All rights reserved. No part of this software may be used or reproduced in
any form by any means without prior written permission.

An abstract abstraction class for the creation of windows.
Only 1 window can be created since this uses a static method to hold the window
Frame of the derived class should be executed in the main program loop.
******************************************************************************/

#include <windows.h> 
#include "DataTypes.h"

using namespace std;

#pragma once

class WindowManager
{
protected:
	WNDCLASS  m_kWndClass;
	HWND      m_hWindow;
	MSG       m_kMessage;

public:
	WindowManager (void);
	~WindowManager (void);

	// Window manipulation methods 
	Error32 Create(HINSTANCE hInstance, LPSTR szTitle, int iWidth = CW_USEDEFAULT,
						int iHeight = CW_USEDEFAULT, UInt32 iStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE);
	virtual void AfterCreate(void) {};
	static LRESULT CALLBACK WndProc (HWND hWindow, UINT iMessage, WPARAM wParam, LPARAM lParam);
	void Run (void);

	// Custom methods
	virtual Bool32 MessageHandler (UINT iMessage, WPARAM wParam, LPARAM lParam);
	virtual Bool32 Frame (void) = 0;
};