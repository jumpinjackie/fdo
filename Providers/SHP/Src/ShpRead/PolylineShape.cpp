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

PolylineShape::PolylineShape (int nRecordNumber, void* pMemory, bool bOverlay, int nParts, int nPoints, BoundingBoxEx *box) :
    PolyShape (nRecordNumber, pMemory, bOverlay,
        (int*)(((ESRIPolylineRecord*)pMemory) + 1),
        (DoublePoint*)(((int*)(((ESRIPolylineRecord*)pMemory) + 1)) + (bOverlay ? ((ESRIPolylineRecord*)pMemory)->nNumParts : nParts))),
    mRecord ((ESRIPolylineRecord*)pMemory)
{
    if (!bOverlay)
    {
        SetShapeType (ePolylineShape);
        if (NULL != box)
            mRecord->boundingBox = *box;
        else
        {
            DoublePoint none (fNO_DATA, fNO_DATA);
            mRecord->boundingBox = BoundingBox (&none);
        }
        _ASSERT(nParts >= 1);
        mRecord->nNumParts = nParts;
        _ASSERT(nPoints >= 1);
        mRecord->nNumPoints = nPoints;
        int* parts = GetParts ();
        for (int i = 0; i < nParts; i++)
            parts[i] = 0;
        DoublePoint* points = GetPoints ();
        for (int i = 0; i < nPoints; i++)
        {
            points[i].x = 0.0;
            points[i].y = 0.0;
        }
    }
    else
    {
        _ASSERT(GetNumParts () >= 1);
        _ASSERT(GetNumPoints () >= 1);
    }
}

PolylineShape* PolylineShape::NewPolylineShape (int nRecordNumber, int nParts, int nPoints, BoundingBoxEx *box)
{
    void* p;

    p = new char[PolylineShape::GetSize (nParts, nPoints)];
    return (new PolylineShape (nRecordNumber, p, false, nParts, nPoints, box));
}

PolylineShape::~PolylineShape ()
{
}

FdoByteArray* PolylineShape::GetGeometry ()
{
    ATLTRACE(L"PolylineShape::GetGeometry()\n");

    FdoPtr<FdoFgfGeometryFactory> factory;
    FdoPtr<FdoLineStringCollection> lines;
    FdoPtr<FdoILineString> line;
    FdoPtr<FdoIGeometry> geometry;
    int parts;
    int count;
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
            line = factory->CreateLineString (FdoDimensionality_XY, count * 2, (double*)&GetPoints ()[GetParts ()[i]]);
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
        FdoException* ex = FdoException::Create (NlsMsgGet(SHP_CREATE_GEOMETRY_FAILED, "Geometry creation failed for '%1$ls'.", L"PolylineShape"), ge);
        ge->Release ();
        throw ex;
    }
    catch (...)
    {
        throw FdoException::Create (NlsMsgGet(SHP_CREATE_GEOMETRY_FAILED, "Geometry creation failed for '%1$ls'.", L"PolylineShape"));
    }

    return (ret);
}

void PolylineShape::DebugPrintDetails ()
{
// ****************************************************************************
// * D E B U G  B L O C K
// ****************************************************************************
#ifdef _DEBUG
    try
    {
        ATLTRACE(L"\n>>>>>>>>>> PolylineShape Details START <<<<<<<<<<\n");
        ATLTRACE(L"Total Polylines: %d  Vertices: %d\n", GetNumParts (), GetNumPoints ());

        int nPreviousPoint = 0;
        for (int nParts = 0; nParts < GetNumParts (); nParts++)
        {
            int nNumPoints = 0;

            if (GetNumParts () > 1)
            {
                if (nParts == (GetNumParts () - 1))
                {
                    // At the end
                    nNumPoints = GetNumPoints () - GetParts ()[GetNumParts () - 1];
                }
                else
                {
                    nNumPoints = GetParts ()[nParts+1] - nPreviousPoint;
                }
            }
            else
            {
                nNumPoints = GetNumPoints ();
            }


            ATLTRACE(L"\nPolyline: %d  Total Vertices: %d\n\n",nParts+1,nNumPoints);

            nPreviousPoint = GetParts ()[nParts+1];
            int nStartPoint = GetParts ()[nParts];

            for (int nIndex = 0; nIndex < nNumPoints; nIndex++)
            {
                ATLTRACE(L"V: %ld  x = %lf  y = %lf\n", nIndex + 1, (GetPoints ()[nStartPoint + nIndex]).x, (GetPoints ()[nStartPoint + nIndex]).y);
            }
        }

        ATLTRACE(L"\n>>>>>>>>>> PolylineShape Details END <<<<<<<<<<\n\n");
    }
    catch (...)
    {
        ATLTRACE(L">>>>>>>>>> DebugPrintDetails() - EXCEPTION <<<<<<<<<<\n");
    }
#endif
// ****************************************************************************
// * D E B U G  B L O C K
// ****************************************************************************
}



