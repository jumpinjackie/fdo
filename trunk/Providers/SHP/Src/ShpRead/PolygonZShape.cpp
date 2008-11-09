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
#include "PolygonMShape.h"
#include "PolygonZShape.h"

#include <FdoSpatial.h>

PolygonZShape::PolygonZShape (int nRecordNumber, void* pMemory, bool bOverlay, int nParts, int nPoints, BoundingBoxEx *box, bool has_m) :
    PolygonMShape (nRecordNumber, pMemory, bOverlay,
        (char*)pMemory
            + (bOverlay ?
                superclass::GetSize (((ESRIPolygonRecord*)pMemory)->nNumParts, ((ESRIPolygonRecord*)pMemory)->nNumPoints) :
                superclass::GetSize (nParts, nPoints)),
        nParts, nPoints, box, has_m),
    mZData ((double*)(GetPoints () + GetNumPoints ())),
    mHasM (has_m)
{
    if (!bOverlay)
    {
        SetShapeType (ePolygonZShape);
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

PolygonZShape* PolygonZShape::NewPolygonZShape (int nRecordNumber, int nParts, int nPoints, BoundingBoxEx *box, bool has_m)
{
    void *p;

    p = new char[PolygonZShape::GetSize (nParts, nPoints, has_m)];
    return (new PolygonZShape (nRecordNumber, p, false, nParts, nPoints, box, has_m));
}

PolygonZShape::~PolygonZShape ()
{
}

FdoByteArray* PolygonZShape::GetGeometry ()
{
    _FDORPT0(0, "PolygonZShape::GetGeometry()\n");

    FdoPtr<FdoFgfGeometryFactory> factory;
    int dimensionality;
    FdoPtr<FdoLinearRingCollection> rings;
    FdoPtr<FdoPolygonCollection> polygons;
    FdoPtr<FdoILinearRing> exteriorRing;
    FdoPtr<FdoILinearRing> ring;
    FdoPtr<FdoIPolygon> polygon;
    FdoPtr<FdoIGeometry> geometry;
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
        rings = FdoLinearRingCollection::Create (); 
        for (int i = 0; i < GetNumParts (); i++)
        {
            count = (i + 1 < GetNumParts ()) ? GetParts ()[i + 1] : GetNumPoints (); // next index
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
            ring = factory->CreateLinearRing (dimensionality, doubles, points);

            if (!RELATE_RINGS && (i==0))
                exteriorRing = FDO_SAFE_ADDREF(ring.p);
            else
                rings->Add (ring);

            delete[] points;
        }

        if (RELATE_RINGS)
            // Might return a Polygon or a MultiPolygon
            geometry = FdoSpatialUtility::CreateGeometryFromRings (rings, RELATE_RINGS);
        else
            // Always returns a Polygon
            geometry = factory->CreatePolygon(exteriorRing, rings);

        ret = factory->GetFgf (geometry);
    }
    catch (FdoException* ge)
    {
        FdoException* ex = FdoException::Create (NlsMsgGet(SHP_CREATE_GEOMETRY_FAILED, "Geometry creation failed for '%1$ls'.", L"PolygonZShape"), ge);
        ge->Release ();
        throw ex;
    }
    catch (...)
    {
        throw FdoException::Create (NlsMsgGet(SHP_CREATE_GEOMETRY_FAILED, "Geometry creation failed for '%1$ls'.", L"PolygonZShape"));
    }

    return (ret);
}

void PolygonZShape::DebugPrintDetails ()
{
    try
    {
        _FDORPT0(0, "\n>>>>>>>>>> PolygonZShape Details START <<<<<<<<<<\n");
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

        _FDORPT0(0, "\n>>>>>>>>>> PolygonZShape Details END <<<<<<<<<<\n\n");
    }
    catch (...)
    {
        _FDORPT0(0, ">>>>>>>>>> DebugPrintDetails() - EXCEPTION <<<<<<<<<<\n");
    }
}

