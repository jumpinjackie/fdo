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
#include "PointMShape.h"

PointMShape::PointMShape (int nRecordNumber, void* pMemory, bool bOverlay, void* pMValues, BoundingBoxEx *box) :
    PointShape (nRecordNumber, pMemory, bOverlay, box),
    mMData ((NULL != pMValues) ? pMValues : ((char*)pMemory + superclass::GetSize ()))
{
    if (!bOverlay)
    {
        SetShapeType (ePointMShape);
        // for point types, the M data points at the (only) M value
        // so there are no separate range and array to initialize
        if (NULL != box)
            GetMData ()->GetArray ()[0] = box->mMin;
        else
            GetMData ()->GetArray ()[0] = 0.0;
    }
}

PointMShape* PointMShape::NewPointMShape (int nRecordNumber, BoundingBoxEx *box)
{
    void* p;

    p = new char[PointMShape::GetSize ()];
    return (new PointMShape (nRecordNumber, p, false, NULL, box));
}

PointMShape::~PointMShape ()
{
}

FdoByteArray* PointMShape::GetGeometry ()
{
    ATLTRACE(L"PointMShape::GetGeometry()\n");

    FdoPtr<FdoFgfGeometryFactory> factory;
    FdoPtr<FdoIGeometry> geometry;
    DoublePoint* points;
    double point[3];
    FdoByteArray* ret;

    ret = NULL;

    factory = FdoFgfGeometryFactory::GetInstance ();
    try
    {
        points = GetPoints ();
        point[0] = points->x;
        point[1] = points->y;
        point[2] = GetMData ()->GetArray ()[0];
        geometry = factory->CreatePoint (FdoDimensionality_XY | FdoDimensionality_M, point);
        ret = factory->GetFgf (geometry);
    }
    catch (FdoException* ge)
    {
        FdoException* ex = FdoException::Create (NlsMsgGet(SHP_CREATE_GEOMETRY_FAILED, "Geometry creation failed for '%1$ls'.", L"PointMShape"), ge);
        ge->Release ();
        throw ex;
    }
    catch (...)
    {
        throw FdoException::Create (NlsMsgGet(SHP_CREATE_GEOMETRY_FAILED, "Geometry creation failed for '%1$ls'.", L"PointMShape"));
    }

    return (ret);
}

void PointMShape::DebugPrintDetails ()
{
    try
    {
        ATLTRACE(L"\n>>>>>>>>>> PointMShape Details START <<<<<<<<<<\n");
        superclass::DebugPrintDetails ();

        // Add the extra M Data details
        double min = GetMData ()->GetRangeMin ();
        double max = GetMData ()->GetRangeMax ();

        ATLTRACE(L"\nMData Min: %lf  MData Max: %lf\n",min,max);

        double* pMArray = GetMData ()->GetArray ();
        for (int i = 0; i < GetNumPoints (); i++)
            ATLTRACE(L"\nMArray[%d] = %lf\n", i, pMArray[i]);

        ATLTRACE(L"\n>>>>>>>>>> PointMShape Details END <<<<<<<<<<\n\n");
    }
    catch (...)
    {
        ATLTRACE(L">>>>>>>>>> DebugPrintDetails() - EXCEPTION <<<<<<<<<<\n");
    }
}


