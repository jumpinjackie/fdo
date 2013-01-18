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
#include "MultiPointMShape.h"

MultiPointMShape::MultiPointMShape (int nRecordNumber, void* pMemory, bool bOverlay, void* pMValues, int nPoints, BoundingBoxEx *box, bool initialize_m) :
    MultiPointShape (nRecordNumber, pMemory, bOverlay, nPoints, box),
    mMData ((NULL == pMValues) ? (GetPoints () + (bOverlay ? mRecord->nNumPoints : nPoints)) : pMValues)
{
    if (!bOverlay)
    {
        SetShapeType (eMultiPointMShape);
        if (initialize_m)
        {
            if (NULL != box)
            {
                GetMData ()->SetRangeMin (box->mMin);
                GetMData ()->SetRangeMax (box->mMax);
            }
            else
            {
                GetMData ()->SetRangeMin (fNO_DATA);
                GetMData ()->SetRangeMax (fNO_DATA);
            }
            int count = GetNumPoints ();
            double* ms = GetMData ()->GetArray ();
            for (int i = 0; i < count; i++)
                ms[i] = 0.0;
        }
    }
}

MultiPointMShape* MultiPointMShape::NewMultiPointMShape (int nRecordNumber, int nPoints, BoundingBoxEx *box, bool initialize_m)
{
    void* p;

    p = new char[MultiPointMShape::GetSize (nPoints)];
    return (new MultiPointMShape (nRecordNumber, p, false, NULL, nPoints, box, initialize_m));
}

MultiPointMShape::~MultiPointMShape ()
{
}

FdoByteArray* MultiPointMShape::GetGeometry ()
{
    _FDORPT0(0, "MultiPointMShape::GetGeometry()\n");

    FdoPtr<FdoFgfGeometryFactory> factory;
    FdoPtr<FdoIGeometry> geometry;
    double* xys;
    double* ms;
    double* points;
    double* p;
    double point[3];
    FdoByteArray* ret;

    ret = NULL;

    factory = FdoFgfGeometryFactory::GetInstance ();
    try
    {
        if (GetNumPoints () > 1)
        {
            xys = (double*)GetPoints ();
            ms = GetMData ()->GetArray ();
            points = new double[GetNumPoints () * 3];
            p = points;
            for (int i = 0; i < GetNumPoints (); i++)
            {
                *p = *xys;
                p++;
                xys++;
                *p = *xys;
                p++;
                xys++;
                *p = *ms;
                p++;
                ms++;
            }
            geometry = factory->CreateMultiPoint (FdoDimensionality_XY | FdoDimensionality_M, GetNumPoints () * 3, points);
            delete[] points;
        }
        else
        {
            point[0] = GetPoints ()->x;
            point[1] = GetPoints ()->y;
            point[2] = GetMData ()->GetArray ()[0];
            geometry = factory->CreatePoint (FdoDimensionality_XY | FdoDimensionality_M, point);
            ret = factory->GetFgf (geometry);
        }
        ret = factory->GetFgf (geometry);
    }
    catch (FdoException* ge)
    {
        FdoException* ex = FdoException::Create (NlsMsgGet(SHP_CREATE_GEOMETRY_FAILED, "Geometry creation failed for '%1$ls'.", L"MultiPointMShape"), ge);
        ge->Release ();
        throw ex;
    }
    catch (...)
    {
        throw FdoException::Create (NlsMsgGet(SHP_CREATE_GEOMETRY_FAILED, "Geometry creation failed for '%1$ls'.", L"MultiPointMShape"));
    }

    return (ret);
}

void MultiPointMShape::DebugPrintDetails ()
{
    try
    {
        _FDORPT0(0, "\n>>>>>>>>>> MultiPointMShape Details START <<<<<<<<<<\n");
        superclass::DebugPrintDetails ();

        // Add the extra M Data details
        double min = GetMData ()->GetRangeMin ();
        double max = GetMData ()->GetRangeMax ();

        _FDORPT2(0, "\nMData Min: %lf  MData Max: %lf\n",min,max);

        double* pMArray = GetMData ()->GetArray ();
        for (int i = 0; i < GetNumPoints (); i++)
        {
            _FDORPT2(0, "\nMArray[%d] = %lf\n", i, pMArray[i]);
        }

        _FDORPT0(0, "\n>>>>>>>>>> MultiPointMShape Details END <<<<<<<<<<\n\n");
    }
    catch (...)
    {
        _FDORPT0(0, ">>>>>>>>>> DebugPrintDetails() - EXCEPTION <<<<<<<<<<\n");
    }
}



