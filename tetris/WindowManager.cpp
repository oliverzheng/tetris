/******************************************************************************
WindowManager.cpp

Created for: Tetris

Copyright (C) 2006-2007 Oliver Zheng.
All rights reserved. No part of this software may be used or reproduced in
any form by any means without prior written permission.

WindowManager is the generic class for managing windows.
******************************************************************************/

#include "WindowManager.h"
#include "resource.h"
#include "CustomWindow.h"

using namespace std;

WindowManager::WindowManager (void) {}

WindowManager::~WindowManager (void) {}

/******************************************************************************
Create
Sets up and registers a window.

Input Parameters:
	HINSTANCE hInstance	-	the instance handler of the process
	LPSTR szTitle		-	the title of the Window
	int iWidth			-	width of the window
	int iHeight			-	height of the window
	UInt32 iStyle		-	default window behavior

Output:
	Error32	-	the enumerated value of an error
******************************************************************************/
Error32 WindowManager::Create (HINSTANCE hInstance, LPSTR szTitle, int iWidth, int iHeight, UInt32 iStyle)
{
	// "Visual" properties
	m_kWndClass.hCursor			= LoadCursor (NULL, IDC_ARROW);
	m_kWndClass.hIcon			= LoadIcon (hInstance, MAKEINTRESOURCE(IDI_TETRIS_ICON));
	m_kWndClass.hbrBackground	= (HBRUSH) GetStockObject (WHITE_BRUSH);

	// System properties
	m_kWndClass.hInstance		= hInstance;
	m_kWndClass.lpfnWndProc		= WndProc;
	m_kWndClass.lpszClassName	= "Tetris";

	// Extra properties
	m_kWndClass.lpszMenuName	= NULL;

	m_kWndClass.cbClsExtra		= NULL;
	m_kWndClass.cbWndExtra		= NULL;
	m_kWndClass.style			= NULL;

	// Try to register class
	if (!RegisterClass (&m_kWndClass)) {
		return ErrorRegisterClass;
	}

	// Create the window
	m_hWindow = CreateWindow ("Tetris", szTitle, iStyle, CW_USEDEFAULT, CW_USEDEFAULT, iWidth, iHeight, 
								NULL, NULL, hInstance, (void *) this);
	SetWindowText (m_hWindow, szTitle);

	AfterCreate();

	return NoError;
}

/******************************************************************************
WndProc
Message handler that would pass the message down to CustomWindow

Input Parameters:
	Generic message parameters.

Output:
	LRESULT
******************************************************************************/
LRESULT CALLBACK WindowManager::WndProc (HWND hWindow, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	WindowManager * pkWindow   = NULL;
	Bool32   bProcessed = EN_FALSE;

	switch (iMessage)
	{
		// Window is creating - set custom information
		case WM_NCCREATE:
			SetWindowLong (hWindow, GWL_USERDATA, (long)((LPCREATESTRUCT(lParam))->lpCreateParams));
			break;

        case WM_KEYDOWN: 
			CustomWindow::keyDown(hWindow, wParam);
			break;

		// Window message - Let our handler process it
		default:
			pkWindow = (WindowManager *) GetWindowLong (hWindow, GWL_USERDATA);
			if (NULL != pkWindow) {
				bProcessed = pkWindow->MessageHandler (iMessage, wParam, lParam);
			}
			break;
	}
	// Message not processed - let windows handle it
	if (EN_FALSE == bProcessed) {
		return DefWindowProc (hWindow, iMessage, wParam, lParam);
	}
	return 0;
}

/******************************************************************************
Run
The loop that would be repeated throughout program execution.
Dispatches messages to the handler.

Input Parameters:
	None

Output:
	None
******************************************************************************/
void WindowManager::Run (void)
{
	while (1)
	{
		// Query to see if there is any message in the queue
		if (PeekMessage (&m_kMessage, m_hWindow, 0, 0, PM_REMOVE))
		{
			// If it is the WM_QUIT message, quit the loop
			if (WM_QUIT == m_kMessage.message) {
				break;
			}
			// Process the message normally
			else {
				TranslateMessage (&m_kMessage);
				DispatchMessage (&m_kMessage);
			}
		}

		// do frame 
		Frame ();
	}
}

/******************************************************************************
Run
The message handler processor.

Input Parameters:
	None

Output:
	None
******************************************************************************/
Bool32 WindowManager::MessageHandler (UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage) {
		// Close window
		case WM_CLOSE:
			exit(0);
			return EN_TRUE;
			break;

		// Not handled - let Windows handle
		default:
			return EN_FALSE;
			break;
	}
}