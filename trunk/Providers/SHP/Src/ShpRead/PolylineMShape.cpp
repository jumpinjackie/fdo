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
#include "PolylineShape.h"
#include "PolylineMShape.h"

PolylineMShape::PolylineMShape (int nRecordNumber, void* pMemory, bool bOverlay, void* pMValues, int nParts, int nPoints, BoundingBoxEx *box, bool initialize_m) :
    PolylineShape (nRecordNumber, pMemory, bOverlay, nParts, nPoints, box),
    mMData ((NULL != pMValues) ? pMValues : (double*)(GetPoints () + GetNumPoints ()))
{
    if (!bOverlay)
    {
        SetShapeType (ePolylineMShape);
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

PolylineMShape* PolylineMShape::NewPolylineMShape (int nRecordNumber, int nParts, int nPoints, BoundingBoxEx *box, bool initialize_m)
{
    void* p;

    p = new char[PolylineMShape::GetSize (nParts, nPoints)];
    return (new PolylineMShape (nRecordNumber, p, false, NULL, nParts, nPoints, box, initialize_m));
}

PolylineMShape::~PolylineMShape ()
{
}

FdoByteArray* PolylineMShape::GetGeometry ()
{
    _FDORPT0(0, "PolylineMShape::GetGeometry()\n");

    FdoPtr<FdoFgfGeometryFactory> factory;
    FdoPtr<FdoLineStringCollection> lines;
    FdoPtr<FdoILineString> line;
    FdoPtr<FdoIGeometry> geometry;
    int parts;
    int count;
    double* points;
    double* xys;
    double* ms;
    double* p;
    FdoByteArray* ret;

    ret = NULL;

    factory = FdoFgfGeometryFactory::GetInstance ();
    try
    {
        lines = FdoLineStringCollection::Create ();
        parts = GetNumParts ();
        for (int i = 0; i < parts; i++)
        {
            count = (i + 1 < parts) ? GetParts ()[i + 1] : GetNumPoints (); // next index
            count -= GetParts ()[i];
            points = new double[count * 3];
            xys = (double*)&GetPoints ()[GetParts ()[i]];
            ms = &(GetMData ()->GetArray ()[GetParts ()[i]]);
            p = points;
            for (int j = 0; j < count; j++)
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
            line = factory->CreateLineString (FdoDimensionality_XY | FdoDimensionality_M, count * 3, points);
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
        FdoException* ex = FdoException::Create (NlsMsgGet(SHP_CREATE_GEOMETRY_FAILED, "Geometry creation failed for '%1$ls'.", L"PolylineMShape"), ge);
        ge->Release ();
        throw ex;
    }
    catch (...)
    {
        throw FdoException::Create (NlsMsgGet(SHP_CREATE_GEOMETRY_FAILED, "Geometry creation failed for '%1$ls'.", L"PolylineMShape"));
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
void PolylineMShape::DebugPrintDetails ()
{
    try
    {
        _FDORPT0(0, "\n>>>>>>>>>> PolylineMShape Details START <<<<<<<<<<\n");
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

        _FDORPT0(0, "\n>>>>>>>>>> PolylineMShape Details END <<<<<<<<<<\n\n");
    }
    catch (...)
    {
        _FDORPT0(0, ">>>>>>>>>> DebugPrintDetails() - EXCEPTION <<<<<<<<<<\n");
    }
}

