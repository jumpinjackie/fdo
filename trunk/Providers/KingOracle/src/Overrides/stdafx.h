/*
 * Copyright (C) 2006  SL-King d.o.o
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
 */

#ifndef KGORAOVERRIDES_STDAFX_H
#define KGORAOVERRIDES_STDAFX_H

#ifdef _WIN32
#pragma once

#define WIN32_LEAN_AND_MEAN        // Exclude rarely-used stuff from Windows headers

#include <windows.h>
#pragma warning(disable: 4100)  // Disable warning C4100: 'xxxx' : unreferenced formal parameter
#endif
#include <Fdo.h>
#include "../Provider/FDO_31_Types.h"
#include <KingOracle/fdokgoraprovider.h>
//#include <FdoCommonMiscUtil.h>
#include "FdoKgOraXmlGlobals.h"
#include "FdoCommonOSUtil.h"



#endif 


