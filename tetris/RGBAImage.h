/******************************************************************************
RGBAImage.h

Created for: Tetris

Copyright (C) 2006-2007 Oliver Zheng.
All rights reserved. No part of this software may be used or reproduced in
any form by any means without prior written permission.

An abstraction class that extrapolates resource bitmaps and stores each
pixel as an Red, Green, Blue, and Alpha value to be rendered with a surface.
******************************************************************************/

#include "DataTypes.h"
#include <cstring>
#include <windows.h>
#include <fstream>
#include <d3d8.h>

#pragma once

class RGBAImage
{
protected:
	// Image size
	UInt32        m_iWidth;
	UInt32        m_iHeight;

	// Image buffer
	UInt32 *      m_piImageBuffer;

public:
	// Constructor & Destructor
	RGBAImage (void);
	~RGBAImage (void);

	// Operators
	RGBAImage & operator = (RGBAImage & rkImage);

	// Load image from Targa
	Error32 LoadFromTargaResource (int resource);

	// Image manipulation
	void SetColorKey (UInt8 iRed, UInt8 iGreen, UInt8 iBlue);
	void SetWidth (UInt32);
	void SetHeight (UInt32);
	void SetColor (UInt32 iX, UInt32 iY, UInt8 iRed, UInt8 iGreen, UInt8 iBlue, UInt8 iAlpha);
	void SetImageBuffer (UInt32 * pImage);
	UInt32 GetWidth (void);
	UInt32 GetHeight (void);
	UInt32 GetColor (UInt32 iX, UInt32 iY);
	UInt32 * GetImageBuffer (void);
};