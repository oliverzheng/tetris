/******************************************************************************
DirectXScreen.cpp

Created for: Tetris

Copyright (C) 2006-2007 Oliver Zheng.
All rights reserved. No part of this software may be used or reproduced in
any form by any means without prior written permission.


******************************************************************************/

#include "DirectXScreen.h"

// Singleton object
DirectXScreen * DirectXScreen::m_pkSingleton = NULL;

// Default constructor
DirectXScreen::DirectXScreen (void)
{
	m_pkD3D			= NULL;
	m_pkD3DDevice	= NULL;
	m_hWindow		= NULL;

	assert(!m_pkSingleton);

	m_pkSingleton = this;
}

// Assignment constructor
DirectXScreen::DirectXScreen (HWND hWindow)
{
	m_pkD3D			= NULL;
	m_pkD3DDevice	= NULL;
	m_hWindow		= hWindow;

	assert(!m_pkSingleton);

	m_pkSingleton	= this;

	Init (hWindow);
}

DirectXScreen::~DirectXScreen (void)
{
	if (NULL != m_pkD3DDevice) {
		m_pkD3DDevice->Release();
		m_pkD3DDevice = NULL;
	}

	if (NULL != m_pkD3D) {
		m_pkD3D->Release();
		m_pkD3D = NULL;
	}

	assert (m_pkSingleton);

	m_pkSingleton = NULL;
}

/******************************************************************************
Init
Initializes DirectDraw/3D

Input Parameters:
	HWND	-	the handle of the window.

Output:
	Error32
******************************************************************************/
Error32 DirectXScreen::Init (HWND hWindow)
{
	// Create Direct3D object
	m_pkD3D		= Direct3DCreate8(D3D_SDK_VERSION);
	m_hWindow	= hWindow;

	if (NULL == m_pkD3D) {
		return ErrorInitDirect3D;
	}

	return NoError;
}

