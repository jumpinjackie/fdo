// 
//  
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  

#include "stdafx.h"
#include <FdoCommonGeometryUtil.h>


// ----------------------------------------------------------------------------
//                               Public Functions
// ----------------------------------------------------------------------------

FdoInt32 FdoCommonGeometryUtil::GetAllGeometryTypesCode ()

// +---------------------------------------------------------------------------
// | The function returns the hex-code that represents the selection of all
// | geometry types.
// +---------------------------------------------------------------------------

{

    return ALL_GEOMETRY_POINT_CURVE_SURFACE;

}  //  GetAllGeometryTypesCode ()

FdoInt32 FdoCommonGeometryUtil::GetCurveGeometryTypesCode ()

// +---------------------------------------------------------------------------
// | The function returns the hex-code that represents the selection of all
// | curve/line based geometry types.
// +---------------------------------------------------------------------------

{

    return ALL_GEOMETRY_CURVE;

}  //  GetCurveGeometryTypesCode ()

FdoInt32 FdoCommonGeometryUtil::GetCurveSurfaceGeometryTypesCode ()

// +---------------------------------------------------------------------------
// | The function returns the hex-code that represents the selection of all
// | curve/line and polygon based geometry types.
// +---------------------------------------------------------------------------

{

    return ALL_GEOMETRY_CURVE_SURFACE;

}  //  GetCurveSurfaceGeometryTypesCode ()

FdoInt32 FdoCommonGeometryUtil::GetPointGeometryTypesCode ()

// +---------------------------------------------------------------------------
// | The function returns the hex-code that represents the selection of all
// | point based geometry types.
// +---------------------------------------------------------------------------

{

    return All_GEOMETRY_POINT;

}  //  GetPointGeometryTypesCode ()

FdoInt32 FdoCommonGeometryUtil::GetPointCurveGeometryTypesCode ()

// +---------------------------------------------------------------------------
// | The function returns the hex-code that represents the selection of all
// | point and curve/line based geometry types.
// +---------------------------------------------------------------------------

{

    return ALL_GEOMETRY_POINT_CURVE;

}  //  GetPointCurveGeometryTypesCode ()

FdoInt32 FdoCommonGeometryUtil::GetPointSurfaceGeometryTypesCode ()

// +---------------------------------------------------------------------------
// | The function returns the hex-code that represents the selection of all
// | point and polygon based geometry types.
// +---------------------------------------------------------------------------

{

    return ALL_GEOMETRY_POINT_SURFACE;

}  //  GetPointSurfaceGeometryTypesCode ()

FdoInt32 FdoCommonGeometryUtil::GetSurfaceGeometryTypesCode ()

// +---------------------------------------------------------------------------
// | The function returns the hex-code that represents the selection of all
// | polygon based geometry types.
// +---------------------------------------------------------------------------

{

    return ALL_GEOMETRY_SURFACE;

}  //  GetSurfaceGeometryTypesCode ()

FdoInt32 FdoCommonGeometryUtil::MapGeometryTypeToHexCode (
                                                        FdoGeometryType gType)

// +---------------------------------------------------------------------------
// | The functions map a geometry type to its unique hex-code that can be used
// | in a bit-wise operation.
// +---------------------------------------------------------------------------

{

    switch (gType) {

      case FdoGeometryType_None:
        return FdoCommonGeometryTypeHexCode_None;
        break;

      case FdoGeometryType_Point:
        return FdoCommonGeometryTypeHexCode_Point;
        break;

      case FdoGeometryType_LineString:
        return FdoCommonGeometryTypeHexCode_LineString;
        break;

      case FdoGeometryType_Polygon:
        return FdoCommonGeometryTypeHexCode_Polygon;
        break;

      case FdoGeometryType_MultiPoint:
        return FdoCommonGeometryTypeHexCode_MultiPoint;
        break;

      case FdoGeometryType_MultiLineString:
        return FdoCommonGeometryTypeHexCode_MultiLineString;
        break;

      case FdoGeometryType_MultiPolygon:
        return FdoCommonGeometryTypeHexCode_MultiPolygon;
        break;

      case FdoGeometryType_MultiGeometry:
        return FdoCommonGeometryTypeHexCode_MultiGeometry;
        break;

      case FdoGeometryType_CurveString:
        return FdoCommonGeometryTypeHexCode_CurveString;
        break;

      case FdoGeometryType_CurvePolygon:
        return FdoCommonGeometryTypeHexCode_CurvePolygon;
        break;

      case FdoGeometryType_MultiCurveString:
        return FdoCommonGeometryTypeHexCode_MultiCurveString;
        break;

      case FdoGeometryType_MultiCurvePolygon:
        return FdoCommonGeometryTypeHexCode_MultiCurvePolygon;
        break;

      default:
        throw FdoException::Create(
                              FdoException::NLSGetMessage(
                                  FDO_NLSID(FDO_128_GEOMETRY_MAPPING_ERROR)));
        break;

    }  //  switch ...

}  //  MapGeometryTypeToHexCode ()

