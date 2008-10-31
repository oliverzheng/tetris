/******************************************************************************
DirectXSurface.cpp

Created for: Tetris

Copyright (C) 2006-2007 Oliver Zheng.
All rights reserved. No part of this software may be used or reproduced in
any form by any means without prior written permission.

The generic DirectDraw surface class.
******************************************************************************/

#include "DirectXSurface.h"

DirectXSurface::DirectXSurface (void)
{
	m_pkD3DSurface = NULL;
	m_pkRawImage   = NULL;
}

DirectXSurface::~DirectXSurface (void)
{
	if (m_pkD3DSurface != NULL) {
		m_pkD3DSurface->Release ();
		m_pkD3DSurface = NULL;
	}

	if (m_pkRawImage != NULL) {
		delete m_pkRawImage;
		m_pkRawImage   = NULL;
	}
}

/******************************************************************************
Create
Puts an image into the surface.

Input Parameters:
	RGBAImage *	-	the pointer of the RGBAImage.

Output:
	Error32	-	the enumerated value of an error
******************************************************************************/
Error32 DirectXSurface::Create (RGBAImage * pkRawImage)
{
	// Set the surface raw image and update the Direct3D surface
	SetRawImage (pkRawImage);
	return Update ();
}

/******************************************************************************
Update
Updates the image into the buffer.

Input Parameters:
	None

Output:
	Error32	-	the enumerated value of an error
******************************************************************************/
Error32 DirectXSurface::Update (void)
{
	if (m_pkD3DSurface != NULL) {
		m_pkD3DSurface->Release ();
		m_pkD3DSurface = NULL;
	}

	if (m_pkRawImage == NULL) {
		return ErrorInvalidRawImage;
	}

	// Create the surface
	if (FAILED(DirectXScreen::GetSingleton()->GetDevice()->CreateImageSurface(m_pkRawImage->GetWidth(),
			m_pkRawImage->GetHeight(), (D3DFORMAT)DirectXScreen::GetSingleton()->GetFormat(), &m_pkD3DSurface))) {
		
		m_pkD3DSurface = NULL;
		return ErrorCreateImageSurface;
	}

	// Lock surface
	D3DLOCKED_RECT kLockedRect;
	m_pkD3DSurface->LockRect (&kLockedRect, NULL, 0);

	// Pointers to locked surface
	WORD * pi16SurfaceBuffer;
	DWORD * pi32SurfaceBuffer;

	// Use correct pointer depending on depth
	if (DirectXScreen::GetSingleton ()->GetBitdepth () == 16) {
		// Cast a pointer to point to the first pixel
		pi16SurfaceBuffer = (WORD *) kLockedRect.pBits;
	} else {
		// Cast a pointer to point to the first pixel
		pi32SurfaceBuffer = (DWORD *) kLockedRect.pBits;
	}

	// Fill surface
	UInt32 iX, iY;
	UInt32 iDepth;

	// Get depth in bytes and calculate pitch
	iDepth = DirectXScreen::GetSingleton ()->GetBitdepth () / 8;
	UInt32 iPitch = kLockedRect.Pitch / iDepth;
	UInt8  iRed, iGreen, iBlue, iAlpha;
	UInt32 iColor;

	for (iY=0; iY < m_pkRawImage->GetHeight (); iY++) {
		for (iX=0; iX < m_pkRawImage->GetWidth (); iX++) {

			// Get color components
			iColor = m_pkRawImage->GetColor (iX, iY);
			iRed   = (UInt8)((iColor & 0xFF000000) >> 24);
			iGreen = (UInt8)((iColor & 0x00FF0000) >> 16);
			iBlue  = (UInt8)((iColor & 0x0000FF00) >> 8);
			iAlpha = (UInt8)((iColor & 0x000000FF));

			// Write color to surface buffer according to mode
			if (DirectXScreen::GetSingleton ()->GetBitdepth () == 16) {
				if (DirectXScreen::GetSingleton ()->GetFormat () == D3DFMT_R5G6B5) {
					pi16SurfaceBuffer [iX + iY * iPitch] = (UInt16)((iRed * ((1 << 5) -1) / 255 << 11) |
																	  (iGreen * ((1 << 6) -1) / 255 << 5) |
																	  (iBlue  * ((1 << 5) -1) / 255));
				}
				if (DirectXScreen::GetSingleton ()->GetFormat () == D3DFMT_X1R5G5B5) {
					pi16SurfaceBuffer [iX + iY * iPitch] = (UInt16)((iRed * ((1 << 5) -1) / 255 << 10) |
																	  (iGreen * ((1 << 5) -1) / 25 << 5) |
																	  (iBlue  * ((1 << 5) -1) / 255));
				}
				if (DirectXScreen::GetSingleton ()->GetFormat () == D3DFMT_A1R5G5B5) {
					pi16SurfaceBuffer [iX + iY * iPitch] = (UInt16)(((iAlpha > 0) ? 1 : 0 << 15) |
																	  (iRed * ((1 << 5) -1) / 255 << 10) |
																	  (iGreen * ((1 << 5) -1) / 255 << 5) |
																	  (iBlue  * ((1 << 5) -1) / 255));
				}
			} else {
				pi32SurfaceBuffer [iX + iY * iPitch] = D3DCOLOR_ARGB (iAlpha, iRed, iGreen, iBlue);
			}
		}
	}

	// Unlock 
	m_pkD3DSurface->UnlockRect ();

	return NoError;
}

/******************************************************************************
Render
Draws the image onto a designated position.

Input Parameters:
	POINT *	-	the x & y location of the position to start rendering.
	RECT *	-	the rectangle to render in.

Output:
	Error32	-	the enumerated value of an error
******************************************************************************/
Error32 DirectXSurface::Render (POINT * pkDestPoint, RECT * pkSourceRect)
{
	// Get back buffer
	if (m_pkD3DSurface != NULL) {
		LPDIRECT3DSURFACE8 pBackBuffer;
		DirectXScreen::GetSingleton()->GetDevice()->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);

		// Copy the surface to the screen 
		if (pkSourceRect != NULL) {
			DirectXScreen::GetSingleton()->GetDevice()->CopyRects(m_pkD3DSurface, pkSourceRect, 1, pBackBuffer, pkDestPoint);
		} else {
			RECT kImageRect;

			// Use entire image
			kImageRect.left   = 0;
			kImageRect.top    = 0;
			kImageRect.right  = m_pkRawImage->GetWidth () - 1;
			kImageRect.bottom = m_pkRawImage->GetHeight () - 1;
			DirectXScreen::GetSingleton()->GetDevice()->CopyRects(m_pkD3DSurface, &kImageRect, 1, pBackBuffer, pkDestPoint);
		}

		// Release back buffer
		pBackBuffer->Release ();
	}

	return NoError;
}

/******************************************************************************
SetRawImage
Stores the image in the object buffer.

Input Parameters:
	RGBAImage *	-	the pointer to the RGBAImage image.

Output:
	Error32	-	the enumerated value of an error
******************************************************************************/
void DirectXSurface::SetRawImage (RGBAImage * pkRawImage)
{
	if (m_pkRawImage == NULL) {
		m_pkRawImage = new RGBAImage ();
	}

	m_pkRawImage->SetWidth (pkRawImage->GetWidth ());
	m_pkRawImage->SetHeight (pkRawImage->GetHeight ());
	m_pkRawImage->SetImageBuffer (pkRawImage->GetImageBuffer ());
}

RGBAImage * DirectXSurface::GetRawImage (void)
{
	return m_pkRawImage;
}