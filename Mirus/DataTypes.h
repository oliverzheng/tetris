/******************************************************************************
DataTypes.h

Created for: Tetris

Copyright (C) 2006-2007 Oliver Zheng.
All rights reserved. No part of this software may be used or reproduced in
any form by any means without prior written permission.

Typedef's of common datatypes
******************************************************************************/

#include <string>
#include "Error.h"

using namespace std;

#pragma once

// Basic type definitions
typedef	char			Int8;
typedef	unsigned char	UInt8;
typedef	short			Int16;
typedef	unsigned short	UInt16;
typedef	long			Int32;
typedef	unsigned long	UInt32;
typedef	unsigned int	UInt;

// Floating definitions
typedef	float			Real32;
typedef	double			Real64;

// Composed definitions
enum Bool32 {
	EN_FALSE	= 0,
	EN_TRUE		= 1,

	Bool32_Force32 = 0xFFFFFFFF,
};