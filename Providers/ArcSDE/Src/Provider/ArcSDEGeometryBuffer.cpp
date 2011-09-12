/*
 * Copyright (C) 2011.  Oyvind Idland.
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
 */

#include "stdafx.h"

ArcSDEGeometryBuffer::ArcSDEGeometryBuffer() :
    mGeomXY(),
    mGeomPointsZ(),
    mGeomPointsM(),
    mGeomOrdinates(),
    mGeomOffsets(),
    mGeomPartOffsets(),
    mGeomSubpartOffsets(),
    mbHasM(false),
    mbHasZ(false)
{
    mShapeType = 0L;
    mNumParts = 0;
    mNumSubParts = 0;
    mNumPoints = 0;
}


bool ArcSDEGeometryBuffer::GetPointsSegment(int part, int subPart, long &numPointsForSubPart, long &offset)
{
    int subPartIdx = mGeomPartOffsets[part] + subPart;
    offset = mGeomSubpartOffsets[subPartIdx];
    numPointsForSubPart = mGeomSubpartOffsets[subPartIdx +1] - mGeomSubpartOffsets[subPartIdx];
    return true;
}

void ArcSDEGeometryBuffer::InitBuffers(FdoInt32 dim, int totalNumPoints, SE_POINT** points, LFLOAT** zBuffer, LFLOAT** mBuffer)
{
    // Allocate arrays for XY, Z, and M (as required):
    mGeomXY.SetSize(totalNumPoints);

    *points = mGeomXY.GetAddr();
    *zBuffer = NULL;
    *mBuffer = NULL;

    if (dim & FdoDimensionality_Z)
    {
        mGeomPointsZ.SetSize(totalNumPoints);
        *zBuffer = mGeomPointsZ.GetAddr();
    }
    if (dim & FdoDimensionality_M)
    {
        mGeomPointsM.SetSize(totalNumPoints);
        *mBuffer = mGeomPointsM.GetAddr();
    }
}



int ArcSDEGeometryBuffer::numSubParts(int partIdx)
{
    int numSubs = mGeomPartOffsets[partIdx+1] - mGeomPartOffsets[partIdx];
    return numSubs;
}

void ArcSDEGeometryBuffer::LoadFromSdeGeometry(SE_SHAPE shape)
{
    LONG lResult = SE_SUCCESS;
    FdoPtr<FdoIGeometry> geom;
    FdoInt32 iOrdinateIndex = 0;

    // Get shape type, whether it has Z/M values, and number of points/parts/subparts:
    mNumParts = 0L;
    mNumSubParts = 0L;
    mShapeType = 0L;
    mNumPoints = 0L;
    lResult = SE_shape_get_type (shape, &mShapeType);
    handle_sde_err<FdoCommandException>(lResult, __FILE__, __LINE__, ARCSDE_GEOMETRY_CONVERSION_SHAPE_TO_FGF, "Error encountered while converting ArcSDE shape to FGF.");
    mbHasZ = (TRUE==SE_shape_is_3D(shape)) ? true : false;
    mbHasM = (TRUE==SE_shape_is_measured(shape)) ? true : false;
    lResult = SE_shape_get_num_parts(shape, &mNumParts, &mNumSubParts);
    handle_sde_err<FdoCommandException>(lResult, __FILE__, __LINE__, ARCSDE_GEOMETRY_CONVERSION_SHAPE_TO_FGF, "Error encountered while converting ArcSDE shape to FGF.");
    lResult = SE_shape_get_num_points (shape, 0, 0, &mNumPoints);
    handle_sde_err<FdoCommandException>(lResult, __FILE__, __LINE__, ARCSDE_GEOMETRY_CONVERSION_SHAPE_TO_FGF, "Error encountered while converting ArcSDE shape to FGF.");

    // Get shape's point/part/subpart data:
    mGeomPartOffsets.SetSize(mNumParts + 1);  // Add 1 extra for later convenience
    mGeomSubpartOffsets.SetSize(mNumSubParts + 1); // Add 1 extra for later convenience
    mGeomXY.SetSize(mNumPoints);

    if (mbHasZ)
    {
        mGeomPointsZ.SetSize(mNumPoints);
    }
    if (mbHasM)
    {
        mGeomPointsM.SetSize(mNumPoints);
    }

    lResult = SE_shape_get_all_points (shape, SE_DEFAULT_ROTATION, mGeomPartOffsets.GetAddr(), mGeomSubpartOffsets.GetAddr(), mGeomXY.GetAddr(), mbHasZ ? mGeomPointsZ.GetAddr():NULL, mbHasM ? mGeomPointsM.GetAddr():NULL);
    handle_sde_err<FdoCommandException>(lResult, __FILE__, __LINE__, ARCSDE_GEOMETRY_CONVERSION_SHAPE_TO_FGF, "Error encountered while converting ArcSDE shape to FGF.");
    // Add additional index for later convenience
    mGeomPartOffsets[mNumParts] = mNumSubParts;
    mGeomSubpartOffsets[mNumSubParts] = mNumPoints;
}


