/******************************************************************************
RGBAImage.cpp

Created for: Tetris

Copyright (C) 2006-2007 Oliver Zheng.
All rights reserved. No part of this software may be used or reproduced in
any form by any means without prior written permission.

The class that stores each pixel of an image that would be rendered in surfaces.
******************************************************************************/

#include "RGBAImage.h"

RGBAImage::RGBAImage (void)
{
	m_iWidth        = 0;
	m_iHeight       = 0;
	m_piImageBuffer = NULL;
}

RGBAImage::~RGBAImage (void)
{
	m_iWidth        = 0;
	m_iHeight       = 0;

	// If memory was allocated, release it
	if (NULL != m_piImageBuffer)
	{
		delete [] m_piImageBuffer;
		m_piImageBuffer = NULL;
	}
}

/******************************************************************************
operator =
Overloads the = to assign the image.

Input Parameters:
	RGBAImage &

Output:
	RGBAImage
******************************************************************************/
RGBAImage & RGBAImage::operator = (RGBAImage & rkImage)
{
	m_iWidth  = rkImage.GetWidth ();
	m_iHeight = rkImage.GetHeight ();

	SetImageBuffer (rkImage.GetImageBuffer ());

	// Return an instance of this class
	return * this;
}

/******************************************************************************
LoadFromTargaResource
Loads the Targa image file from resources.

Input Parameters:
	int resource	-	the resource # defined in resource.h

Output:
	Error32
******************************************************************************/
Error32 RGBAImage::LoadFromTargaResource (int resource) {
	HRSRC hRes = FindResource(NULL,
							  MAKEINTRESOURCE(resource), 
							  RT_RCDATA
							 );
	HGLOBAL hResourceLoaded = LoadResource(NULL, hRes);
    char *lpResLock = (char *) LockResource(hResourceLoaded);

	// Read field description size 
	UInt8	iFieldDescSize = * (UInt8 *)(lpResLock + 0);
	
	// Read image color code
	UInt8	iImageCode = * (UInt8 *)(lpResLock + 2);
	if (2 != iImageCode) {
		return ErrorTargaNotSupported;
	}

	// Read color map
	UInt16 iMapLength = * (UInt16 *)(lpResLock + 5);

	// Read image start positions
	UInt16	iXStart = * (UInt16 *)(lpResLock + 8);
	UInt16	iYStart = * (UInt16 *)(lpResLock + 10);

	// Read image size
	UInt16	iWidth = * (UInt16 *)(lpResLock + 12);
	UInt16	iHeight = * (UInt16 *)(lpResLock + 14);

	m_iWidth	= iWidth;
	m_iHeight	= iHeight;

	// Read image bit depth
	UInt8	iImageBits = * (UInt8 *)(lpResLock + 16);
	if (32 != iImageBits) {
		return ErrorTargaNotSupported;
	}

	// Read image description
	UInt8	iImageDesc = * (UInt8 *)(lpResLock + 17);

	// Read image buffer from file
	UInt32 * piBuffer = new UInt32 [m_iWidth * m_iHeight];
	memcpy(piBuffer, lpResLock + 18 + iFieldDescSize + iMapLength * 4, m_iWidth * m_iHeight * 4 * sizeof (UInt8));

	// Allocate memory for image buffer
	if (NULL != m_piImageBuffer) {
		delete [] m_piImageBuffer;
	}

	m_piImageBuffer = new UInt32 [m_iWidth * m_iHeight];

	UInt8	iRed, iGreen, iBlue, iAlpha;
	UInt32 iColor;

	// Get each pixel color components and fill image buffer 
	UInt32 iX, iY;

	for (iY = 0; iY < m_iHeight; iY++) {
		for (iX = 0; iX < m_iWidth; iX++) {
			// Doens't need to be flipped
			if ((iImageDesc & 1) << 4) {
				// Get color components 
				iColor = piBuffer [iX + (iY * m_iWidth)];
				iAlpha = (UInt8)((iColor & 0xFF000000) >> 24);
				iRed   = (UInt8)((iColor & 0x00FF0000) >> 16);
				iGreen = (UInt8)((iColor & 0x0000FF00) >> 8);
				iBlue  = (UInt8)((iColor & 0x000000FF));

				// Copy flipped position
				m_piImageBuffer [iX + (iY * m_iWidth)] = iAlpha << 0 | iBlue << 8 | iGreen << 16 | iRed << 24;
			}
			// Needs to be flipped
			else {
				// Get color components
				iColor = piBuffer [iX + (iY * m_iWidth)];
				iAlpha = (UInt8)((iColor & 0xFF000000) >> 24);
				iRed   = (UInt8)((iColor & 0x00FF0000) >> 16);
				iGreen = (UInt8)((iColor & 0x0000FF00) >> 8);
				iBlue  = (UInt8)((iColor & 0x000000FF));

				// Copy position
				m_piImageBuffer [iX + (m_iHeight - 1 - iY) * (m_iWidth)] = iAlpha << 0 | iBlue << 8 | iGreen << 16 | iRed << 24;
			}
		}
	}

	// Close file, release memory and return no error
	if (NULL != piBuffer) {
		delete [] piBuffer;
	}

	return NoError;
}

