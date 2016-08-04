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

#include <FdoSpatial.h>

#define GET_ITEM(p, i, xptr, yptr)  \
        {                  \
            double dummy1; \
            int dim_ring;  \
            (p)->GetItemByMembers(i, xptr, yptr, &dummy1, &dummy1, &dim_ring); \
        }

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
        mRecord->nNumParts = nParts;
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
    _FDORPT0(0, "PolygonShape::GetGeometry()\n");

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
#pragma message ("TODO: Revisit shape policy and revise shape classes as appropriate.")
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
            // Might return a Polygon or a MultiPolygon. Use the SHP custom method for relating rings.
            geometry = CreateGeometryFromRings (rings, RELATE_RINGS);
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

//
// The following code is cloned from FdoSpatialUtility::PointInRing(). 
// (Refer the original method for extensive commnents on the altgorithm)
// Optimized it for speed by skipping the "on boundary" test because 
// a) it is expensive and b) we assume the rings are well formed and 
// the chosen point is not on the ring.

bool PolygonShape::PointInRing(FdoILinearRing* ring, double x, double y)
{
    bool yflag0, yflag1;
    double vtx0X, vtx0Y, vtx1X, vtx1Y;

    bool isInside = false;
    int count = 0;
   
    int numverts = ring->GetCount();

    // get the last point in the contour
    GET_ITEM(ring, numverts-1, &vtx0X, &vtx0Y);

    // get test bit for above/below X axis
    yflag0 = (vtx0Y >= y);

    for (int j = 0; j < numverts; j++)
    {
        GET_ITEM(ring, j, &vtx1X, &vtx1Y);
       
        yflag1 = (vtx1Y >= y);

        if (yflag0 != yflag1)
        {
            if (((vtx1Y-y)*(vtx0X-vtx1X) >=
                (vtx1X-x)*(vtx0Y-vtx1Y)) == yflag1)
            {
                isInside  = !isInside;
            }
        }

        // move to the next pair of vertices, retaining info as possible
        yflag0 = yflag1;
        vtx0X = vtx1X;
        vtx0Y = vtx1Y;
    }

    return isInside;
} 

