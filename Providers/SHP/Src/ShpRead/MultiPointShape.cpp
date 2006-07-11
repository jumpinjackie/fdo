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
#include "MultiPointShape.h"

MultiPointShape::MultiPointShape (int nRecordNumber, void* pMemory, bool bOverlay, int nPoints, BoundingBoxEx *box) :
    Shape (nRecordNumber, pMemory, bOverlay, (DoublePoint*)(((ESRIMultiPointRecord*)pMemory) + 1)),
    mRecord ((ESRIMultiPointRecord*)pMemory)
{
    if (!bOverlay)
    {
        SetShapeType (eMultiPointShape);
        if (NULL != box)
            mRecord->boundingBox = *box;
        else
        {
            DoublePoint none (fNO_DATA, fNO_DATA);
            mRecord->boundingBox = BoundingBox (&none);
        }
        _ASSERT(nPoints >= 1);
        mRecord->nNumPoints = nPoints;
        int count = GetNumPoints ();
        DoublePoint* points = GetPoints ();
        for (int i = 0; i < count; i++)
        {
            points[i].x = 0.0;
            points[i].y = 0.0;
        }
    }
    else
        _ASSERT(GetNumPoints () >= 1);
}

MultiPointShape* MultiPointShape::NewMultiPointShape (int nRecordNumber, int nPoints, BoundingBoxEx *box)
{
    void* p;

    p = new char[MultiPointShape::GetSize (nPoints)];
    return (new MultiPointShape (nRecordNumber, p, false, nPoints, box));
}

MultiPointShape::~MultiPointShape ()
{
}

FdoByteArray* MultiPointShape::GetGeometry ()
{
    _FDORPT0(0, "MultiPointShape::GetGeometry()\n");

    FdoPtr<FdoFgfGeometryFactory> factory;
    FdoPtr<FdoIGeometry> geometry;
    FdoByteArray* ret;

    ret = NULL;

    factory = FdoFgfGeometryFactory::GetInstance ();
    try
    {
        if (GetNumPoints () > 1)
            geometry = factory->CreateMultiPoint (FdoDimensionality_XY, GetNumPoints () * 2, (double*)GetPoints ());
        else
            geometry = factory->CreatePoint (FdoDimensionality_XY, (double*)GetPoints ());
        ret = factory->GetFgf (geometry);
    }
    catch (FdoException* ge)
    {
        FdoException* ex = FdoException::Create (NlsMsgGet(SHP_CREATE_GEOMETRY_FAILED, "Geometry creation failed for '%1$ls'.", L"MultiPointShape"), ge);
        ge->Release ();
        throw ex;
    }
    catch (...)
    {
        throw FdoException::Create (NlsMsgGet(SHP_CREATE_GEOMETRY_FAILED, "Geometry creation failed for '%1$ls'.", L"MultiPointShape"));
    }

    return (ret);
}

void MultiPointShape::DebugPrintDetails ()
{
// ****************************************************************************
// * D E B U G  B L O C K
// ****************************************************************************
#ifdef _DEBUG
    try
    {
        _FDORPT0(0, "\n>>>>>>>>>> MultiPointShape Details START <<<<<<<<<<\n");
        _FDORPT1(0, "Total Vertices: %d\n\n", GetNumPoints ());

        for (int nIndex = 0; nIndex < GetNumPoints (); nIndex++)
        {
            _FDORPT3(0, "V: %ld  x = %lf  y = %lf\n", nIndex + 1, (GetPoints ()[nIndex]).x, (GetPoints ()[nIndex]).y);
        }
        _FDORPT0(0, "\n>>>>>>>>>> MultiPointShape Details END <<<<<<<<<<\n\n");
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

