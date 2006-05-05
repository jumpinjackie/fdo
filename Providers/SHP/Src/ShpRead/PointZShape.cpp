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
#include "PointMShape.h"
#include "PointZShape.h"

PointZShape::PointZShape (int nRecordNumber, void* pMemory, bool bOverlay, BoundingBoxEx *box, bool has_m) :
    PointMShape (nRecordNumber, pMemory, bOverlay, (char*)pMemory + superclass::GetSize (), box),
    mZData ((char*)pMemory + superclass::superclass::GetSize ()),
    mHasM (has_m)
{
    if (!bOverlay)
    {
        SetShapeType (ePointZShape);
        // for point types, the Z data points at the (only) Z value,
        // so there are no separate range and array to initialize
        if (NULL != box)
            GetZData ()->GetArray ()[0] = box->zMin;
        else
            GetZData ()->GetArray ()[0] = 0.0;
    }
}

PointZShape* PointZShape::NewPointZShape (int nRecordNumber, BoundingBoxEx *box, bool has_m)
{
    void* p;

    p = new char[PointZShape::GetSize ()];
    return (new PointZShape (nRecordNumber, p, false, box, has_m));
}

PointZShape::~PointZShape ()
{
}

FdoByteArray* PointZShape::GetGeometry ()
{
    ATLTRACE(L"PointZShape::GetGeometry()\n");

    FdoPtr<FdoFgfGeometryFactory> factory;
    int dimensionality;
    DoublePoint* points;
    double point[4];
    FdoPtr<FdoIGeometry> geometry;
    FdoByteArray* ret;

    ret = NULL;

    factory = FdoFgfGeometryFactory::GetInstance ();
    dimensionality = FdoDimensionality_XY | FdoDimensionality_Z;
    try
    {
        points = GetPoints ();
        point[0] = points->x;
        point[1] = points->y;
        point[2] = GetZData ()->GetArray ()[0];
        point[3] = GetMData ()->GetArray ()[0];
        if (point[3] > fNO_DATA)
            dimensionality |= FdoDimensionality_M;
        geometry = factory->CreatePoint (dimensionality, point);
        ret = factory->GetFgf (geometry);
    }
    catch (FdoException* ge)
    {
        FdoException* ex = FdoException::Create (NlsMsgGet(SHP_CREATE_GEOMETRY_FAILED, "Geometry creation failed for '%1$ls'.", L"PointZShape"), ge);
        ge->Release ();
        throw ex;
    }
    catch (...)
    {
        throw FdoException::Create (NlsMsgGet(SHP_CREATE_GEOMETRY_FAILED, "Geometry creation failed for '%1$ls'.", L"PointZShape"));
    }

    return (ret);
}

/*****************************************************************************
 * Name         : DebugPrintDetails
 *
 * Description  : This method outputs debugging geometry information.
 *
 * Parameters   :
 *
 * Return       : N/A
 *
 * Notes        : N/A
 *
 *****************************************************************************/
void PointZShape::DebugPrintDetails ()
{
    try
    {
        ATLTRACE(L"\n>>>>>>>>>> PointZShape Details START <<<<<<<<<<\n");
        superclass::DebugPrintDetails ();

        // Add the extra Z Data details
        double min = GetZData ()->GetRangeMin ();
        double max = GetZData ()->GetRangeMax ();

        ATLTRACE(L"\nZData Min: %lf  ZData Max: %lf\n", min, max);

        double* pZArray = GetZData ()->GetArray ();
        for (int i = 0; i < GetNumPoints (); i++)
            ATLTRACE(L"\nZArray[%d] = %lf\n", i, pZArray[i]);

        ATLTRACE(L"\n>>>>>>>>>> PointZShape Details END <<<<<<<<<<\n\n");
    }
    catch (...)
    {
        ATLTRACE(L">>>>>>>>>> DebugPrintDetails() - EXCEPTION <<<<<<<<<<\n");
    }
}

