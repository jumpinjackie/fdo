// 
//  
//  Copyright (C) 2008 Autodesk Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  

#ifndef STDAFX_H
#define STDAFX_H

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <io.h>
#endif

#include <float.h>
#include <time.h>

//disable STL bounds checking
#define _SECURE_SCL 0

#include <cstring>
#include <string>
#include <wchar.h>

//FDO headers
#include "Fdo.h"
#include "FdoCommonStringUtil.h"
#include "FdoSpatial.h"

//Redefine the FDO smart pointer so that we use our own 
//which skips some null checks on the operator overloads
#define FdoPtr SltFdoPtr
#include "SltFdoPtr.h"

//SQLite headers
extern "C"
{
#include "sqlite3.h"
#include "sqliteInt.h"
}

/*
*                                THE RESTRICT CONTRACT
*
* I, [insert your name], a PROFESSIONAL or AMATEUR [circle one] programmer recognize
* that there are limits to what a compiler can do. I certify that, to the best of my 
* knowledge, there are no magic elves or monkeys in the compiler which through the 
* forces of fairy dust can always make code faster. I understand that there are some 
* problems for which there is not enough information to solve. I hereby declare that 
* given the opportunity to provide the compiler with sufficient information, perhaps 
* through some key word, I will gladly use said keyword and not complain about how
* "the compiler should be doing this for me."
*
* In this case, I promise that the pointer declared along with the restrict qualifier 
* is not aliased. I certify that writes through this pointer will not affect the values
* read through any other pointer available in the same context which is also declared 
* as restricted.
*
* Your agreement to this contract is implied by use of the restrict keyword. 
*/
#ifndef _MSC_VER
  #define __restrict __restrict__
  #define __forceinline __inline__
  #define __fastcall 
  #define _wcsdup wcsdup
  #define _snprintf snprintf
  #define _wcsicmp wcscasecmp
  #define _access(x,y) access(x,y)
  typedef long long int __int64;
#endif

#include "slt.h"

#pragma warning (disable: 4100)

#endif