/******************************************************************************
SetMode
Sets the mode and creates the Direct3D device.

Input Parameters:
	UInt32 iFullscreen	-	fullscreen or not
	UInt16 iWidth		-	width of the device
	UInt16 iHeight		-	height of the device
	UInt16 iDepth		-	image bits (16 or 24)
	bool bHardware		-	Hardware acceleration or not.
Output:
	Error32
******************************************************************************/
Error32 DirectXScreen::SetMode (UInt32 iFullscreen, UInt16 iWidth, UInt16 iHeight, UInt16 iDepth, bool bHardware)
{
	D3DPRESENT_PARAMETERS	kPresentParams; 
	UInt32				iDeviceType;

	// Set type of device to create (hardware or software)
	if (!bHardware)	{
		iDeviceType = D3DDEVTYPE_REF;
	} else {
		iDeviceType = D3DDEVTYPE_HAL;
	}

	// Reset present parameters and set swap effect
	ZeroMemory(&kPresentParams, sizeof (D3DPRESENT_PARAMETERS) );
	kPresentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;

	// If windowed mode
	if (!iFullscreen) {
		D3DDISPLAYMODE kCurrentMode;

		// Get current mode information
		if (FAILED (m_pkD3D->GetAdapterDisplayMode (D3DADAPTER_DEFAULT, &kCurrentMode))) {
			return ErrorGetAdapterDisplayMode;
		}

		// Set windowed mode and backbuffer format compatible with current format
		kPresentParams.Windowed			= true;
		kPresentParams.BackBufferFormat	= kCurrentMode.Format;

		// Try to create device
		if (FAILED(m_pkD3D->CreateDevice (D3DADAPTER_DEFAULT, (D3DDEVTYPE) iDeviceType, m_hWindow,
					D3DCREATE_SOFTWARE_VERTEXPROCESSING, &kPresentParams, &m_pkD3DDevice))) {
			return ErrorCreateDevice;
		}
	}
	// Full Screen Mode
	else {
		// Set full screen mode and full screen information
		kPresentParams.Windowed			= false;
		kPresentParams.BackBufferCount	= 1;
		kPresentParams.BackBufferWidth	= iWidth;
		kPresentParams.BackBufferHeight	= iHeight;
		kPresentParams.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

		kPresentParams.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
		// If 16 bit, try to create the device using different 16 bit pixel color formats
		if (iDepth == 16) {
			kPresentParams.BackBufferFormat = D3DFMT_R5G6B5;
			if (FAILED (m_pkD3D->CreateDevice (D3DADAPTER_DEFAULT, (D3DDEVTYPE) iDeviceType, m_hWindow,
												D3DCREATE_SOFTWARE_VERTEXPROCESSING, &kPresentParams, &m_pkD3DDevice))) {
				kPresentParams.BackBufferFormat = D3DFMT_X1R5G5B5;
				if (FAILED (m_pkD3D->CreateDevice (D3DADAPTER_DEFAULT, (D3DDEVTYPE) iDeviceType, m_hWindow,
													D3DCREATE_SOFTWARE_VERTEXPROCESSING, &kPresentParams, &m_pkD3DDevice))) {
					kPresentParams.BackBufferFormat = D3DFMT_A1R5G5B5;
					if (FAILED (m_pkD3D->CreateDevice (D3DADAPTER_DEFAULT, (D3DDEVTYPE) iDeviceType, m_hWindow,
													  D3DCREATE_SOFTWARE_VERTEXPROCESSING, &kPresentParams, &m_pkD3DDevice))) {
						return ErrorCreateDevice;
					}
				}
			}
		}

		 // If 32 bit, try to create the device using different pixel 32 color formats
		else {
			kPresentParams.BackBufferFormat = D3DFMT_A8R8G8B8;
			if (FAILED (m_pkD3D->CreateDevice (D3DADAPTER_DEFAULT, (D3DDEVTYPE) iDeviceType, m_hWindow,
											  D3DCREATE_SOFTWARE_VERTEXPROCESSING, &kPresentParams, &m_pkD3DDevice))) {
				kPresentParams.BackBufferFormat = D3DFMT_X8R8G8B8;
				if (FAILED (m_pkD3D->CreateDevice (D3DADAPTER_DEFAULT, (D3DDEVTYPE) iDeviceType, m_hWindow,
												   D3DCREATE_SOFTWARE_VERTEXPROCESSING, &kPresentParams, &m_pkD3DDevice))){
					return ErrorCreateDevice;
				}
			}
		}
		 
	}

	m_iFormat = kPresentParams.BackBufferFormat;

	// Set render states
	m_pkD3DDevice->SetRenderState (D3DRS_CULLMODE, D3DCULL_NONE);
	m_pkD3DDevice->SetRenderState (D3DRS_LIGHTING, FALSE);
	m_pkD3DDevice->SetRenderState (D3DRS_ALPHABLENDENABLE, TRUE);
	m_pkD3DDevice->SetRenderState (D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pkD3DDevice->SetRenderState (D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// Set texture color states
	m_pkD3DDevice->SetTextureStageState (0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pkD3DDevice->SetTextureStageState (0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	return NoError;
}

/******************************************************************************
Clear
Clear the entire screen to a color

Input Parameters:
	Red, Green, Blue, Alpha
Output:
	Error32
******************************************************************************/
Error32 DirectXScreen::Clear (UInt8 iRed, UInt8 iGreen, UInt8 iBlue, UInt8 iAlpha)
{
	UInt32 iColor;
	iColor = D3DCOLOR_RGBA (iRed, iGreen, iBlue, iAlpha);

	// Clear the screen to certain color
	if (FAILED (m_pkD3DDevice->Clear (0, NULL, D3DCLEAR_TARGET, iColor, 0, 0))) {
		return ErrorClear;
	}

	return NoError;
}

/******************************************************************************
StartFrame
Starts rendering the scene/screen.

Input Parameters:
	None

Output:
	Error32
******************************************************************************/
Error32 DirectXScreen::StartFrame (void)
{

	// Start rendering
	if (FAILED (m_pkD3DDevice->BeginScene () )) {
		return ErrorBeginScene;
	}

	return NoError;
}

/******************************************************************************
EndFrame
Stops rendering.

Input Parameters:
	

Output:
	Error32
******************************************************************************/
Error32 DirectXScreen::EndFrame (void)
{
	// End rendering
	if (FAILED(m_pkD3DDevice->EndScene ())){
		return ErrorEndScene;
	}

	// Present data to the screen
	if (FAILED (m_pkD3DDevice->Present (NULL, NULL, NULL, NULL))) {
		return ErrorPresent;
	}

	return NoError;
}

/******************************************************************************
DrawLine
Draw a line from point a to point b.

Input Parameters:
	Point a: x, y
	Point b: x, y
	RGBA

Output:
	Error32
******************************************************************************/
Error32 DirectXScreen::DrawLine (Real32 fX1, Real32 fY1, Real32 fX2, Real32 fY2, UInt8 iRed, UInt8 iGreen, UInt8 iBlue, UInt8 iAlpha)
{
	UInt32 iColor;
	iColor = D3DCOLOR_RGBA (iRed, iGreen, iBlue, iAlpha);

	// Create rectangle vertices
	Vertex kVertices [] =
	{  // x, y, z, w, color, texture coordinates (u,v)
		{fX1, fY1, 0, 1.0f, iColor, 0, 0},
		{fX2, fY2, 0, 1.0f, iColor, 0, 0},
	};

	// Draw the line
	DirectXScreen::GetSingleton ()->GetDevice ()->SetVertexShader (D3DFVF_VERTEX);
	if (FAILED(DirectXScreen::GetSingleton()->GetDevice()->DrawPrimitiveUP(D3DPT_LINELIST, 2, kVertices, sizeof(Vertex)))) {
		return ErrorDrawPrimitive;
	}
	return NoError;
}

/******************************************************************************
DrawRectangle
Draw a rectangle from Point a to Point b

Input Parameters:
	Point a: x, y
	Point b: x, y
	RGBA of the fill color

Output:
	Error32
******************************************************************************/
Error32 DirectXScreen::DrawRectangle (Real32 fX1, Real32 fY1, Real32 fX2, Real32 fY2, UInt8 iRed, UInt8 iGreen, UInt8 iBlue, UInt8 iAlpha)
{
	UInt32 iColor;
	iColor = D3DCOLOR_RGBA (iRed, iGreen, iBlue, iAlpha);
	 
	// Create rectangle vertices
	Vertex kVertices [] =
	{  // x, y, z, w, color, texture coordinates (u,v)
		{fX1, fY1, 0, 1.0f, iColor, 0, 0},
		{fX2, fY1, 0, 1.0f, iColor, 0, 0},
		{fX2, fY2, 0, 1.0f, iColor, 0, 0},
		{fX1, fY2, 0, 1.0f, iColor, 0, 0},
		{fX1, fY1, 0, 1.0f, iColor, 0, 0},
	};
	 
	// Draw the line
	DirectXScreen::GetSingleton()->GetDevice()->SetVertexShader (D3DFVF_VERTEX);
	if (FAILED(DirectXScreen::GetSingleton()->GetDevice()->DrawPrimitiveUP(D3DPT_LINESTRIP, 4, kVertices, sizeof(Vertex)))) {
		return ErrorDrawPrimitive;
	}

	return NoError;
}

/******************************************************************************
DrawCircle
Draws a circle by using many triangles.

Input Parameters:
	Center: x, y
	Radius
	RGBA - the color of the fill
	vertices - the number of triangles to use

Output:
	Error32
******************************************************************************/
Error32 DirectXScreen::DrawCircle (Real32 iCenterX, Real32 iCenterY, Real32 iRadius, UInt8 iRed,
								UInt8 iGreen, UInt8 iBlue, UInt8 iAlpha, UInt32 iVertices)
{
	UInt32 iColor;
	iColor = D3DCOLOR_RGBA (iRed, iGreen, iBlue, iAlpha);
	Vertex * pkVertices;

	// Allocate needed vertices
	pkVertices = new Vertex [iVertices + 1];
	Real32 fAngle = 0;
	Real32 fComplete;
	UInt32 iVertex;

	// Calculate each vertex position 
	for (iVertex = 0; iVertex < iVertices; iVertex ++) {

		// Percentage of circle already drawn
		fComplete = (Real32)iVertex / (Real32)iVertices;
		pkVertices [iVertex].m_fX		= (Real32) ((Real32)iCenterX + ((Real32)iRadius * cos (6.2831f*fComplete)));
		pkVertices [iVertex].m_fY		= (Real32) ((Real32)iCenterY + ((Real32)iRadius * sin (6.2831f*fComplete)));
		pkVertices [iVertex].m_fZ		= 0;
		pkVertices [iVertex].m_fRHW		= 1.0f;
		pkVertices [iVertex].m_iColor	= iColor;
		pkVertices [iVertex].m_ftU		= 0;
		pkVertices [iVertex].m_ftV		= 0;
	}

	// Close the circle
	pkVertices [iVertex].m_fX		= pkVertices [0].m_fX;
	pkVertices [iVertex].m_fY		= pkVertices [0].m_fY;
	pkVertices [iVertex].m_fZ		= 0;
	pkVertices [iVertex].m_fRHW		= 1.0f;
	pkVertices [iVertex].m_iColor	= iColor;
	pkVertices [iVertex].m_ftU		= 0;
	pkVertices [iVertex].m_ftV		= 0;

	// Draw the circle
	DirectXScreen::GetSingleton ()->GetDevice ()->SetVertexShader (D3DFVF_VERTEX);
	if (FAILED(DirectXScreen::GetSingleton()->GetDevice()->DrawPrimitiveUP(D3DPT_LINESTRIP, iVertices, pkVertices, sizeof(Vertex)))) {
		return ErrorDrawPrimitive;
	}

	delete [] pkVertices;
	return NoError;
}

/******************************************************************************
IsModeSupported
Checks to see if mode (width, height, image bits) is supported.

Input Parameters:
	Width
	Height
	Image bits

Output:
	Bool32
******************************************************************************/
Bool32 DirectXScreen::IsModeSupported (UInt16 iWidth, UInt16 iHeight, UInt16 iDepth)
{
	UInt32		iNumberOfModes;
	UInt32		iMode;
	D3DDISPLAYMODE	kMode;

	// Get number of available modes
	iNumberOfModes = m_pkD3D->GetAdapterModeCount (D3DADAPTER_DEFAULT);

	 // For each mode check if mode is equal
	for (iMode = 0; iMode < iNumberOfModes; iMode ++) {
		// Get mode information
		m_pkD3D->EnumAdapterModes (D3DADAPTER_DEFAULT, iMode, &kMode);
		// Compare dimensions
		if ((iWidth == kMode.Width) && (iHeight == kMode.Height)) {
			// Compare bit depth
			if (iDepth == 16) {
				if ((kMode.Format == D3DFMT_R5G6B5) || (kMode.Format == D3DFMT_X1R5G5B5) || (kMode.Format == D3DFMT_A1R5G5B5)) {
					return EN_TRUE;
				}
			} else {
				if ((kMode.Format == D3DFMT_A8R8G8B8) || (kMode.Format == D3DFMT_X8R8G8B8)) {
					return EN_TRUE;
				}  
			}
		}
	}

	return EN_FALSE;
}

// Shows or hides the cursor
void DirectXScreen::ShowCursor (UInt32 iShowCursor)
{
	m_pkD3DDevice->ShowCursor (iShowCursor);
}

// Returns the Direct3D device
LPDIRECT3DDEVICE8 DirectXScreen::GetDevice (void)
{
	return m_pkD3DDevice;
}

// Returns the backbuffer format
UInt32 DirectXScreen::GetFormat (void)
{
	return m_iFormat;
}

/******************************************************************************
GetBitdepth
Returns the bit depth by checking the display mode.

Input Parameters:
	None

Output:
	UInt32
******************************************************************************/
UInt32 DirectXScreen::GetBitdepth (void)
{
	UInt32 iBitdepth;
	switch (m_iFormat) {
		case D3DFMT_R5G6B5:
		case D3DFMT_X1R5G5B5:
		case D3DFMT_A1R5G5B5:
			iBitdepth = 16;
			break;
		case D3DFMT_A8R8G8B8:
		case D3DFMT_X8R8G8B8:
			iBitdepth = 32;
			break;
	}
	return iBitdepth;
}

// Returns the DirectXScreen singleton
DirectXScreen * DirectXScreen::GetSingleton (void)
{
	assert (m_pkSingleton);
	return m_pkSingleton;
}