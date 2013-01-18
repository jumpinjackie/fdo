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
#include "PolygonZShape.h"
#include "MultiPatchShape.h"
#include <vector>

MultiPatchShape::MultiPatchShape (int nRecordNumber, void* pMemory, bool bOverlay, int nParts, int nPoints, BoundingBoxEx* box, bool has_m) :
    PolygonZShape (nRecordNumber, pMemory, bOverlay, nParts, nPoints, box, has_m),
    mPartTypes (GetParts () + GetNumParts ())
{
    // fix up addresses cause by the injection of the part-type array before the points array
    // this *maybe* could be done in the constructor, but it's more than my job's worth
    // MultiPatch:
    // Byte 0 Shape Type 31 Integer 1 Little
    // Byte 4 Box Box Double 4 Little
    // Byte 36 NumParts NumParts Integer 1 Little
    // Byte 40 NumPoints NumPoints Integer 1 Little
    // Byte 44 Parts Parts Integer NumParts Little
    // ->Byte W PartTypes PartTypes Integer NumParts Little
    // +=Byte X Points Points Point NumPoints Little
    // +=Byte Y Zmin Zmin Double 1 Little
    // +=Byte Y+8 Zmax Zmax Double 1 Little
    // +=Byte Y+16 Zarray Zarray Double NumPoints Little
    // +=Byte Z* Mmin Mmin Double 1 Little
    // +=Byte Z+8* Mmax Mmax Double 1 Little
    // +=Byte Z+16* Marray Marray Double NumPoints Little
    size_t offset = GetNumParts () * sizeof (int);
    mPartTypes = (int*)mPoints;
    mPoints = (DoublePoint*)((char*)mPoints + offset);
    GetZData ()->SetRange ((double*)((char*)GetZData ()->GetRange () + offset));
    GetZData ()->SetArray ((double*)((char*)GetZData ()->GetArray () + offset));
    GetMData ()->SetRange ((double*)((char*)GetMData ()->GetRange () + offset));
    GetMData ()->SetArray ((double*)((char*)GetMData ()->GetArray () + offset));

    if (!bOverlay)
    {
        SetShapeType (eMultiPatchShape);
        for (int i = 0; i < GetNumParts (); i++)
            GetPartTypes ()[i] = NOT_IN_RING_COMPLEX;
        // redo the superclass initialization
        if (NULL != box)
        {
            GetZData ()->SetRangeMin (box->zMin);
            GetZData ()->SetRangeMax (box->zMax);
            GetMData ()->SetRangeMin (box->mMin);
            GetMData ()->SetRangeMax (box->mMax);
        }
        else
        {
            GetZData ()->SetRangeMin (fNO_DATA);
            GetZData ()->SetRangeMax (fNO_DATA);
            GetMData ()->SetRangeMin (fNO_DATA);
            GetMData ()->SetRangeMax (fNO_DATA);
        }
        int count = GetNumPoints ();
        double* zs = GetZData ()->GetArray ();
        double* ms = GetMData ()->GetArray ();
        for (int i = 0; i < count; i++)
        {
            zs[i] = 0.0;
            ms[i] = 0.0;
        }
    }
}

MultiPatchShape* MultiPatchShape::NewMultiPatchShape (int nRecordNumber, int nParts, int nPoints, BoundingBoxEx* box, bool has_m)
{
    void *p;

    p = new char[MultiPatchShape::GetSize (nParts, nPoints, has_m)];
    return (new MultiPatchShape (nRecordNumber, p, false, nParts, nPoints, box, has_m));
}

MultiPatchShape::~MultiPatchShape ()
{
}