FdoInt32 FdoCommonGeometryUtil::MapGeometryTypeToHexCode (int pos)

// +---------------------------------------------------------------------------
// | The functions map a geometry type to its unique hex-code that can be used
// | in a bit-wise operation.
// +---------------------------------------------------------------------------

{

    switch (pos) {

      case  0:
        return FdoCommonGeometryTypeHexCode_None;
        break;

      case  1:
        return FdoCommonGeometryTypeHexCode_Point;
        break;

      case  2:
        return FdoCommonGeometryTypeHexCode_LineString;
        break;

      case  3:
        return FdoCommonGeometryTypeHexCode_Polygon;
        break;

      case  4:
        return FdoCommonGeometryTypeHexCode_MultiPoint;
        break;

      case  5:
        return FdoCommonGeometryTypeHexCode_MultiLineString;
        break;

      case  6:
        return FdoCommonGeometryTypeHexCode_MultiPolygon;
        break;

      case  7:
        return FdoCommonGeometryTypeHexCode_MultiGeometry;
        break;

      case  8:
        return FdoCommonGeometryTypeHexCode_CurveString;
        break;

      case  9:
        return FdoCommonGeometryTypeHexCode_CurvePolygon;
        break;

      case 10:
        return FdoCommonGeometryTypeHexCode_MultiCurveString;
        break;

      case 11:
        return FdoCommonGeometryTypeHexCode_MultiCurvePolygon;
        break;

      default:
        throw FdoException::Create(
                              FdoException::NLSGetMessage(
                                  FDO_NLSID(FDO_128_GEOMETRY_MAPPING_ERROR)));
        break;

    }  //  switch ...

}  //  MapGeometryTypeToHexCode ()

FdoGeometryType FdoCommonGeometryUtil::MapHexCodeToGeometryType (
                                                        FdoInt32 gTypeHexCode)

// +---------------------------------------------------------------------------
// | The function maps a geometry type hex-code to the corresponding FDO
// | geometry type.
// +---------------------------------------------------------------------------

{

    switch (gTypeHexCode) {

      case FdoCommonGeometryTypeHexCode_None:
        return FdoGeometryType_None;
        break;

      case FdoCommonGeometryTypeHexCode_Point:
        return FdoGeometryType_Point;
        break;

      case FdoCommonGeometryTypeHexCode_LineString:
        return FdoGeometryType_LineString;
        break;

      case FdoCommonGeometryTypeHexCode_Polygon:
        return FdoGeometryType_Polygon;
        break;

      case FdoCommonGeometryTypeHexCode_MultiPoint:
        return FdoGeometryType_MultiPoint;
        break;

      case FdoCommonGeometryTypeHexCode_MultiLineString:
        return FdoGeometryType_MultiLineString;
        break;

      case FdoCommonGeometryTypeHexCode_MultiPolygon:
        return FdoGeometryType_MultiPolygon;
        break;

      case FdoCommonGeometryTypeHexCode_MultiGeometry:
        return FdoGeometryType_MultiGeometry;
        break;

      case FdoCommonGeometryTypeHexCode_CurveString:
        return FdoGeometryType_CurveString;
        break;

      case FdoCommonGeometryTypeHexCode_CurvePolygon:
        return FdoGeometryType_CurvePolygon;
        break;

      case FdoCommonGeometryTypeHexCode_MultiCurveString:
        return FdoGeometryType_MultiCurveString;
        break;

      case FdoCommonGeometryTypeHexCode_MultiCurvePolygon:
        return FdoGeometryType_MultiCurvePolygon;
        break;

      default:
        throw FdoException::Create(
                              FdoException::NLSGetMessage(
                                   FDO_NLSID(FDO_128_GEOMETRY_MAPPING_ERROR)));
        break;

    }  //  switch ...

}  //  MapHexCodeToGeometryType ()

