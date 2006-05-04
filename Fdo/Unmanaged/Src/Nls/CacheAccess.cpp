/*
 * 
* Copyright (C) 2004-2006  Autodesk, Inc.
* 
* This library is free software; you can redistribute it and/or
* modify it under the terms of version 2.1 of the GNU Lesser
* General Public License as published by the Free Software Foundation.
* 
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
* 
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
* 
 */

#ifdef _WIN32

#include <windows.h>
#include "CacheAccess.h"

// the sole purpose of this class is to have one static instance,
// the constructor of which initializes the critical section structure
// and cleans up on exit

extern "C" { CRITICAL_SECTION CacheCriticalSection; }

static CacheAccess Initializer;

CacheAccess::CacheAccess (void)
{
    InitializeCriticalSection (&CacheCriticalSection);
}

CacheAccess::~CacheAccess (void)
{
    DeleteCriticalSection (&CacheCriticalSection);
}

#endif // _WIN32