bool ArcSDEGeometryBuffer::FetchOrdinates(DynamicBuffer<double> &destOrdinates, int offset, int numPoints)
{
    FdoInt32 iNumOrdinates = numPoints * (2 + (mbHasZ ? 1 : 0) + (mbHasM ? 1 : 0));
    destOrdinates.SetSize(iNumOrdinates);
    int ordinateIndex = 0;
    for(int i=0; i<numPoints; i++)
    {
        destOrdinates[ordinateIndex] = mGeomXY[offset].x;         
        ordinateIndex++;
        destOrdinates[ordinateIndex] = mGeomXY[offset].y;         
        ordinateIndex++;
        if (mbHasZ) 
        { 
            destOrdinates[ordinateIndex] = mGeomPointsZ[offset];        
            ordinateIndex++; 
        }
        if (mbHasM) 
        { 
            destOrdinates[ordinateIndex] = mGeomPointsM[offset];
            ordinateIndex++; 
        }
        offset++;
    }
    return true;
}

bool ArcSDEGeometryBuffer::GetOrdinates(DynamicBuffer<double> &destOrdinates, int part, int subPart)
{
    long numPointsForSubPart;
    long offset;
    GetPointsSegment(part, subPart, numPointsForSubPart, offset);
    FetchOrdinates(destOrdinates, offset, numPointsForSubPart);
    return true;
}

bool ArcSDEGeometryBuffer::GetOrdinates(DynamicBuffer<double> &destOrdinates)
{
    FetchOrdinates(destOrdinates, 0, mGeomXY.GetCount());
    return true;
}

bool ArcSDEGeometryBuffer::GetCoorOrdinates(long &count, SE_POINT * &xy, LFLOAT *&z, LFLOAT * &m, int part, int subPart)
{
    return false;
}