FdoByteArray* MultiPatchShape::GetGeometry ()
{
    _FDORPT0(0, "MultiPatchShape::GetGeometry()\n");

    FdoPtr<FdoFgfGeometryFactory> factory;
    int dimensionality;
    FdoPtr<FdoLinearRingCollection> rings;
    FdoPtr<FdoPolygonCollection> polygons;
    FdoPtr<FdoILinearRing> ring;
    FdoPtr<FdoIPolygon> polygon;
    FdoPtr<FdoLinearRingCollection> finalInnerRings = FdoLinearRingCollection::Create();
    FdoPtr<FdoILinearRing> finalOuterRing;
    FdoPtr<FdoIGeometry> geometry;
    State state;
    int count;
    double* xys;
    double* zs;
    double* ms;
    double* points;
    double triangle[12]; // 3 points of x,y,z,m
    int index;
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
        state = NOT_IN_RING_COMPLEX;
        rings = FdoLinearRingCollection::Create ();
        polygons = FdoPolygonCollection::Create ();
        for (int i = 0; i < GetNumParts (); i++)
        {
            count = (i + 1 < GetNumParts ()) ? GetParts ()[i + 1] : GetNumPoints (); // next index
            count -= GetParts ()[i];
            xys = (double*)&GetPoints ()[GetParts ()[i]];
            zs = &(GetZData ()->GetArray ())[GetParts ()[i]];
            if (HasMeasure ())
                ms = &(GetMData ()->GetArray ())[GetParts ()[i]];
            else
                ms = NULL;
            switch (GetPartTypes ()[i])
            {
                // A linked strip of triangles, where every vertex (after the first two)
                // completes a new triangle. A new triangle is always formed by
                // connecting the new vertex with its two immediate predecessors.
                case eTriangleStripPartType:
                    FinishPolygon (finalOuterRing.p, finalInnerRings, state, rings);
                    for (int j = 0; j < count - 2; j++)
                    {
                        // arrange points in triangle array (X,Y,Z,M)
                        index = 0;
                        triangle[index++] = *(xys + 0);
                        triangle[index++] = *(xys + 1);
                        triangle[index++] = *(zs + 0);
                        if (HasMeasure ()) triangle[index++] = *(ms + 0);
                        triangle[index++] = *(xys + 2);
                        triangle[index++] = *(xys + 3);
                        triangle[index++] = *(zs + 1);
                        if (HasMeasure ()) triangle[index++] = *(ms + 1);
                        triangle[index++] = *(xys + 4);
                        triangle[index++] = *(xys + 5);
                        triangle[index++] = *(zs + 2);
                        if (HasMeasure ()) triangle[index++] = *(ms + 2);
                        ring = factory->CreateLinearRing (dimensionality, index, triangle);
                        AddRings(finalOuterRing.p, finalInnerRings, ring, rings);
                        xys += 2; // x,y
                        zs++;
                        if (HasMeasure ()) ms++;
                    }
                    break;

                // A linked fan of triangles, where every vertex (after the first two)
                // completes a new triangle. A new triangle is always formed by
                // connecting the new vertex with its immediate predecessor and the
                // first vertex of the part.
                case eTriangleFanPartType:
                    FinishPolygon (finalOuterRing.p, finalInnerRings, state, rings);
                    for (int j = 0; j < count - 2; j++)
                    {
                        // arrange points in triangle array (X,Y,Z,M)
                        if (0 == j)
                        {
                            index = 0;
                            triangle[index++] = *(xys + 0);
                            triangle[index++] = *(xys + 1);
                            triangle[index++] = *(zs + 0);
                            if (HasMeasure ()) triangle[index++] = *(ms + 0);
                            xys += 2; // x,y
                            zs++;
                            if (HasMeasure ()) ms++;
                        }
                        else
                            if (HasMeasure ())
                                index = 4;
                            else
                                index = 3;
                        triangle[index++] = *(xys + 0);
                        triangle[index++] = *(xys + 1);
                        triangle[index++] = *(zs + 0);
                        if (HasMeasure ()) triangle[index++] = *(ms + 0);
                        triangle[index++] = *(xys + 2);
                        triangle[index++] = *(xys + 3);
                        triangle[index++] = *(zs + 1);
                        if (HasMeasure ()) triangle[index++] = *(ms + 1);
                        ring = factory->CreateLinearRing (dimensionality, index, triangle);
                        AddRings(finalOuterRing.p, finalInnerRings, ring, rings);
                        xys += 2; // x,y
                        zs++;
                        if (HasMeasure ()) ms++;
                    }
                    break;
                case eOuterRingPartType:
                    FinishPolygon (finalOuterRing.p, finalInnerRings, state, rings);
                    // fall through
                case eInnerRingPartType:
                    state = IN_OUTER_RING_COMPLEX;
                    points = new double[count * 4];
                    p = points;
                    index = 0;
                    for (int j = 0; j < count; j++)
                    {
                        // arrange points in points array (X,Y,Z,M)
                        *p = *xys;
                        index++;
                        p++;
                        xys++;
                        *p = *xys;
                        index++;
                        p++;
                        xys++;
                        *p = *zs;
                        index++;
                        p++;
                        zs++;
                        if (HasMeasure ())
                        {
                            *p = *ms;
                            index++;
                            p++;
                            ms++;
                        }
                    }
                    ring = factory->CreateLinearRing (dimensionality, index, points);
                    delete[] points;
                    rings->Add (ring);
                    break;
                case eFirstRingPartType:
                    FinishPolygon (finalOuterRing.p, finalInnerRings, state, rings);
                    state = IN_FIRST_RING_COMPLEX;
                    // fall through
                case eRingPartType:
                    if (IN_FIRST_RING_COMPLEX != state)
                        FinishPolygon (finalOuterRing.p, finalInnerRings, state, rings);
                    points = new double[count * 4]; 
                    p = points;
                    index = 0;
                    for (int j = 0; j < count; j++)
                    {
                        // arrange points in points array (X,Y,Z,M)
                        *p = *xys;
                        index++;
                        p++;
                        xys++;
                        *p = *xys;
                        index++;
                        p++;
                        xys++;
                        *p = *zs;
                        index++;
                        p++;
                        zs++;
                        if (HasMeasure ())
                        {
                            *p = *ms;
                            index++;
                            p++;
                            ms++;
                        }
                    }
                    ring = factory->CreateLinearRing (dimensionality, index, points);
                    delete[] points;
                    rings->Add (ring);
                    if (IN_FIRST_RING_COMPLEX != state)
                        FinishPolygon (finalOuterRing.p, finalInnerRings, state, rings);
                    break;
                default:
                    // No idea what this is
                    break;
            }
        }

        // finish up any pending polygon
        FinishPolygon (finalOuterRing.p, finalInnerRings, state, rings);

        // Create the final geometry:
        geometry = factory->CreatePolygon(finalOuterRing, finalInnerRings);
        /* The old code would potentially generate multipolygons, which are not allowed by ECO 10400:
        if (1 < polygons->GetCount ())
            geometry = factory->CreateMultiPolygon (polygons);
        else
            geometry = polygons->GetItem (0);
        */
        ret = factory->GetFgf (geometry);
    }
    catch (FdoException* ge)
    {
        FdoException* ex = FdoException::Create (NlsMsgGet(SHP_CREATE_GEOMETRY_FAILED, "Geometry creation failed for '%1$ls'.", L"MultiPatchShape"), ge);
        ge->Release ();
        throw ex;
    }
    catch (...)
    {
        throw FdoException::Create (NlsMsgGet(SHP_CREATE_GEOMETRY_FAILED, "Geometry creation failed for '%1$ls'.", L"MultiPatchShape"));
    }

    return (ret);
}



