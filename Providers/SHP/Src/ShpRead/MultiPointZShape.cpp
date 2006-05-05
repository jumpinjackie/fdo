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
#include "MultiPointMShape.h"
#include "MultiPointZShape.h"

MultiPointZShape::MultiPointZShape (int nRecordNumber, void* pMemory, bool bOverlay, int nPoints, BoundingBoxEx *box, bool has_m) :
    MultiPointMShape (nRecordNumber, pMemory, bOverlay,
        (char*)pMemory + superclass::GetSize (bOverlay ? ((ESRIMultiPointRecord*)pMemory)->nNumPoints : nPoints),
        nPoints, box, has_m),
    mZData (GetPoints () + (bOverlay ? mRecord->nNumPoints : nPoints)),
    mHasM (has_m)
{
    if (!bOverlay)
    {
        SetShapeType (eMultiPointZShape);
        if (NULL != box)
        {
            GetZData ()->SetRangeMin (box->zMin);
            GetZData ()->SetRangeMax (box->zMax);
        }
        else
        {
            GetZData ()->SetRangeMin (fNO_DATA);
            GetZData ()->SetRangeMax (fNO_DATA);
        }
        int count = GetNumPoints ();
        double* zs = GetZData ()->GetArray ();
        for (int i = 0; i < count; i++)
            zs[i] = 0.0;
    }
}

MultiPointZShape* MultiPointZShape::NewMultiPointZShape (int nRecordNumber, int nPoints, BoundingBoxEx *box, bool has_m)
{
    void* p;

    p = new char[MultiPointZShape::GetSize (nPoints, has_m)];
    return (new MultiPointZShape (nRecordNumber, p, false, nPoints, box, has_m));
}

MultiPointZShape::~MultiPointZShape ()
{
}

FdoByteArray* MultiPointZShape::GetGeometry ()
{
    ATLTRACE(L"MultiPointZShape::GetGeometry()\n");

    FdoPtr<FdoFgfGeometryFactory> factory;
    int dimensionality;
    FdoPtr<FdoIGeometry> geometry;
    double* xys;
    double* zs;
    double* ms;
    double* points;
    double* p;
    double point[4];
    FdoByteArray* ret;

    ret = NULL;

    factory = FdoFgfGeometryFactory::GetInstance ();
    dimensionality = FdoDimensionality_XY | FdoDimensionality_Z;
    try
    {
        if (GetNumPoints () > 1)
        {
            xys = (double*)GetPoints ();
            zs = GetZData ()->GetArray ();
            if (HasMeasure ())
                ms = GetMData ()->GetArray ();
            else
                ms = NULL;
            points = new double[GetNumPoints () * 4];
            p = points;
            for (int i = 0; i < GetNumPoints (); i++)
            {
                *p = *xys;
                p++;
                xys++;
                *p = *xys;
                p++;
                xys++;
                *p = *zs;
                p++;
                zs++;
                if (HasMeasure ())
                {
                    *p = *ms;
                    if (fNO_DATA < *ms)
                        dimensionality |= FdoDimensionality_M;
                    p++;
                    ms++;
                }
            }
            if (0 == (dimensionality & FdoDimensionality_M) && HasMeasure ())
            {   // no M, but we included M's because HasMeasure() said to, so remove them
                p = points;
                xys = points;
                for (int i = 0; i < GetNumPoints (); i++)
                {
                    *xys = *p;
                    p++;
                    xys++;
                    *xys = *p;
                    p++;
                    xys++;
                    *xys = *p;
                    p++;
                    xys++;
                    p++;
                }
            }
            geometry = factory->CreateMultiPoint (dimensionality, GetNumPoints () * (0 == (dimensionality & FdoDimensionality_M) ? 3 : 4), points);
            delete[] points;
        }
        else
        {
            point[0] = GetPoints ()->x;
            point[1] = GetPoints ()->y;
            point[2] = GetZData ()->GetArray ()[0];
            if (HasMeasure ())
            {
                point[3] = GetMData ()->GetArray ()[0];
                if (fNO_DATA < point[3])
                    dimensionality |= FdoDimensionality_M;
            }
            geometry = factory->CreatePoint (dimensionality, point);
        }
        ret = factory->GetFgf (geometry);
    }
    catch (FdoException* ge)
    {
        FdoException* ex = FdoException::Create (NlsMsgGet(SHP_CREATE_GEOMETRY_FAILED, "Geometry creation failed for '%1$ls'.", L"MultiPointZShape"), ge);
        ge->Release ();
        throw ex;
    }
    catch (...)
    {
        throw FdoException::Create (NlsMsgGet(SHP_CREATE_GEOMETRY_FAILED, "Geometry creation failed for '%1$ls'.", L"MultiPointZShape"));
    }

    return (ret);
}

void MultiPointZShape::DebugPrintDetails ()
{
    try
    {
        ATLTRACE(L"\n>>>>>>>>>> MultiPointZShape Details START <<<<<<<<<<\n");
        superclass::DebugPrintDetails ();

        // Add the extra Z Data details
        double min = GetZData ()->GetRangeMin ();
        double max = GetZData ()->GetRangeMax ();

        ATLTRACE(L"\nZData Min: %lf  ZData Max: %lf\n",min,max);

        double* pZArray = GetZData ()->GetArray ();
        if (pZArray)
        {
            for (int i = 0; i < GetNumPoints (); i++)
            {
                ATLTRACE(L"\nZArray[%d] = %lf\n", i, pZArray[i]);
            }
        }

        ATLTRACE(L"\n>>>>>>>>>> MultiPointZShape Details END <<<<<<<<<<\n\n");
    }
    catch (...)
    {
        ATLTRACE(L">>>>>>>>>> DebugPrintDetails() - EXCEPTION <<<<<<<<<<\n");
    }
}

