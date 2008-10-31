/******************************************************************************
main.cpp

Created for: Tetris

Copyright (C) 2006-2007 Oliver Zheng.
All rights reserved. No part of this software may be used or reproduced in
any form by any means without prior written permission.

Entry program into Tetris.
It setups a CustomWindow and lets it loop.
******************************************************************************/

#include <windows.h>
#include <winbase.h>
#include "Tetris.h"
#include "DirectXManager.h"
#include "CustomWindow.h"

#define	APP_TITLE	"Tetris"

using namespace std;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nShowCmd)
{
	// The window
	CustomWindow	kWindow;
	// Create window
	kWindow.Create (hInstance, APP_TITLE, 700 + 7, 536 + 33);
	// Enter message loop
	kWindow.Run ();

	return 0;
}