void MultiPatchShape::FinishPolygon (FdoILinearRing*& finalOuterRing, FdoLinearRingCollection* finalInnerRings, State& state, FdoLinearRingCollection* rings)
{
    FdoPtr<FdoFgfGeometryFactory> factory;
    FdoPtr<FdoILinearRing> ring;
    FdoPtr<FdoIPolygon> polygon;

    if (0 != rings->GetCount ())
    {
        factory = FdoFgfGeometryFactory::GetInstance ();
        ring = rings->GetItem (0);
        rings->RemoveAt (0);
        if (IN_OUTER_RING_COMPLEX == state) // create outer ring complex
        {
            AddRings(finalOuterRing, finalInnerRings, ring, rings);
        }
        else if (IN_FIRST_RING_COMPLEX == state) // create first ring complex
        {
    #pragma message ("ToDo: handle first ring in multi-patch shape")
            // for now we assume that the first ring is the outer ring
            AddRings(finalOuterRing, finalInnerRings, ring, rings);
        }
        else if (NOT_IN_RING_COMPLEX == state) // create simple ring polygon
        {
            if (0 != rings->GetCount ())
                rings->Clear (); // uh-oh!
            AddRings(finalOuterRing, finalInnerRings, ring, rings);
        }
    }
    state = NOT_IN_RING_COMPLEX;
    rings->Clear ();
}

void MultiPatchShape::DebugPrintDetails ()
{
// ****************************************************************************
// * D E B U G  B L O C K
// ****************************************************************************
#ifdef _DEBUG
    try
    {
        _FDORPT0(0, "\n>>>>>>>>>> MultiPatchShape Details START <<<<<<<<<<\n");
        _FDORPT2(0, "Total Patches: %d  Vertices: %d\n", GetNumParts (), GetNumPoints ());

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
                    nNumPoints = GetParts ()[nParts + 1] - nPreviousPoint;
                }
            }
            else
            {
                nNumPoints = GetNumPoints ();
            }


            _FDORPT3(0, "\nPatch: %d  Total Vertices: %d  Patch Type: %d\n\n", GetNumParts () + 1, GetNumPoints (), GetPartTypes ()[nParts]);

            nPreviousPoint = GetParts ()[nParts+1];
            int nStartPoint = GetParts ()[nParts];

            for (int nIndex = 0; nIndex < nNumPoints; nIndex++)
            {
                _FDORPT3(0, "V: %ld  x = %lf  y = %lf\n", nIndex + 1, (GetPoints ()[nStartPoint + nIndex]).x,(GetPoints ()[nStartPoint + nIndex]).y);
            }
        }

        _FDORPT0(0, "\n>>>>>>>>>> MultiPatchShape Details END <<<<<<<<<<\n\n");
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


