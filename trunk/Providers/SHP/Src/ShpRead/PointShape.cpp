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
#include "PointShape.h"

PointShape::PointShape (int nRecordNumber, void* pMemory, bool bOverlay, BoundingBoxEx *box) :
    Shape (nRecordNumber, pMemory, bOverlay, (DoublePoint*)((char*)pMemory + sizeof (int)))
{
    if (!bOverlay)
    {
        SetShapeType (ePointShape);
        if (NULL != box)
            *GetBoundingBox () = *box;
        else
        {
            DoublePoint none (fNO_DATA, fNO_DATA);
            mBoundingBox = BoundingBox (&none);
        }
        GetPoints ()->x = 0.0;
        GetPoints ()->y = 0.0;
    }
    else
        mBoundingBox = BoundingBox (GetPoints ());
}

PointShape* PointShape::NewPointShape (int nRecordNumber, BoundingBoxEx *box)
{
    void* p;

    p = new char[PointShape::GetSize ()];
    return (new PointShape (nRecordNumber, p, false, box));
}

PointShape::~PointShape ()
{
}

FdoByteArray* PointShape::GetGeometry ()
{
    _FDORPT0(0, "PointShape::GetGeometry()\n");

    FdoPtr<FdoFgfGeometryFactory> factory;
    FdoPtr<FdoIGeometry> geometry;
    FdoByteArray* ret;

    ret = NULL;

    factory = FdoFgfGeometryFactory::GetInstance ();
    try
    {
        geometry = factory->CreatePoint (FdoDimensionality_XY, (double*)GetPoints ());
        ret = factory->GetFgf (geometry);
    }
    catch (FdoException* ge)
    {
        FdoException* ex = FdoException::Create (NlsMsgGet(SHP_CREATE_GEOMETRY_FAILED, "Geometry creation failed for '%1$ls'.", L"PointShape"), ge);
        ge->Release ();
        throw ex;
    }
    catch (...)
    {
        throw FdoException::Create (NlsMsgGet(SHP_CREATE_GEOMETRY_FAILED, "Geometry creation failed for '%1$ls'.", L"PointShape"));
    }

    return (ret);
}

void PointShape::DebugPrintDetails ()
{
// ****************************************************************************
// * D E B U G  B L O C K
// ****************************************************************************
#ifdef _DEBUG
    try
    {
        _FDORPT0(0, "\n>>>>>>>>>> PointShape Details START <<<<<<<<<<\n");
        _FDORPT1(0, "Total Vertices: %d\n\n", GetNumPoints ());

        for (int i = 0; i < GetNumPoints (); i++)
        {
            _FDORPT3(0, "V: %ld  x = %lf  y = %lf\n", i,(GetPoints ()[i]).x, (GetPoints ()[i]).y);
        }
        _FDORPT0(0, "\n>>>>>>>>>> PointShape Details END <<<<<<<<<<\n\n");
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