FdoInt32 FdoCommonGeometryUtil::GetGeometryTypes(FdoInt32 value)
{
    FdoInt32 currGeometricType;

    // The following sets the corresponding geometry types.

    FdoInt32 geometryTypes = 0x00000;

    for (int i = 0; i < MAX_GEOMETRIC_TYPE_SIZE; i++)
    {
        currGeometricType = GetGeometricType(i);
        if (currGeometricType != -1)
        {
            if ((value & currGeometricType) > 0)
            {
                switch (currGeometricType)
                {
                    case FdoGeometricType_Point:
                        geometryTypes = geometryTypes | MapGeometryTypeToHexCode(FdoGeometryType_Point);
                        geometryTypes = geometryTypes | MapGeometryTypeToHexCode(FdoGeometryType_MultiPoint);
                        break;
                    case FdoGeometricType_Curve:
                        geometryTypes = geometryTypes | MapGeometryTypeToHexCode(FdoGeometryType_LineString);
                        geometryTypes = geometryTypes | MapGeometryTypeToHexCode(FdoGeometryType_MultiLineString);
                        geometryTypes = geometryTypes | MapGeometryTypeToHexCode(FdoGeometryType_CurveString);
                        geometryTypes = geometryTypes | MapGeometryTypeToHexCode(FdoGeometryType_MultiCurveString);
                        break;
                    case FdoGeometricType_Surface:
                        geometryTypes = geometryTypes | MapGeometryTypeToHexCode(FdoGeometryType_Polygon);
                        geometryTypes = geometryTypes | MapGeometryTypeToHexCode(FdoGeometryType_MultiPolygon);
                        geometryTypes = geometryTypes | MapGeometryTypeToHexCode(FdoGeometryType_CurvePolygon);
                        geometryTypes = geometryTypes | MapGeometryTypeToHexCode(FdoGeometryType_MultiCurvePolygon);
                        break;
                }
            }
        }
    }

	return geometryTypes;
}

void FdoCommonGeometryUtil::GeometryTypesToArray( FdoInt32 hexType, FdoGeometryType* gTypes, FdoInt32& typeCount )
{
    FdoInt32 currTypeHexCode;

    // Based on the current geometry type value fill the corresponding vector with the
    // selected geometry types.
    typeCount = 0;
    for (int i = 0; i < MAX_GEOMETRY_TYPE_SIZE; i++)
    {
        currTypeHexCode = MapGeometryTypeToHexCode(i);
        if ((hexType & currTypeHexCode) > 0)
            gTypes[typeCount++] = MapHexCodeToGeometryType(currTypeHexCode);
    }
}

FdoInt32 FdoCommonGeometryUtil::GetGeometricType(int pos)
{
    switch (pos) 
    {
        case 0:  return FdoGeometricType_Point;   break;
        case 1:  return FdoGeometricType_Curve;   break;
        case 2:  return FdoGeometricType_Surface; break;
        case 3:  return FdoGeometricType_Solid;   break;
        default: return -1;                       break;
    }
}

FdoInt32 FdoCommonGeometryUtil::GetNoneGeometryTypesCode()
{
	return FdoCommonGeometryTypeHexCode_None;
}

FdoInt32 FdoCommonGeometryUtil::GetCountGeometryTypesFromHex (FdoInt32 hexType)
{
    FdoInt32 typeCount = 0;
    for (int i = 0; i < MAX_GEOMETRY_TYPE_SIZE; i++)
    {
        if ((hexType & MapGeometryTypeToHexCode(i)) != 0)
            typeCount++;
    }
    return typeCount;
}

