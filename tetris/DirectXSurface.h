/******************************************************************************
DirectXSurface.h

Created for: Tetris

Copyright (C) 2006-2007 Oliver Zheng.
All rights reserved. No part of this software may be used or reproduced in
any form by any means without prior written permission.

An abstraction class for the DirectDraw 8 surface methods
******************************************************************************/

#include "DataTypes.h"
#include "RGBAImage.h"
#include "DirectXScreen.h"
#include <d3d8.h> 

#pragma once

class DirectXSurface
{
protected:
	// Direct3D surface
	LPDIRECT3DSURFACE8	m_pkD3DSurface;
	RGBAImage *			m_pkRawImage;
public:
	// Constructors & Destructor
	DirectXSurface (void);
	~DirectXSurface (void);

	// Surface manipulation routines
	Error32 Create (RGBAImage * pkRawImage);
	Error32 Update (void);
	Error32 Render (POINT * pkDestPoint, RECT * pkSourceRect = NULL);

	// Surface maintenance methods
	void SetRawImage (RGBAImage * pkRawImage);
	RGBAImage * GetRawImage (void);
};