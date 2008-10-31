/******************************************************************************
Error.h

Created for: Tetris

Copyright (C) 2006-2007 Oliver Zheng.
All rights reserved. No part of this software may be used or reproduced in
any form by any means without prior written permission.

An enumeration of errors for DirectDraw
******************************************************************************/

#include <string>

using namespace std;

#pragma once

// Error codes
enum Error32 {
	NoError = 0,

	ErrorRegisterClass,
	ErrorInitDirect3D,
	ErrorGetAdapterDisplayMode,
	ErrorCreateDevice,
	ErrorClear,
	ErrorBeginScene,
	ErrorEndScene,
	ErrorPresent,
	ErrorDrawPrimitive,
	ErrorNotBitmapFile,
	ErrorTargaNotSupported,
	ErrorBitmapNotSupported,
	ErrorInvalidRawImage,
	ErrorCreateImageSurface,
	ErrorCreateTexture,
	ErrorDInputCreate,
	ErrorKeyboardCreateDevice,
	ErrorKeyboardSetDataFormat,
	ErrorKeyboardSetCooperativeLevel,
	ErrorKeyboardAcquire,
	ErrorKeyboardGetDeviceData,

	Error32_Force32 = 0xFFFFFFFF,
};