FdoIGeometry* FdoCommonGeometryUtil::ModifyRingOrientation(FdoIGeometry *geometry)
{
    FdoIGeometry*          newGeometry = FDO_SAFE_ADDREF(geometry);
	FdoGeometryType        geometryType = geometry->GetDerivedType();

    switch (geometryType)
    {
    case FdoGeometryType_Polygon:
        {
            FdoIPolygon * derivedGeom = static_cast<FdoIPolygon *>(geometry);
            if ( !IsPolygonCompatible(derivedGeom) )
            {
                FDO_SAFE_RELEASE(newGeometry);
                newGeometry = ModifyPolygonRingOrientation(derivedGeom);
            }
        }
        break;

    case FdoGeometryType_MultiPolygon:
        {
            FdoIMultiPolygon * derivedGeom = static_cast<FdoIMultiPolygon *>(geometry);
            FdoInt32 numSubGeometries = derivedGeom->GetCount();

            bool bPolygonCCW = true;
            for (FdoInt32 i = 0;  i < numSubGeometries;  i++)
            {
                FdoPtr<FdoIPolygon> subGeom = derivedGeom->GetItem(i);
                bPolygonCCW = IsPolygonCompatible(subGeom);
                if (bPolygonCCW == false)
                    break;
            }
            if (bPolygonCCW == false)
            {
                FdoPtr<FdoPolygonCollection> newSubGeometries = FdoPolygonCollection::Create();
                FdoInt32 numSubGeometries = derivedGeom->GetCount();
                for (FdoInt32 i = 0; i<numSubGeometries; i++)
                {
                    FdoPtr<FdoIPolygon> subGeom = derivedGeom->GetItem(i);
                    FdoPtr<FdoIGeometry> newSubGeometry = ModifyPolygonRingOrientation(subGeom);
                    newSubGeometries->Add(static_cast<FdoIPolygon *>(newSubGeometry.p));
                }
    			FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
                FDO_SAFE_RELEASE(newGeometry);
                newGeometry = gf->CreateMultiPolygon(newSubGeometries);
            }
        }
        break;
    }

    return newGeometry;
}


// modify the polygon so that the exteral ring is ccw and all the interior rings are cw
FdoIGeometry* FdoCommonGeometryUtil::ModifyPolygonRingOrientation(FdoIPolygon *polygon)
{
    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
    FdoIGeometry *geometry;
    FdoPtr<FdoILinearRing> originalExternalRing = polygon->GetExteriorRing();
    FdoPtr<FdoILinearRing> newExteriorRing;
    double *newOrdinates = NULL;
    FdoInt32 dim = originalExternalRing->GetDimensionality();
    FdoInt32 numberCoordinates = originalExternalRing->GetCount();
    FdoInt32 numOrdinates = DimensionalityToNumOrdinates(dim) * numberCoordinates;
    const double *ordinates = originalExternalRing->GetOrdinates();
    bool clockwise = OrdinatesAreClockwise(dim, numOrdinates, ordinates);

    if (clockwise)
    {
        newOrdinates = new double[numOrdinates];
        ReverseOrdinates(dim, numOrdinates,ordinates,newOrdinates);
        newExteriorRing = gf->CreateLinearRing(dim, numOrdinates, newOrdinates);
        delete [] newOrdinates;
    }
    else
    {
        newExteriorRing = originalExternalRing;
    }

    FdoPtr<FdoLinearRingCollection> newInteriorRings = FdoLinearRingCollection::Create();
    for (int j = 0; j<polygon->GetInteriorRingCount();  j++)
    {
        FdoPtr<FdoILinearRing> ring = polygon->GetInteriorRing(j);
        ordinates = ring->GetOrdinates();
        dim = ring->GetDimensionality();
        numberCoordinates = ring->GetCount();
        numOrdinates = DimensionalityToNumOrdinates(dim) * numberCoordinates;
        clockwise = OrdinatesAreClockwise(dim, numOrdinates, ordinates);
        if (clockwise == false)
        {
            newOrdinates = new double[numOrdinates];
            ReverseOrdinates(dim,numOrdinates,ordinates,newOrdinates);
            FdoPtr<FdoILinearRing> newInteriorRing = gf->CreateLinearRing(dim, numOrdinates, newOrdinates);
            newInteriorRings->Add(newInteriorRing);
            delete [] newOrdinates;
        }
        else
        {
            newInteriorRings->Add(ring);
        }
    }
    geometry = gf->CreatePolygon(newExteriorRing, newInteriorRings.p);  
    return geometry;
}

// returns true the external ring is ccw and all the interior rings are cw
bool FdoCommonGeometryUtil::IsPolygonCompatible(FdoIPolygon *polygon)
{
    bool isPolygonCompatible = false;
    FdoPtr<FdoILinearRing> originalExternalRing = polygon->GetExteriorRing();
    FdoInt32 dim = originalExternalRing->GetDimensionality();
    FdoInt32 numberCoordinates = originalExternalRing->GetCount();
    FdoInt32 numOrdinates = DimensionalityToNumOrdinates(dim) * numberCoordinates;
    const double *ordinates = originalExternalRing->GetOrdinates();
    bool clockwise = OrdinatesAreClockwise(dim, numOrdinates, ordinates);
    if (clockwise == false)
    {
        isPolygonCompatible = true;
        for (int j = 0;  j < polygon->GetInteriorRingCount();  j++)
        {
            FdoPtr<FdoILinearRing> ring = polygon->GetInteriorRing(j);
            ordinates = ring->GetOrdinates();
            dim = ring->GetDimensionality();
            numberCoordinates = ring->GetCount();
            numOrdinates = DimensionalityToNumOrdinates(dim) * numberCoordinates;
            clockwise = OrdinatesAreClockwise(dim, numOrdinates, ordinates);
            if (clockwise == false)
            {
                isPolygonCompatible = false;
                break;
            }
        }
    }
    return isPolygonCompatible;
}