//
// This method has been cloned from FdoSpatialUtility::CreateGeometryFromRings() which assumes the rings
// are totally unrelated. For this reason the number of point-in-polygon tests can be very large leading
// to an unacceptable performance problem. In addition it is based on a sort by area approach which also
// adds overhead for area calculation.
//
// For SHP we can assume safely that the 1st ring is outer and the following rings are inner. Since the
// rings orientation is not reliable we'll need to perform a point-in-polygon test for each candidate 
// inner loop against the outer loop. When such a candidate is found outside the outer loop then we'll
// start another outer loop and so on. The result will be a polygon or a multipolygon, depending on how
// many outer loops have been found.
//
FdoIGeometry* PolygonShape::CreateGeometryFromRings( FdoLinearRingCollection* rings, bool relateRings )
{
    FdoPtr<FdoIPolygon>         polygon;
    FdoPtr<FdoIMultiPolygon>    multipolygon;

    FdoIGeometry                *geometry;
    FdoFgfGeometryFactory       *factory = FdoFgfGeometryFactory::GetInstance ();

    FdoInt32    numRings = rings->GetCount(); 
    
    // Just one ring case, quick exit.
    if ( numRings == 1 )
    {
        FdoPtr<FdoILinearRing> ring = rings->GetItem(0);

        polygon = factory->CreatePolygon( ring, NULL );
        geometry = polygon;

        FDO_SAFE_ADDREF( geometry );
        FDO_SAFE_RELEASE( factory );

        return geometry;
    }

    FdoPtr<FdoLinearRingCollection> intRings = FdoLinearRingCollection::Create (); 
    FdoPtr<FdoPolygonCollection>    polygons = FdoPolygonCollection::Create ();

    // Relate (associate) rings: find exterior/interior rings
    // Ignore the rings orientation (cw/ccw) 

    // Array to hold the ring association by index
    int  *ringAssocIndex = new int[numRings];
    for ( int i = 0; i < numRings; i++ )
    { 
        ringAssocIndex[i] = -1; // not associated
    }

    // Navigate forward starting from the 2nd ring. Try to associate the current ring with the current external loop:
    // - inside -> it is a hole
    // - outside -> it is a external loop

    int extRingIndex = 0; 
    FdoPtr<FdoILinearRing> extRing = rings->GetItem(extRingIndex);
    FdoPtr<FdoIEnvelope>   extent = extRing->GetEnvelope();

    for ( int i = 1; i < numRings; i++)
    { 
        FdoPtr<FdoILinearRing> ring = rings->GetItem(i);

        // Pick the mid point of the segment instead of 1st point for "point-in-ring" testing. 
        // This checks unambiguosly "on boundary" cases where the rings are touching.
        double x0, y0, x1, y1;
        GET_ITEM(ring, 0, &x0, &y0);

        double x = x1 = x0;
        double y = y1 = y0;
        int  k = 1;

        while ((x0 == x1) && (y0 == y1) && (k < ring->GetCount() - 1))
        {
            GET_ITEM(ring, k, &x1, &y1);  
            k++;
        }

        // Take the mid of the segment
        x = (x0 + x1)/2;
        y = (y0 + y1)/2;

        // Check the point against the ring bounding box
        bool isInside = (x <= extent->GetMaxX() && x >= extent->GetMinX() && y <= extent->GetMaxY() && y >= extent->GetMinY());

        if (isInside) 
            isInside = PointInRing(extRing, x, y);

        if (isInside) 
        {
            ringAssocIndex[i] = extRingIndex;
        }
        else
        {
            // Start a new external loop
            extRingIndex = i; 
            extRing = rings->GetItem(extRingIndex);
            extent = extRing->GetEnvelope();
        }
    }

    // At this point all the rings have been associated (holes). Those not associated are external loops.
    for ( int i = 0; i < numRings; i++)
    { 
        // External ring, look for its associated rings
        _ASSERT(ringAssocIndex[i] == -1 );

        FdoInt32  extRingIndex = i; 
        FdoPtr<FdoILinearRing> extRing = rings->GetItem(extRingIndex);

        FdoPtr<FdoLinearRingCollection> intRings = FdoLinearRingCollection::Create (); 

        bool  related = true;
        for (int j = i + 1; j < numRings && related; j++)
        { 
            related = (ringAssocIndex[j] == extRingIndex);
            if ( related )
            {
                FdoPtr<FdoILinearRing> intRing = rings->GetItem(j);
                intRings->Add( intRing );
                i++;  
            }
        }

        // Given a polygon with interior rings we need to check for nested rings. 
        bool    hasNestedRings = false;

        if (RELATE_NESTED_RINGS && intRings->GetCount() > 1)
        {
            // Because the nested rings are rare, it is worth doing a separate test, presumably cheap 
            // (because we assume the bounding box test will eliminate lots of candidates): exclude the external ring,
            // take all the interior rings and try to relate them. In case the number of resulting polygons is
            // different from the number of interior rings, then we have nested rings and we'll do full association.
            // Note: "a nested ring" refers to an exterior ring that is inside the interior ring of a surrounding 
            // polygon (at least two concentric polygons, separated by unoccupied space).

            // Returns a Polygon or a MultiPolygon
            FdoPtr<FdoIGeometry> geom = FdoSpatialUtility::CreateGeometryFromRings (intRings, true);
            FdoGeometryType      gtype = geom->GetDerivedType();

            _ASSERT(gtype == FdoGeometryType_Polygon || gtype == FdoGeometryType_MultiPolygon);
            if (gtype == FdoGeometryType_Polygon)
            {
                // Just one external loop with rings
                hasNestedRings = true;
            }
            else if (gtype == FdoGeometryType_MultiPolygon)
            {
                // Check the number of polygons. 
                FdoIMultiPolygon*  multiPoly = (FdoIMultiPolygon *) (geom.p);
                hasNestedRings = (intRings->GetCount() > multiPoly->GetCount());          
            }

            if (hasNestedRings)
            {
                // Create a temporary collection of rings
                FdoPtr<FdoLinearRingCollection> tempRings = FdoLinearRingCollection::Create (); 
                tempRings->Add(extRing);

                for (int i = 0; i < intRings->GetCount(); i++)
                {
                    FdoPtr<FdoILinearRing> intRing = intRings->GetItem(i);
                    tempRings->Add(intRing);
                }
                
                // Do true rings association
                FdoPtr<FdoIGeometry> geom = FdoSpatialUtility::CreateGeometryFromRings (tempRings, true);

                if (geom->GetDerivedType() == FdoGeometryType_MultiPolygon)
                {
                    FdoIMultiPolygon* multiPoly = static_cast<FdoIMultiPolygon *>(geom.p);

                    // Add these polygons to the output
                    for (int i = 0; i < multiPoly->GetCount(); i++)
                    {
                        FdoPtr<FdoIPolygon> polygon = multiPoly->GetItem(i);            
                        polygons->Add (polygon);
                    }
                }
                else
                {
                    // This should never happen
                    _ASSERT(geom->GetDerivedType() == FdoGeometryType_MultiPolygon);  
                    hasNestedRings = false;  
                }
            }
        }
        
        if (!hasNestedRings)
        {
            // Polygon with one or no interior loops
            polygon = factory->CreatePolygon( extRing, intRings );
            polygons->Add (polygon);
        }
    }

    delete[] ringAssocIndex;

    // Check the number of polyons created and return the appropiate geometry type.
    geometry = ( polygons->GetCount() == 1 ) ? (FdoIGeometry *) polygons->GetItem(0) :
                                               (FdoIGeometry *) factory->CreateMultiPolygon (polygons);
    FDO_SAFE_RELEASE( factory );

    return geometry;
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
        _FDORPT0(0, "\n>>>>>>>>>> PolygonShape Details START <<<<<<<<<<\n");
        _FDORPT2(0, "Total Polygons: %d  Vertices: %d\n", GetNumParts (), GetNumPoints ());

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


            _FDORPT2(0, "\nPolygon: %d  Total Vertices: %d\n\n",nParts+1,nNumPoints);

            nPreviousPoint = GetParts ()[nParts+1];
            int nStartPoint = GetParts ()[nParts];

            for (int nIndex=0;nIndex<nNumPoints;nIndex++)
            {
                _FDORPT3(0, "V: %ld  x = %lf  y = %lf\n",nIndex+1,(GetPoints ()[nStartPoint + nIndex]).x,(GetPoints ()[nStartPoint + nIndex]).y);
            }
        }

        _FDORPT0(0, "\n>>>>>>>>>> PolygonShape Details END <<<<<<<<<<\n\n");
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



