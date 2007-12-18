#ifndef _SQLSERVEROVSTDAFX_H_
#define _SQLSERVEROVSTDAFX_H_
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#ifdef _WIN32
#pragma once

#define WIN32_LEAN_AND_MEAN     // Exclude rarely-used stuff from Windows headers

// TODO: reference additional headers your program requires here

#include <windows.h>
#endif
#include <stdio.h>
#include <Fdo.h>

#include <Rdbms/Override/RdbmsOv.h>
#include <Rdbms/Override/SqlServer/SqlServerOv.h>

#endif