FdoByteArray *ArcSDEGeometryBuffer::ToFGF(FdoFgfGeometryFactory *gf)
{
  FdoDimensionality dim = (FdoDimensionality)(FdoDimensionality_XY | (mbHasZ ? FdoDimensionality_Z : 0) | (mbHasM ? FdoDimensionality_M : 0));
  FdoPtr<FdoIGeometry> geom;
  switch (mShapeType)
  {
    /*
    case SG_NIL_SHAPE:
    {
    // how to deal with this?  FGF doesnt handle NIL geometry types.
    }
    break;
    */

  case SG_POINT_SHAPE:
    {
      GetOrdinates(mGeomOrdinates, 0,0);
      geom = gf->CreatePoint(dim, mGeomOrdinates.GetAddr());
    }
    break;

  case SG_MULTI_POINT_SHAPE:
    {
      GetOrdinates(mGeomOrdinates);
      geom = gf->CreateMultiPoint(dim, mGeomOrdinates.GetCount(), mGeomOrdinates.GetAddr());
    }
    break;

  case SG_LINE_SHAPE:
  case SG_SIMPLE_LINE_SHAPE:
    {
      GetOrdinates(mGeomOrdinates);
      geom = gf->CreateLineString(dim, mGeomOrdinates.GetCount(), mGeomOrdinates.GetAddr());
    }
    break;

  case SG_MULTI_LINE_SHAPE:
  case SG_MULTI_SIMPLE_LINE_SHAPE:
    {
      FdoPtr<FdoLineStringCollection> lineStrings = FdoLineStringCollection::Create();

      for (FdoInt32 i=0; i<mNumParts; i++)
      {
        GetOrdinates(mGeomOrdinates, i, 0);
        FdoPtr<FdoILineString> lineString = gf->CreateLineString(dim, mGeomOrdinates.GetCount(), mGeomOrdinates.GetAddr());
        lineStrings->Add(lineString);
      }          
      geom = gf->CreateMultiLineString(lineStrings);
    }
    break;

  case SG_AREA_SHAPE:
    {
      FdoPtr<FdoILinearRing> exteriorRing;
      FdoPtr<FdoLinearRingCollection> interiorRings = FdoLinearRingCollection::Create();

      for (FdoInt32 i=0; i<numSubParts(0); i++)
      {
        GetOrdinates(mGeomOrdinates, 0, i);
        FdoPtr<FdoILinearRing> linearRing = gf->CreateLinearRing(dim, mGeomOrdinates.GetCount(), mGeomOrdinates.GetAddr());
        if (i==0)  // subpart 1 is always the outer ring
          exteriorRing = linearRing;
        else  // subparts 2,3,4,... are always the inner rings
          interiorRings->Add(linearRing);
      }
      geom = gf->CreatePolygon(exteriorRing, interiorRings);
    }
    break;

  case SG_MULTI_AREA_SHAPE:
    {
      FdoPtr<FdoPolygonCollection> polygons = FdoPolygonCollection::Create();
      FdoPtr<FdoILinearRing> exteriorRing;

      for (FdoInt32 p=0; p<mNumParts; p++)
      {
        FdoPtr<FdoLinearRingCollection> interiorRings = FdoLinearRingCollection::Create();
        for (FdoInt32 i=0; i<numSubParts(p); i++)
        {
          GetOrdinates(mGeomOrdinates, p, i);
          FdoPtr<FdoILinearRing> linearRing = gf->CreateLinearRing(dim, mGeomOrdinates.GetCount(), mGeomOrdinates.GetAddr());
          if (i==0)  // subpart 1 is always the outer ring
            exteriorRing = linearRing;
          else  // subparts 2,3,4,... are always the inner rings
            interiorRings->Add(linearRing);
        }
        FdoPtr<FdoIPolygon> polygon = gf->CreatePolygon(exteriorRing, interiorRings);
        polygons->Add(polygon);
      }
      geom = gf->CreateMultiPolygon(polygons);
    }
    break;

  default:
    throw FdoException::Create(NlsMsgGet(ARCSDE_GEOMETRY_CONVERSION_SHAPE_TO_FGF, "Error encountered while converting ArcSDE shape to FGF."));
    break;
  }

  // Convert geom to FGF for caller:
  return gf->GetFgf(geom);
}