/******************************************************************************
SetColorKey
Sets the color transparency.
E.g. if input color = red, all red pixels will be transparent.

Input Parameters:
	UInt8 iRed		-	Red value
	UInt8 iGreen	-	Green value
	UInt8 iBlue		-	Blue value

Output:
	None
******************************************************************************/
void RGBAImage::SetColorKey (UInt8 iRed, UInt8 iGreen, UInt8 iBlue)
{
	// Get each pixel color components and set color key
	UInt32 iX, iY;
	UInt8 iOriRed, iOriGreen, iOriBlue;
	for (iY = 0; iY < m_iHeight; iY++) {
		for (iX = 0; iX < m_iWidth; iX++) {
			iOriRed = (UInt8)((m_piImageBuffer [iX + (iY * m_iWidth)] & 0xFF000000) >> 24);
			iOriGreen = (UInt8)((m_piImageBuffer [iX + (iY * m_iWidth)] & 0x00FF0000) >> 16);
			iOriBlue  = (UInt8)((m_piImageBuffer [iX + (iY * m_iWidth)] & 0x0000FF00) >> 8);

			// If color matches, set alpha to 0
			if ((iOriRed == iRed) && (iOriGreen == iGreen) && (iOriBlue == iBlue)) {
				m_piImageBuffer [iX + (iY * m_iWidth)] = iOriRed << 24 | iOriGreen << 16 | iOriBlue  << 8  | 0;
			}
		}
	}
}

void RGBAImage::SetWidth (UInt32 iWidth)
{
	m_iWidth = iWidth;
}

void RGBAImage::SetHeight (UInt32 iHeight)
{
	m_iHeight = iHeight;
}

/******************************************************************************
SetColor
Sets a color at a given position.

Input Parameters:
	X, Y
	RGBA

Output:
	None
******************************************************************************/
void RGBAImage::SetColor (UInt32 iX, UInt32 iY, UInt8 iRed, UInt8 iGreen, UInt8 iBlue, UInt8 iAlpha)
{
	UInt32 iColor;
	iColor = D3DCOLOR_RGBA (iRed, iGreen, iBlue, iAlpha);
	m_piImageBuffer [iX + (iY * m_iWidth - 1)] = iColor;
}

/******************************************************************************
SetImageBuffer
Copies the image buffer into the object.

Input Parameters:
	UInt32 *	-	 the image

Output:
	None
******************************************************************************/
void RGBAImage::SetImageBuffer (UInt32 * pImage)
{
	if (NULL != m_piImageBuffer) {
		delete [] m_piImageBuffer;
	}
	m_piImageBuffer = new UInt32 [m_iWidth * m_iHeight];
	memcpy (m_piImageBuffer, pImage, sizeof (UInt32) * m_iWidth * m_iHeight);
}

UInt32 RGBAImage::GetWidth (void)
{
	return m_iWidth;
}

UInt32 RGBAImage::GetHeight (void)
{
	return m_iHeight;
}

UInt32 RGBAImage::GetColor (UInt32 iX, UInt32 iY)
{
	return m_piImageBuffer [iX + iY * m_iWidth];
}

UInt32 * RGBAImage::GetImageBuffer (void)
{
	return m_piImageBuffer;
}