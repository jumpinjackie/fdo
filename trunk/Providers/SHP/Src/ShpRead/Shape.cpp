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

#include "stdafx.h"
#include "Shape.h"

Shape::Shape (int nRecordNumber, void* pMemory, bool bOverlay, DoublePoint* pPoints) :
    mRecordNumber (nRecordNumber),
    mMemory (pMemory),
    mOverlay (bOverlay),
    mPoints (pPoints)
{
	// _ASSERT(NULL != pMemory); Corrupted geometries will be ignored (turned into NullShape)
    // _ASSERT(NULL != pPoints); NullShape does this
}

Shape::~Shape ()
{
    if (!mOverlay)
        delete[] (char*)mMemory;
}

