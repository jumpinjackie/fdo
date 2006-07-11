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
#include "NullShape.h"

NullShape::NullShape (int nRecordNumber, void* pMemory, bool bOverlay) :
    Shape (nRecordNumber, pMemory, bOverlay, NULL)
{
    if (!bOverlay)
        SetShapeType (eNullShape);
}

NullShape* NullShape::NewNullShape (int nRecordNumber)
{
    void* p;

    p = new char[NullShape::GetSize ()];
    return (new NullShape (nRecordNumber, p, false));
}

NullShape::~NullShape ()
{
}

FdoByteArray* NullShape::GetGeometry ()
{
    _FDORPT0(0, "NullShape::GetGeometry()\n");

    return (NULL);
}

void NullShape::DebugPrintDetails ()
{
// ****************************************************************************
// * D E B U G  B L O C K
// ****************************************************************************
#ifdef _DEBUG
    try
    {
        _FDORPT0(0, "\n>>>>>>>>>> NullShape Details START <<<<<<<<<<\n");
        _FDORPT0(0, "\nNO DETAILS");
        _FDORPT0(0, "\n>>>>>>>>>> NullShape Details END <<<<<<<<<<\n\n");
    }
    catch (...)
    {
        _FDORPT0(0, ">>>>>>>>>> DebugPrintDetails() - EXCEPTION <<<<<<<<<<\n");
    }
#endif
// ****************************************************************************
// * D E B U G  B L O C K
// ****************************************************************************
}