SE_SHAPE ArcSDEGeometryBuffer::FgfToShape(FdoFgfGeometryFactory* gf, FdoByteArray* fgf, SE_CONNECTION sdeConnection, SE_COORDREF coordref, bool bCropToExtents)
{
    LONG lResult = SE_SUCCESS;
    // Convert FGF to FdoIGeometry:
    FdoPtr<FdoIGeometry> geom = gf->CreateGeometryFromFgf(fgf);
    double z,m;
    FdoInt32 dim = geom->GetDimensionality();
    long totalNumPoints = 0;
    long numPoints = 0;
    SE_SHAPE temp_shape = NULL;
    SE_COORDREF temp_coordref = NULL;
    SE_ENVELOPE temp_coordref_envelope;
    SE_ENVELOPE given_coordref_envelope;
    SE_SHAPE *work_shape = NULL;
    SE_SHAPE result_shape;
    FdoException *exception = NULL;
    LFLOAT *zBuffer = NULL;
    LFLOAT *mBuffer = NULL;
    SE_POINT *pointBuffer = NULL;
    
    try
    {
        // Prepare the SE_SHAPE object we will work with:
        lResult = SE_shape_create(coordref, &result_shape);
        handle_sde_err<FdoCommandException>(sdeConnection, lResult, __FILE__, __LINE__, ARCSDE_CONVERT_SHAPE_FAILED, "Failed to convert FGF geometry to ArcSDE shape.");
        if (bCropToExtents)
        {
            // NOTE: ArcSDE won't let us create a shape that extends beyond its associated coordinate system's
            //    extents.  Even a "NULL" coordinate system is considered to have extents (starting at (0,0)
            //    which disallows negative ordinates).  Therefore, we need to create a temporary coordinate
            //    reference that is big enough to store the given shape before it is cropped to the geometry
            //    column's coordinate reference's extents.

            // Set the temporary coordref's extents to fit the given geometry:
            FdoPtr<FdoIEnvelope> fgfEnv = geom->GetEnvelope();
            temp_coordref_envelope.minx = fgfEnv->GetMinX();
            temp_coordref_envelope.maxx = fgfEnv->GetMaxX();
            temp_coordref_envelope.miny = fgfEnv->GetMinY();
            temp_coordref_envelope.maxy = fgfEnv->GetMaxY();
            
            // Enlarge the temporary coordref's extents to also fit the given coorref's extents:
            lResult = SE_coordref_get_xy_envelope(coordref, &given_coordref_envelope);
            handle_sde_err<FdoCommandException>(sdeConnection, lResult, __FILE__, __LINE__, ARCSDE_CONVERT_SHAPE_FAILED, "Failed to convert FGF geometry to ArcSDE shape.");
            if (given_coordref_envelope.minx < temp_coordref_envelope.minx)
                temp_coordref_envelope.minx = given_coordref_envelope.minx;
            if (given_coordref_envelope.maxx > temp_coordref_envelope.maxx)
                temp_coordref_envelope.maxx = given_coordref_envelope.maxx;
            if (given_coordref_envelope.miny < temp_coordref_envelope.miny)
                temp_coordref_envelope.miny = given_coordref_envelope.miny;
            if (given_coordref_envelope.maxy > temp_coordref_envelope.maxy)
                temp_coordref_envelope.maxy = given_coordref_envelope.maxy;

            // Create the temporary coordref
            lResult = SE_coordref_create(&temp_coordref);
            handle_sde_err<FdoCommandException>(sdeConnection, lResult, __FILE__, __LINE__, ARCSDE_CONVERT_SHAPE_FAILED, "Failed to convert FGF geometry to ArcSDE shape.");
            // Make it a duplicate of the column coordref (e.g. same coordinate system)
            lResult = SE_coordref_duplicate(coordref, temp_coordref);
            handle_sde_err<FdoCommandException>(sdeConnection, lResult, __FILE__, __LINE__, ARCSDE_CONVERT_SHAPE_FAILED, "Failed to convert FGF geometry to ArcSDE shape.");
            // Expand the extents of the temporary coordref
            lResult = SE_coordref_set_xy_by_envelope(temp_coordref, &temp_coordref_envelope);
            handle_sde_err<FdoCommandException>(sdeConnection, lResult, __FILE__, __LINE__, ARCSDE_CONVERT_SHAPE_FAILED, "Failed to convert FGF geometry to ArcSDE shape.");

            // Work on a temporary shape object:
            lResult = SE_shape_create(temp_coordref, &temp_shape);
            handle_sde_err<FdoCommandException>(sdeConnection, lResult, __FILE__, __LINE__, ARCSDE_CONVERT_SHAPE_FAILED, "Failed to convert FGF geometry to ArcSDE shape.");

            work_shape = &temp_shape;
        }
        else
        {
            // work directly on the output shape, since no cropping required:
            work_shape = &result_shape;
        }

        // Convert FdoIGeometry to SE_SHAPE:
        switch (geom->GetDerivedType())
        {
            /*
            case FdoGeometryType_None:
                lResult = SE_shape_make_nil(*work_shape);
                handle_sde_err<FdoCommandException>(m_Connection->GetConnection(), lResult, __FILE__, __LINE__, ARCSDE_CONVERT_SHAPE_FAILED, "Failed to convert FGF geometry to ArcSDE shape.");
            break;
            */

            case FdoGeometryType_Point:
            {
                FdoIPoint *point = static_cast<FdoIPoint*>(geom.p);
                SE_POINT sePoint;
                double   *pZ = NULL, *pM = NULL;
                point->GetPositionByMembers(&sePoint.x, &sePoint.y, &z, &m, &dim);
                if (dim & FdoDimensionality_Z)
                    pZ = &z;
                if (dim & FdoDimensionality_M)
                    pM = &m;
                lResult = SE_shape_generate_point (1L, &sePoint, pZ, pM, *work_shape);
                handle_sde_err<FdoCommandException>(sdeConnection, lResult, __FILE__, __LINE__, ARCSDE_CONVERT_SHAPE_FAILED, "Failed to convert FGF geometry to ArcSDE shape.");
            }
            break;

            case FdoGeometryType_MultiPoint:
            {
                FdoIMultiPoint *multiPoint = static_cast<FdoIMultiPoint*>(geom.p);
                totalNumPoints = multiPoint->GetCount();
                FdoPtr<FdoIPoint> point;

                InitBuffers(dim, totalNumPoints, &pointBuffer, &zBuffer, &mBuffer);

                // Populate arrays for XY, Z and M (as required):
                for (FdoInt32 i=0; i<totalNumPoints; i++)
                {
                    point = multiPoint->GetItem(i);
                    point->GetPositionByMembers(&(mGeomXY[i].x), &(mGeomXY[i].y), &z, &m, &dim);
                    if (dim & FdoDimensionality_Z)
                        mGeomPointsZ[i] = z;
                    if (dim & FdoDimensionality_M)
                        mGeomPointsM[i] = m;
                }

                // Populate the SE_SHAPE based on the XY,Z, and M arrays:
                lResult = SE_shape_generate_point (totalNumPoints, pointBuffer, zBuffer, mBuffer, *work_shape);
                handle_sde_err<FdoCommandException>(sdeConnection, lResult, __FILE__, __LINE__, ARCSDE_CONVERT_SHAPE_FAILED, "Failed to convert FGF geometry to ArcSDE shape.");
            }
            break;

            case FdoGeometryType_LineString:
            {
                FdoILineString *lineString = static_cast<FdoILineString*>(geom.p);
                totalNumPoints = lineString->GetCount();

                InitBuffers(dim, totalNumPoints, &pointBuffer, &zBuffer, &mBuffer);
                // Populate arrays for XY, Z and M (as required):
                for (FdoInt32 i=0; i<totalNumPoints; i++)
                {
                    lineString->GetItemByMembers(i, &(mGeomXY)[i].x, &(mGeomXY)[i].y, &z, &m, &dim);
                    if (dim & FdoDimensionality_Z)
                        mGeomPointsZ[i] = z;
                    if (dim & FdoDimensionality_M)
                        mGeomPointsM[i] = m;
                }
                // Populate the SE_SHAPE based on the XY,Z, and M arrays:
                lResult = SE_shape_generate_line (totalNumPoints, 1, NULL, pointBuffer, zBuffer, mBuffer, *work_shape);
                handle_sde_err<FdoCommandException>(sdeConnection, lResult, __FILE__, __LINE__, ARCSDE_CONVERT_SHAPE_FAILED, "Failed to convert FGF geometry to ArcSDE shape.");
            }
            break;

            case FdoGeometryType_MultiLineString:
            {
                FdoIMultiLineString *multiLineString = static_cast<FdoIMultiLineString*>(geom.p);
                FdoPtr<FdoILineString> lineString;
                FdoInt32 numLineStrings = multiLineString->GetCount();
                mGeomPartOffsets.SetSize(numLineStrings);

                // Figure out the total number of points and each linestring's offset in the offset array:
                for (FdoInt32 i=0; i<numLineStrings; i++)
                {
                    // Set this linestring's offset in the 'flat' point arrays:
                    mGeomPartOffsets[i] = totalNumPoints;

                    // Add this linestring's number of points to totalNumPoints:
                    lineString = multiLineString->GetItem(i);
                    totalNumPoints += lineString->GetCount();
                }

                // Allocate arrays for XY, Z, and M (as required):
                InitBuffers(dim, totalNumPoints, &pointBuffer, &zBuffer, &mBuffer);

                // Populate arrays for XY, Z and M (as required):
                long lPointIndex = 0;
                for (FdoInt32 i=0; i<numLineStrings; i++)
                {
                    lineString = multiLineString->GetItem(i);
                    numPoints = lineString->GetCount();
                    for (FdoInt32 j=0; j<numPoints; j++)
                    {
                        lineString->GetItemByMembers(j, &(mGeomXY)[lPointIndex].x, &(mGeomXY)[lPointIndex].y, &z, &m, &dim);
                        if (dim & FdoDimensionality_Z)
                            mGeomPointsZ[lPointIndex] = z;
                        if (dim & FdoDimensionality_M)
                            mGeomPointsM[lPointIndex] = m;

                        lPointIndex++;
                    }
                }

                // Populate the SE_SHAPE based on the XY,Z, and M arrays:
                lResult = SE_shape_generate_line (totalNumPoints, numLineStrings, mGeomPartOffsets.GetAddr(), pointBuffer, zBuffer, mBuffer, *work_shape);
                handle_sde_err<FdoCommandException>(sdeConnection, lResult, __FILE__, __LINE__, ARCSDE_CONVERT_SHAPE_FAILED, "Failed to convert FGF geometry to ArcSDE shape.");
            }
            break;

            case FdoGeometryType_Polygon:
            {
                FdoIPolygon *polygon = static_cast<FdoIPolygon*>(geom.p);
                FdoPtr<FdoILinearRing> linearRing;

                // Figure out the total number of points:
                linearRing = polygon->GetExteriorRing();
                totalNumPoints = linearRing->GetCount();
                FdoInt32 numInteriorRings = polygon->GetInteriorRingCount();
                for (FdoInt32 i=0; i<numInteriorRings; i++)
                {
                    linearRing = polygon->GetInteriorRing(i);
                    totalNumPoints += linearRing->GetCount();
                }

                InitBuffers(dim, totalNumPoints, &pointBuffer, &zBuffer, &mBuffer);

                // Populate arrays for XY, Z and M (as required):
                long lPointIndex = 0;
                // add exterior ring's points:
                linearRing = polygon->GetExteriorRing();
                numPoints = linearRing->GetCount();
                for (FdoInt32 i=0; i<numPoints; i++)
                {
                    linearRing->GetItemByMembers(i, &(mGeomXY)[lPointIndex].x, &(mGeomXY)[lPointIndex].y, &z, &m, &dim);
                    if (dim & FdoDimensionality_Z)
                        mGeomPointsZ[lPointIndex] = z;
                    if (dim & FdoDimensionality_M)
                        mGeomPointsM[lPointIndex] = m;

                    lPointIndex++;
                }
                // add interior rings' points:
                for (FdoInt32 i=0; i<numInteriorRings; i++)
                {
                    linearRing = polygon->GetInteriorRing(i);
                    numPoints = linearRing->GetCount();
                    for (FdoInt32 j=0; j<numPoints; j++)
                    {
                        linearRing->GetItemByMembers(j, &(mGeomXY)[lPointIndex].x, &(mGeomXY)[lPointIndex].y, &z, &m, &dim);
                        if (dim & FdoDimensionality_Z)
                            mGeomPointsZ[lPointIndex] = z;
                        if (dim & FdoDimensionality_M)
                            mGeomPointsM[lPointIndex] = m;

                        lPointIndex++;
                    }
                }

                // Populate the SE_SHAPE based on the XY,Z, and M arrays:
                lResult = SE_shape_generate_polygon (totalNumPoints, 1, NULL, pointBuffer, zBuffer, mBuffer, *work_shape);
                handle_sde_err<FdoCommandException>(sdeConnection, lResult, __FILE__, __LINE__, ARCSDE_CONVERT_SHAPE_FAILED, "Failed to convert FGF geometry to ArcSDE shape.");
            }
            break;

            case FdoGeometryType_MultiPolygon:
            {
                FdoIMultiPolygon *multiPolygon = static_cast<FdoIMultiPolygon*>(geom.p);
                FdoPtr<FdoILinearRing> linearRing;
                FdoPtr<FdoIPolygon> polygon;

                // Figure out the total number of points, and the part offsets:
                FdoInt32 numPolygons = multiPolygon->GetCount();
                mGeomPartOffsets.SetSize(numPolygons);
                for (FdoInt32 i=0; i<numPolygons;i ++)
                {
                    // Store this polygon's offset in the 'flat' point arrays:
                    mGeomPartOffsets[i] = totalNumPoints;

                    // Add this polygon's number of points to totalNumPoints:
                    polygon = multiPolygon->GetItem(i);
                    linearRing = polygon->GetExteriorRing();
                    totalNumPoints += linearRing->GetCount();
                    FdoInt32 numInteriorRings = polygon->GetInteriorRingCount();
                    for (FdoInt32 j=0; j<numInteriorRings; j++)
                    {
                        linearRing = polygon->GetInteriorRing(j);
                        totalNumPoints += linearRing->GetCount();
                    }
                }

                InitBuffers(dim, totalNumPoints, &pointBuffer, &zBuffer, &mBuffer);

                // Populate arrays for XY, Z and M (as required):
                long lPointIndex = 0;
                for (FdoInt32 i=0; i<numPolygons;i ++)
                {
                    polygon = multiPolygon->GetItem(i);

                    // add exterior ring's points:
                    linearRing = polygon->GetExteriorRing();
                    numPoints = linearRing->GetCount();
                    for (FdoInt32 i=0; i<numPoints; i++)
                    {
                        linearRing->GetItemByMembers(i, &(mGeomXY)[lPointIndex].x, &(mGeomXY)[lPointIndex].y, &z, &m, &dim);
                        if (dim & FdoDimensionality_Z)
                            mGeomPointsZ[lPointIndex] = z;
                        if (dim & FdoDimensionality_M)
                            mGeomPointsM[lPointIndex] = m;

                        lPointIndex++;
                    }
                    // add interior rings' points:
                    FdoInt32 numInteriorRings = polygon->GetInteriorRingCount();
                    for (FdoInt32 j=0; j<numInteriorRings; j++)
                    {
                        linearRing = polygon->GetInteriorRing(j);
                        numPoints = linearRing->GetCount();
                        for (FdoInt32 k=0; k<numPoints; k++)
                        {
                            linearRing->GetItemByMembers(k, &(mGeomXY)[lPointIndex].x, &(mGeomXY)[lPointIndex].y, &z, &m, &dim);
                            if (dim & FdoDimensionality_Z)
                                mGeomPointsZ[lPointIndex] = z;
                            if (dim & FdoDimensionality_M)
                                mGeomPointsM[lPointIndex] = m;

                            lPointIndex++;
                        }
                    }
                }

                // Populate the SE_SHAPE based on the XY,Z, and M arrays:
                lResult = SE_shape_generate_polygon (totalNumPoints, numPolygons, mGeomPartOffsets.GetAddr(), pointBuffer, zBuffer, mBuffer, *work_shape);
                handle_sde_err<FdoCommandException>(sdeConnection, lResult, __FILE__, __LINE__, ARCSDE_CONVERT_SHAPE_FAILED, "Failed to convert FGF geometry to ArcSDE shape.");
            }
            break;

            case FdoGeometryType_MultiGeometry:
            case FdoGeometryType_CurveString:
            case FdoGeometryType_CurvePolygon:
            case FdoGeometryType_MultiCurveString:
            case FdoGeometryType_MultiCurvePolygon:
            default:
                throw FdoException::Create(NlsMsgGet1(ARCSDE_UNSUPPORTED_GEOMETRY_TYPE, "The geometry type '%1$d' is not supported by ArcSDE.", geom->GetDerivedType()));
            break;
        }


        // Crop resulting shape to extents, if requested:
        if (bCropToExtents)
        {
            lResult = SE_shape_clip(temp_shape, &given_coordref_envelope, result_shape);
            handle_sde_err<FdoCommandException>(sdeConnection, lResult, __FILE__, __LINE__, ARCSDE_SHAPE_CLIP_FAILED, "Failed to clip the given shape to the coordinate reference's extents.");
        }
    }
    catch (FdoException *e)
    {
        exception = e;  // re-throw this later, after cleaning up
    }

    // clean up:
    if (temp_shape != NULL)
        SE_shape_free(temp_shape);
    if (temp_coordref != NULL)
        SE_coordref_free(temp_coordref);

    // Re-throw exception, if any:
    if (exception)
        throw exception;
    return result_shape;
}







