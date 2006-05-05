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
#include "PolygonShape.h"

#include <Spatial.h>

PolygonShape::PolygonShape (int nRecordNumber, void* pMemory, bool bOverlay, int nParts, int nPoints, BoundingBoxEx *box) :
    PolyShape (nRecordNumber, pMemory, bOverlay,
        (int*)(((ESRIPolygonRecord*)pMemory) + 1),
        (DoublePoint*)(((int*)(((ESRIPolygonRecord*)pMemory) + 1)) + (bOverlay ? ((ESRIPolygonRecord*)pMemory)->nNumParts : nParts))),
    mRecord ((ESRIPolygonRecord*)pMemory)
{
    if (!bOverlay)
    {
        SetShapeType (ePolygonShape);
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

PolygonShape* PolygonShape::NewPolygonShape (int nRecordNumber, int nParts, int nPoints, BoundingBoxEx *box)
{
    void* p;

    p = new char[PolygonShape::GetSize (nParts, nPoints)];
    return (new PolygonShape (nRecordNumber, p, false, nParts, nPoints, box));
}

PolygonShape::~PolygonShape ()
{
}

FdoByteArray* PolygonShape::GetGeometry ()
{
    ATLTRACE(L"PolygonShape::GetGeometry()\n");

    FdoPtr<FdoFgfGeometryFactory> factory;
    FdoPtr<FdoLinearRingCollection> rings;
    FdoPtr<FdoILinearRing> exteriorRing;
    FdoPtr<FdoILinearRing> ring;
    FdoPtr<FdoIGeometry> geometry;
    int parts;
    int count;
    FdoByteArray* ret;

    ret = NULL;

    factory = FdoFgfGeometryFactory::GetInstance ();
    try
    {
        rings = FdoLinearRingCollection::Create ();
        parts = GetNumParts ();
        for (int i = 0; i < parts; i++)
        {
            count = (i + 1 < parts) ? GetParts ()[i + 1] : GetNumPoints (); // next index
            count -= GetParts ()[i];
            try
            {
                if (count == 2)
                {   // this happens often enough in the Sheboygan data to make an effort here
#pragma message ("TODO: Revisit bad shape policy and revise shape classes as appropriate.")
                    double* doubles;

                    doubles = (double*)&GetPoints ()[GetParts ()[i]];
                    if ((doubles[0] != doubles[2]) || (doubles[1] != doubles[3]))
                    {
                        double points[6];
                        memmove (points, doubles, 4 * sizeof (double));
                        points[4] = doubles[0];
                        points[5] = doubles[1];
                        ring = factory->CreateLinearRing (FdoDimensionality_XY, (count + 1) * 2, points);
                    }
                    else
                        ring = factory->CreateLinearRing (FdoDimensionality_XY, count * 2, (double*)&GetPoints ()[GetParts ()[i]]);
                }
                else
                    ring = factory->CreateLinearRing (FdoDimensionality_XY, count * 2, (double*)&GetPoints ()[GetParts ()[i]]);

                if (!RELATE_RINGS && (i==0))
                    exteriorRing = FDO_SAFE_ADDREF(ring.p);
                else
                    rings->Add (ring);
            }
            catch (FdoException* ge)
            {
                // ignore this bad interior ring
                // I wish there were a logging facility so the user could be warned about this stuff
                ge->Release ();
            }
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
        FdoException* ex = FdoException::Create (NlsMsgGet(SHP_CREATE_GEOMETRY_FAILED, "Geometry creation failed for '%1$ls'.", L"PolygonShape"), ge);
        ge->Release ();
        throw ex;
    }
    catch (...)
    {
        throw FdoException::Create (NlsMsgGet(SHP_CREATE_GEOMETRY_FAILED, "Geometry creation failed for '%1$ls'.", L"PolygonShape"));
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
void PolygonShape::DebugPrintDetails ()
{
// ****************************************************************************
// * D E B U G  B L O C K
// ****************************************************************************
#ifdef _DEBUG
    try
    {
        ATLTRACE(L"\n>>>>>>>>>> PolygonShape Details START <<<<<<<<<<\n");
        ATLTRACE(L"Total Polygons: %d  Vertices: %d\n", GetNumParts (), GetNumPoints ());

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


            ATLTRACE(L"\nPolygon: %d  Total Vertices: %d\n\n",nParts+1,nNumPoints);

            nPreviousPoint = GetParts ()[nParts+1];
            int nStartPoint = GetParts ()[nParts];

            for (int nIndex=0;nIndex<nNumPoints;nIndex++)
            {
                ATLTRACE(L"V: %ld  x = %lf  y = %lf\n",nIndex+1,(GetPoints ()[nStartPoint + nIndex]).x,(GetPoints ()[nStartPoint + nIndex]).y);
            }
        }

        ATLTRACE(L"\n>>>>>>>>>> PolygonShape Details END <<<<<<<<<<\n\n");
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


