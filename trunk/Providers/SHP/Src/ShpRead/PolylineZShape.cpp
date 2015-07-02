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
#include "PolylineMShape.h"
#include "PolylineZShape.h"

PolylineZShape::PolylineZShape (int nRecordNumber, void* pMemory, bool bOverlay, int nParts, int nPoints, BoundingBoxEx *box, bool has_m) :
    PolylineMShape (nRecordNumber, pMemory, bOverlay,
        (char*)pMemory
            + (bOverlay ?
                superclass::GetSize (((ESRIPolylineRecord*)pMemory)->nNumParts, ((ESRIPolylineRecord*)pMemory)->nNumPoints) :
                superclass::GetSize (nParts, nPoints)),
        nParts, nPoints, box, has_m),
    mZData ((double*)(GetPoints () + GetNumPoints ())),
    mHasM (has_m)
{
    if (!bOverlay)
    {
        SetShapeType (ePolylineZShape);
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

PolylineZShape* PolylineZShape::NewPolylineZShape (int nRecordNumber, int nParts, int nPoints, BoundingBoxEx *box, bool has_m)
{
    void* p;

    p = new char[PolylineZShape::GetSize (nParts, nPoints, has_m)];
    return (new PolylineZShape (nRecordNumber, p, false, nParts, nPoints, box, has_m));
}

PolylineZShape::~PolylineZShape ()
{
}

FdoByteArray* PolylineZShape::GetGeometry ()
{
    _FDORPT0(0, "PolylineZShape::GetGeometry()\n");

    FdoPtr<FdoFgfGeometryFactory> factory;
    int dimensionality;
    FdoPtr<FdoLineStringCollection> lines;
    FdoPtr<FdoILineString> line;
    FdoPtr<FdoIGeometry> geometry;
    int parts;
    int count;
    int doubles;
    double* points;
    double* xys;
    double* zs;
    double* ms;
    double* p;
    FdoByteArray* ret;

    ret = NULL;

    factory = FdoFgfGeometryFactory::GetInstance ();
    try
    {
        // scan the M array to see if anything has a valid M value
        dimensionality = FdoDimensionality_XY | FdoDimensionality_Z;
        if (HasMeasure ())
            dimensionality |= FdoDimensionality_M;
        lines = FdoLineStringCollection::Create ();
        parts = GetNumParts ();
        for (int i = 0; i < parts; i++)
        {
            count = (i + 1 < parts) ? GetParts ()[i + 1] : GetNumPoints (); // next index
            count -= GetParts ()[i];
            doubles = count * ((0 != (dimensionality & FdoDimensionality_M)) ? 4 : 3);
            points = new double[doubles];
            xys = (double*)&GetPoints ()[GetParts ()[i]];
            zs = &(GetZData ()->GetArray ()[GetParts ()[i]]);
            if (HasMeasure ())
                ms = &(GetMData ()->GetArray ()[GetParts ()[i]]);
            else
                ms = NULL;
            p = points;
            for (int j = 0; j < count; j++)
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
                    p++;
                    ms++;
                }
            }
            line = factory->CreateLineString (dimensionality, doubles, points);
            delete[] points;
            lines->Add (line);
        }
        if (1 < lines->GetCount ())
            geometry = factory->CreateMultiLineString (lines);
        else
            geometry = lines->GetItem (0);
        ret = factory->GetFgf (geometry);
    }
    catch (FdoException* ge)
    {
        FdoException* ex = FdoException::Create (NlsMsgGet(SHP_CREATE_GEOMETRY_FAILED, "Geometry creation failed for '%1$ls'.", L"PolylineZShape"), ge);
        ge->Release ();
        throw ex;
    }
    catch (...)
    {
        throw FdoException::Create (NlsMsgGet(SHP_CREATE_GEOMETRY_FAILED, "Geometry creation failed for '%1$ls'.", L"PolylineZShape"));
    }

    return (ret);
}

void PolylineZShape::DebugPrintDetails ()
{
    try
    {
        _FDORPT0(0, "\n>>>>>>>>>> PolylineZShape Details START <<<<<<<<<<\n");
        superclass::DebugPrintDetails ();

        // Add the extra Z Data details
        double min = GetZData ()->GetRangeMin ();
        double max = GetZData ()->GetRangeMax ();

        _FDORPT2(0, "\nZData Min: %lf  ZData Max: %lf\n",min,max);

        double* pZArray = GetZData ()->GetArray ();
        for (int i = 0; i < GetNumPoints (); i++)
        {
            _FDORPT2(0, "\nZArray[%d] = %lf\n", i, pZArray[i]);
        }

        _FDORPT0(0, "\n>>>>>>>>>> PolylineZShape Details END <<<<<<<<<<\n\n");
    }
    catch (...)
    {
        _FDORPT0(0, ">>>>>>>>>> DebugPrintDetails() - EXCEPTION <<<<<<<<<<\n");
    }
}


