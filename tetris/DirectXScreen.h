/******************************************************************************
DirectXScreen.h

Created for: Tetris

Copyright (C) 2006-2007 Oliver Zheng.
All rights reserved. No part of this software may be used or reproduced in
any form by any means without prior written permission.

An abstraction class for the DirectDraw 8 screen methods
******************************************************************************/

#include "DataTypes.h"
#include "Timer.h"
#include <windows.h> 
#include <d3d8.h> 
#include <math.h> 
#include <assert.h> 

#pragma once

class Vertex
{
public:
	// Transformed position
	FLOAT m_fX, m_fY, m_fZ, m_fRHW;
	// Color
	DWORD m_iColor;
	// Texture coordinates
	FLOAT m_ftU, m_ftV;
};

// Custom vertex type
#define D3DFVF_VERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

class DirectXScreen
{
protected:
	LPDIRECT3D8			m_pkD3D;
	LPDIRECT3DDEVICE8	m_pkD3DDevice;
	HWND				m_hWindow;
	UInt32				m_iFormat;

	// Singleton
	static DirectXScreen *  m_pkSingleton;

public:
	// Constructors & Destructor
	DirectXScreen (void);
	DirectXScreen (HWND hWindow);
	~DirectXScreen (void);

	// Screen manipulation routines
	Error32 Init (HWND hWindow);
	Error32 SetMode (UInt32 iFullscreen, UInt16 iWidth, UInt16 iHeight, UInt16 iDepth, bool bHardware); 

	// Render routines 
	Error32 Clear (UInt8 iRed, UInt8 iGreen, UInt8 iBlue, UInt8 iAlpha);
	Error32 StartFrame (void);
	Error32 EndFrame (void);

	// Draw routines
	Error32 DrawLine (Real32 fX1, Real32 fY1, Real32 fX2, Real32 fY2, UInt8 iRed, UInt8 iGreen, UInt8 iBlue, UInt8 iAlpha);
	Error32 DrawRectangle (Real32 fX1, Real32 fY1, Real32 fX2, Real32 fY2, UInt8 iRed, UInt8 iGreen, UInt8 iBlue, UInt8 iAlpha);
	Error32 DrawCircle (Real32 fCenterX, Real32 fCenterY, Real32 iRadius, UInt8 iRed, UInt8 iGreen, UInt8 iBlue, UInt8 iAlpha, UInt32 iVertices);

	// Miscellaneous routines
	Bool32 IsModeSupported (UInt16 iWidth, UInt16 iHeight, UInt16 iDepth);
	void ShowCursor (UInt32 iShowCursor);

	// Maintenance methods
	LPDIRECT3DDEVICE8 GetDevice (void);
	UInt32 GetFormat (void);

	UInt32 GetBitdepth (void);
	// Singleton
	static DirectXScreen * GetSingleton (void);
};