///////////////////////////////////////////////////////////////////////////////////////
// FDO RING ORIENTATION HELPERS
//
// The following several routines are for analyzing and reversing polygon loops.
// They are constructed in such a way as to allow migration to fdo spatial utility.
//

// detemines the number of ordinates in a particular dimensionality coordinate
inline FdoInt32 FdoCommonGeometryUtil::DimensionalityToNumOrdinates(FdoInt32 dimensionality)
{
    FdoInt32 value = 0;

    if ( dimensionality == FdoDimensionality_XY )
        value = 2;
    else if ( dimensionality == ( FdoDimensionality_XY | FdoDimensionality_Z ) )
        value = 3;
    else if ( dimensionality == ( FdoDimensionality_XY | FdoDimensionality_M ) )
        value = 3;
    else if ( dimensionality == ( FdoDimensionality_XY | FdoDimensionality_Z | FdoDimensionality_M ) )
        value = 4;

    return value;
}

// Determines if orientation is clockwise or counterclockwise
// based on whether x2y2 is to the left of the line defined by x0y0,x1y1
// if x2y2 is to the left, the orientation is counterclockwise
// if x2y2 is on the line, the orientation is the degenerate case
// if x2y2 is to the right, the orientation is clockwise
inline bool FdoCommonGeometryUtil::Clockwise(double x0, double y0, double x1, double y1, double x2, double y2)
{
    return ((x1 - x0) * (y2 - y0) - (x2 - x0) * (y1 - y0)) < 0;
}

// Determines if the passed array of ordinates is clockwise
bool FdoCommonGeometryUtil::OrdinatesAreClockwise(FdoInt32 dimensionality, FdoInt32 numOrdinates, const double* ordinates)
{
    // we will determine the bottom right corner vertex
    // and check to see if it is to the left or right
    // of the next corresponding segment.
    FdoInt32 dimLen = DimensionalityToNumOrdinates(dimensionality);
    if( 0 == dimLen )
        return false;
    FdoInt32 minOffset = 0;
    double minX = ordinates[0];
    double minY = ordinates[1];
    // leave out the final closing vertex, since it shouldn't be used for
    // the corner check. It will be confused with the first vertex in the 
    // first vertex special case check (0 == minOffset) below.
    FdoInt32 maxOrdinates = numOrdinates-dimLen;
    for( FdoInt32 i = dimLen; i < maxOrdinates; i += dimLen )
    {
        if( ordinates[i+1] > minY )
            continue;
        if( ordinates[i+1] == minY )
            if( ordinates[i] < minX )
                continue;
        // if we get here, we found a better min vertex
        minOffset = i;
        minX = ordinates[i];
        minY = ordinates[1 + i];
    }

    if( 0 == minOffset )
        return Clockwise(
            ordinates[maxOrdinates-dimLen], ordinates[1 + maxOrdinates-dimLen],
            ordinates[0],                   ordinates[1],
            ordinates[dimLen],              ordinates[1 + dimLen] );
    else
        return Clockwise(
            ordinates[minOffset-dimLen], ordinates[1 + minOffset-dimLen],
            ordinates[minOffset],        ordinates[1 + minOffset],
            ordinates[minOffset+dimLen], ordinates[1 + minOffset+dimLen] );
}

// Reverses the individual coordinates in the passed ordinate array
void FdoCommonGeometryUtil::ReverseOrdinates(FdoInt32 dimensionality, FdoInt32 numOrdinates, const double* ordinates, double *out_ordinates)
{
    FdoInt32 dimLen = DimensionalityToNumOrdinates(dimensionality);
    if( 0 == dimLen )
        return;

    for(int i=0,j = numOrdinates-dimLen; i<numOrdinates; i+=dimLen, j-=dimLen)
    {
        for( FdoInt32 n = 0; n < dimLen; n++ )
        {
            out_ordinates[j+n] = ordinates[i+n];
        }
    }
}

