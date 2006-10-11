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


#include "Util.h"
#include "GeometryUtility.h"


/************************************************************************/
/* Helper: WriteGeometry
/************************************************************************/
void FgfUtil::WriteGeometry(FdoIGeometry* geometry, FdoByteArray ** outputStream)
{
    FdoInt32 dimensionality = 0;
    FdoInt32 numPositions = 0;
    FdoInt32 numOrdsPerPos = 0;
    FdoInt32 numRings = 0;
    FdoInt32 numSubGeometries = 0;
    FdoInt32 numCurveSegments = 0;
    FdoPtr<FdoIRing> ring;
    FdoPtr<FdoILinearRing> lRing;
    FdoPtr<FdoIDirectPosition> pos;
    FdoILineString * ls;
    FdoIPoint * pt;
    FdoIPolygon * poly;
    FdoIMultiPoint * mpt;
    FdoIMultiLineString * mls;
    FdoIMultiPolygon * mpoly;
    FdoICurveString * cs;
    FdoIMultiCurveString * mcs;
    FdoICurvePolygon * cpoly;
    FdoIMultiCurvePolygon * mcpoly;
    FdoIMultiGeometry * mgeom;
    FdoInt32 i;

    FdoGeometryType geometryType = geometry->GetDerivedType();
    FGFUTIL_WRITE_INT32(outputStream, geometryType);

    switch (geometryType)
    {
	case FdoGeometryType_LineString:
        ls = static_cast<FdoILineString *>(geometry);
	    dimensionality = ls->GetDimensionality();
	    numPositions = ls->GetCount();
	    FGFUTIL_WRITE_INT32(outputStream, dimensionality);
	    FGFUTIL_WRITE_INT32(outputStream, numPositions);
	    for (i=0;  i < numPositions;  i++)
	    {
		    FdoPtr<FdoIDirectPosition> pos = ls->GetItem(i);
		    WriteDirectPosition(outputStream, pos);
	    }
		break;

	case FdoGeometryType_Point:
        pt = static_cast<FdoIPoint *>(geometry);
        dimensionality = pt->GetDimensionality();
	    FGFUTIL_WRITE_INT32(outputStream, dimensionality);
	    pos = pt->GetPosition();
	    WriteDirectPosition(outputStream, pos);
		break;

	case FdoGeometryType_Polygon:
        poly = static_cast<FdoIPolygon *>(geometry);
	    dimensionality = poly->GetDimensionality();
	    FGFUTIL_WRITE_INT32(outputStream, dimensionality);
	    numRings = poly->GetInteriorRingCount();
	    FGFUTIL_WRITE_INT32(outputStream, numRings+1);			// Include exterior ring in count
	    lRing = poly->GetExteriorRing();
	    FgfUtil::WriteLinearRing(lRing, outputStream);
	    for (i=0; i<numRings; i++)
	    {
		    lRing = poly->GetInteriorRing(i);
		    FgfUtil::WriteLinearRing(lRing, outputStream);
	    }
		break;

	case FdoGeometryType_MultiPoint:
        mpt = static_cast<FdoIMultiPoint *>(geometry);
	    numSubGeometries = mpt->GetCount();
	    FGFUTIL_WRITE_INT32(outputStream, numSubGeometries);
	    for (i=0; i<numSubGeometries; i++)
	    {
		    FdoPtr<FdoIPoint> pnt = mpt->GetItem(i);
            WriteGeometry(pnt, outputStream);
	    }
		break;

	case FdoGeometryType_MultiLineString:
        mls = static_cast<FdoIMultiLineString *>(geometry);
	    numSubGeometries = mls->GetCount();
	    FGFUTIL_WRITE_INT32(outputStream, numSubGeometries);
	    for (i=0; i<numSubGeometries; i++)
        {
            FdoPtr<FdoILineString> lineString = mls->GetItem(i);
            WriteGeometry(lineString, outputStream);
        }
		break;

	case FdoGeometryType_MultiPolygon:
        mpoly = static_cast<FdoIMultiPolygon *>(geometry);
	    numSubGeometries = mpoly->GetCount(); 
	    FGFUTIL_WRITE_INT32(outputStream, numSubGeometries);
	    for (i=0; i<numSubGeometries; i++)
        {
            FdoPtr<FdoIPolygon> polygon = mpoly->GetItem(i);
            WriteGeometry(polygon, outputStream);
        }
		break;

	case FdoGeometryType_CurveString:
        cs = static_cast<FdoICurveString *>(geometry);
	    dimensionality = cs->GetDimensionality();
	    FGFUTIL_WRITE_INT32(outputStream, dimensionality);
	    pos = cs->GetStartPosition();
	    WriteDirectPosition(outputStream, pos);
	    numCurveSegments = cs->GetCount();
	    FGFUTIL_WRITE_INT32(outputStream, numCurveSegments);
	    for (i=0; i<numCurveSegments; i++)
	    {
		    FdoPtr<FdoICurveSegmentAbstract> curveSeg = cs->GetItem(i);
		    WriteCurveSegment(curveSeg, outputStream);
	    }
		break;

	case FdoGeometryType_MultiCurveString:
        mcs = static_cast<FdoIMultiCurveString *>(geometry);
	    numSubGeometries = mcs->GetCount();
	    FGFUTIL_WRITE_INT32(outputStream, numSubGeometries);
	    for (i=0; i<numSubGeometries; i++)
	    {
		    FdoPtr<FdoICurveString> curveString = mcs->GetItem(i);
            WriteGeometry(curveString, outputStream);
	    }
		break;

	case FdoGeometryType_CurvePolygon:
        cpoly = static_cast<FdoICurvePolygon *>(geometry);
	    dimensionality = cpoly->GetDimensionality();
	    FGFUTIL_WRITE_INT32(outputStream, dimensionality);
	    numRings = cpoly->GetInteriorRingCount();
	    FGFUTIL_WRITE_INT32(outputStream, numRings + 1);			// Include exterior ring
	    ring = cpoly->GetExteriorRing();
	    FgfUtil::WriteRing(ring, outputStream);

	    // Interior rings
	    for (i=0; i<numRings; i++)
	    {
		    ring = cpoly->GetInteriorRing(i);
		    FgfUtil::WriteRing(ring, outputStream);
	    }
		break;

	case FdoGeometryType_MultiCurvePolygon:
        mcpoly = static_cast<FdoIMultiCurvePolygon *>(geometry);
	    numSubGeometries = mcpoly->GetCount();
	    FGFUTIL_WRITE_INT32(outputStream, numSubGeometries);
	    for (i=0; i<numSubGeometries; i++)
	    {
		    FdoPtr<FdoICurvePolygon> curvePolygon = mcpoly->GetItem(i);
            WriteGeometry(curvePolygon, outputStream);
	    }
		break;

	case FdoGeometryType_MultiGeometry:
        mgeom = static_cast<FdoIMultiGeometry *>(geometry);
	    numSubGeometries = mgeom->GetCount(); 
	    FGFUTIL_WRITE_INT32(outputStream, numSubGeometries);
	    for (i=0; i<numSubGeometries; i++)
	    {
            FdoPtr<FdoIGeometry> geom = mgeom->GetItem(i);
            WriteGeometry(geom, outputStream);
	    }
		break;

    default:
    	throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_UNKNOWN_GEOMETRY_TYPE), 
                                                               L"FgfUtil::WriteGeometry",
                                                               geometryType));
    }
}

/************************************************************************/
/* Helper: WriteLinearRing                                                                     */
/**********************************************************************/
void FgfUtil::WriteLinearRing(FdoILinearRing* lineRing, FdoByteArray ** outputStream)
{
	// Write the number of positions in this ring
	FdoInt32 numPositions = lineRing->GetCount();
	FGFUTIL_WRITE_INT32(outputStream, numPositions);

	// Now write the positions
	for (FdoInt32 i=0; i<numPositions; i++)
    {
		FdoPtr<FdoIDirectPosition> pos = lineRing->GetItem(i);
		WriteDirectPosition(outputStream, pos);
    }
}


/************************************************************************/
/* Helper: WriteRing                                                                     */
/**********************************************************************/
void FgfUtil::WriteRing(FdoIRing* ring, FdoByteArray ** outputStream)
{
	if ( (NULL == ring) ||
		(NULL == outputStream) )
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_FUNCTION),
                                                               L"FgfUtil::WriteRing",
                                                               L"ring/outputSream"));

	// StartPoint
	FdoPtr<FdoICurveSegmentAbstract> curveSeg = ring->GetItem(0);
	FdoPtr<FdoIDirectPosition> startPos = curveSeg->GetStartPosition();
	WriteDirectPosition(outputStream, startPos);

    // NumElements (segments)
	FdoInt32 numCurveSegs = ring->GetCount();
	FGFUTIL_WRITE_INT32(outputStream, numCurveSegs);

	// CurveSegs
	for (FdoInt32 i=0; i<numCurveSegs; i++)
	{
		FdoPtr<FdoICurveSegmentAbstract> curveSeg = ring->GetItem(i);
		WriteCurveSegment(curveSeg, outputStream);
	} 

}


/************************************************************************/
/* Helper: WriteCurveSegment                                                                     */
/************************************************************************/
void FgfUtil::WriteCurveSegment(FdoICurveSegmentAbstract* curveSeg, FdoByteArray ** outputStream)
{
	/*
	*  Note: This function **doesn't** write the first position
	*  of the segment being written. This is to make it compatible with
	*  memory layout of CurveString FGF.
	*/
	if ( (NULL == curveSeg) ||
		(NULL == outputStream) )
	        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_FUNCTION),
                                                                   L"FgfUtil::WriteCurveSegment",
                                                                   L"curveSeg/outputStream"));

	FdoGeometryComponentType geomType = curveSeg->GetDerivedType();

	// Geometry type
	FGFUTIL_WRITE_INT32(outputStream, geomType);

	switch (geomType)
	{
	case FdoGeometryComponentType_CircularArcSegment:
		{
			// Positions, will be three, but only two
			// will be written (mid and end) because start pos is either
			// stored as member of CurveString or as end point of
			// previous segment
			FdoPtr<FdoIDirectPosition> midPos = ((FdoICircularArcSegment*)curveSeg)->GetMidPoint();
			FdoPtr<FdoIDirectPosition> endPos = ((FdoICircularArcSegment*)curveSeg)->GetEndPosition();
			WriteDirectPosition(outputStream, midPos);
			WriteDirectPosition(outputStream, endPos);
			break;
		}

	case FdoGeometryComponentType_LineStringSegment:
		{
			// Although this segment has numPositions positions,
			// we will write numPositions-1 because its start position
			// has already been written as end point of previous segment
			// and we will be writing numPosition-1 positions in fgf buffer
			FdoInt32 numPositions = ((FdoILineStringSegment*)curveSeg)->GetCount();
			FGFUTIL_WRITE_INT32(outputStream, numPositions-1);

			// Only the points except the startpoint will be written because the startpoint is
			// already stored as member of CurveString or as end point of previous
			// segment NOTE: i==1 below.
			for (FdoInt32 i=1; i<numPositions; i++)
			{
				FdoPtr<FdoIDirectPosition> pos = ((FdoILineStringSegment*)curveSeg)->GetItem(i);
				WriteDirectPosition(outputStream, pos);
			}

			break;
		}
	default:
		{
    		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_UNKNOWN_GEOMETRY_COMPONENT_TYPE),
                                                                   L"FgfUtil::WriteCurveSegment",
                                                                   geomType));
		}
	} // of switch
}


/************************************************************************/
/* Helper: ReadGeometry                                                                     */
/************************************************************************/
FdoIGeometry * FgfUtil::ReadGeometry(FdoFgfGeometryFactory * factory, FdoGeometryType expectedGeometryType, const FdoByte ** inputStream, const FdoByte * streamEnd)
{
    FdoPtr<FdoIGeometry> geometry;
    
    const FdoByte * streamStartOfGeometry = *inputStream;
    FgfUtil::SkipGeometry(inputStream, streamEnd);
    const FdoByte * streamEndOfGeometry = *inputStream;
    FdoInt32 streamSizeOfGeometry = streamEndOfGeometry - streamStartOfGeometry;

    // Copy the FGF data to a new (possibly pooled) byte array.
    // Don't use "FdoPtr" smart pointer while the array is being updated.
    FdoByteArray * newByteArray = factory->GetByteArray();
	newByteArray = FdoByteArray::Append(newByteArray, streamSizeOfGeometry, (FdoByte *)streamStartOfGeometry);

    // Create a new (possibly pooled) geometry from the byte array.
    geometry = factory->CreateGeometryFromFgf(newByteArray);

    if (FdoGeometryType_None != expectedGeometryType &&
        expectedGeometryType != geometry->GetDerivedType())
    	throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_7_INVALIDFGF)));

    FDO_SAFE_RELEASE(newByteArray);
    return FDO_SAFE_ADDREF(geometry.p);
}


FdoIGeometry* FgfUtil::ReadGeometryFromAggregate(FdoFgfGeometryFactory * factory, FdoInt32 Index, FdoGeometryType expectedGeometryType, const FdoByte ** inputStream, const FdoByte * streamEnd)
{
	FGFUTIL_SKIP_INT32S(inputStream, streamEnd, 1);  // Skip over geomtype
    FdoInt32 numSubGeometries = FgfUtil::ReadInt32(inputStream, streamEnd);

	if ( (Index >= numSubGeometries) ||
		(Index < 0) )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INDEXOUTOFBOUNDS)));

    // Skip sub-geometries that precede the desired one.
	for (FdoInt32 i=0; i<Index; i++)
	{
        FgfUtil::SkipGeometry(inputStream, streamEnd);
	}

    FdoPtr<FdoIGeometry> geometry =
        FgfUtil::ReadGeometry(factory, expectedGeometryType, inputStream, streamEnd);

    return FDO_SAFE_ADDREF(geometry.p);
}


void FgfUtil::SkipGeometry(const FdoByte ** inputStream, const FdoByte * streamEnd)
{
    FdoPtr<FdoIGeometry> geometry;
    FdoInt32 dimensionality = 0;
    FdoInt32 numPositions = 0;
    FdoInt32 numOrdsPerPos = 0;
    FdoInt32 numRings = 0;
    FdoInt32 numSubGeometries = 0;
    FdoInt32 numCurveSegments = 0;
    FdoInt32 i;

    FdoInt32 geometryType = FgfUtil::ReadInt32(inputStream, streamEnd);
    switch (geometryType)
    {
	case FdoGeometryType_LineString:
	    dimensionality = FgfUtil::ReadInt32(inputStream, streamEnd);
	    numPositions = FgfUtil::ReadInt32(inputStream, streamEnd);
        numOrdsPerPos = GeometryUtility::DimensionalityToNumOrdinates(dimensionality);
        FGFUTIL_SKIP_DIRECTPOSITIONS(inputStream, streamEnd, numOrdsPerPos, numPositions);
		break;

	case FdoGeometryType_Point:
        dimensionality = FgfUtil::ReadInt32(inputStream, streamEnd);
        numOrdsPerPos = GeometryUtility::DimensionalityToNumOrdinates(dimensionality);
        FGFUTIL_SKIP_DIRECTPOSITIONS(inputStream, streamEnd, numOrdsPerPos, 1);
		break;

	case FdoGeometryType_Polygon:
	    dimensionality = FgfUtil::ReadInt32(inputStream, streamEnd);
        numOrdsPerPos = GeometryUtility::DimensionalityToNumOrdinates(dimensionality);
	    numRings = FgfUtil::ReadInt32(inputStream, streamEnd);
	    for (i=0; i<numRings; i++)
	    {
    		numPositions = FgfUtil::ReadInt32(inputStream, streamEnd);
            FGFUTIL_SKIP_DIRECTPOSITIONS(inputStream, streamEnd, numOrdsPerPos, numPositions);
	    }
		break;

	case FdoGeometryType_CurveString:
	    dimensionality = FgfUtil::ReadInt32(inputStream, streamEnd);
        numOrdsPerPos = GeometryUtility::DimensionalityToNumOrdinates(dimensionality);
	    FGFUTIL_SKIP_DIRECTPOSITIONS(inputStream, streamEnd, numOrdsPerPos, 1); // Start position.
	    numCurveSegments = FgfUtil::ReadInt32(inputStream, streamEnd);
		FgfUtil::SkipCurveSegments(numCurveSegments, dimensionality, inputStream, streamEnd);
		break;

	case FdoGeometryType_CurvePolygon:
	    dimensionality = FgfUtil::ReadInt32(inputStream, streamEnd);
        numOrdsPerPos = GeometryUtility::DimensionalityToNumOrdinates(dimensionality);
	    numRings = FgfUtil::ReadInt32(inputStream, streamEnd);
		FgfUtil::SkipRings(numRings, dimensionality, inputStream, streamEnd);
		break;

	case FdoGeometryType_MultiPoint:
	case FdoGeometryType_MultiLineString:
	case FdoGeometryType_MultiPolygon:
	case FdoGeometryType_MultiCurveString:
	case FdoGeometryType_MultiCurvePolygon:
	case FdoGeometryType_MultiGeometry:
	    numSubGeometries = FgfUtil::ReadInt32(inputStream, streamEnd);
	    for (i=0; i<numSubGeometries; i++)
	    {
            FgfUtil::SkipGeometry(inputStream, streamEnd);    // Recurse.
	    }
		break;

    default:
    	throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_UNKNOWN_GEOMETRY_TYPE),
                                                               L"FgfUtil::SkipGeometry",
                                                               geometryType));
    }
}


/************************************************************************/
/* Helper: ReadCurveSeg                                                                     */
/************************************************************************/
FdoICurveSegmentAbstract* FgfUtil::ReadCurveSegment(
    FdoFgfGeometryFactory * factory,
    FdoInt32 dimensionality,
    FdoIDirectPosition* startPos,
    const FdoByte ** inputStream,
    const FdoByte * streamEnd) 
{
	//
	// Reads CurveSegment positions and creates a new
	// CurveString using the startPos passed followed by positions
	// read from the reader. In this manner it creates and returns a
	// complete curveSeg with all of its positions (unlike its fgf
	// counterpart that has its startPos stored as last position of
	// immediately previous curveSeg.
	//
	// It is assumed that reader is currently pointing at beginning
	// of CurveSeg.
	//

	FdoGeometryComponentType geomType = (FdoGeometryComponentType)ReadInt32(inputStream, streamEnd);

	FdoPtr<FdoICurveSegmentAbstract> curveSeg;

	switch (geomType)
	{
	case FdoGeometryComponentType_CircularArcSegment:
		{
			FdoPtr<FdoIDirectPosition> midPos = ReadDirectPosition(factory, dimensionality, inputStream, streamEnd);
			FdoPtr<FdoIDirectPosition> endPos = ReadDirectPosition(factory, dimensionality, inputStream, streamEnd);
			curveSeg = factory->CreateCircularArcSegment(startPos, midPos, endPos);
			break;
		}

	case FdoGeometryComponentType_LineStringSegment:
		{
			FdoInt32 numPositions = ReadInt32(inputStream, streamEnd);
			FdoPtr<FdoDirectPositionCollection> positions = FdoDirectPositionCollection::Create();

			// add the startPos
			positions->Add(startPos);

			// Now add remaining positions
			for (FdoInt32 i=0; i<numPositions; i++)
			{
				FdoPtr<FdoIDirectPosition> pos = ReadDirectPosition(factory, dimensionality, inputStream, streamEnd);
				positions->Add(pos);
			}

			curveSeg = factory->CreateLineStringSegment(positions);
			break;
		}

	default:
		{
		    throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_UNKNOWN_GEOMETRY_COMPONENT_TYPE),
                                                                   L"FgfUtil::ReadCurveSegment",
                                                                   geomType));
		}
	} // of switch

	return FDO_SAFE_ADDREF(curveSeg.p);
}


/************************************************************************/
/* Helper: SkipCurveRings                                                                     */
/************************************************************************/
void FgfUtil::SkipRings(
    FdoInt32 numRings,
    FdoInt32 dimensionality,
    const FdoByte ** inputStream,
    const FdoByte * streamEnd)
{
	if (numRings == 0)
		return;

    FdoInt32 numOrdsPerPos = GeometryUtility::DimensionalityToNumOrdinates(dimensionality);

	for (FdoInt32 i=1; i<=numRings; i++)
	{
		// Skip over start point
		FGFUTIL_SKIP_DIRECTPOSITIONS(inputStream, streamEnd, numOrdsPerPos, 1);

		// NumSegments
		FdoInt32 numCurveSegments = ReadInt32(inputStream, streamEnd);

		FgfUtil::SkipCurveSegments(numCurveSegments, dimensionality, inputStream, streamEnd);
	}
}


/************************************************************************/
/* Helper: SkipCurveSegments                                                                     */
/* It is assumed that we are already pointed at the beginning of a      */
/* curve segment                                                        */
/************************************************************************/
void FgfUtil::SkipCurveSegments(
    FdoInt32 numSegs,
    FdoInt32 dimensionality,
    const FdoByte ** inputStream,
    const FdoByte * streamEnd)
{
	if (0 == numSegs)
		return;

    FdoInt32 numOrdsPerPos = GeometryUtility::DimensionalityToNumOrdinates(dimensionality);

	for (FdoInt32 i=0; i<numSegs; i++)
	{
		// FdoGeometryType
		FdoGeometryComponentType geomType = (FdoGeometryComponentType)ReadInt32(inputStream, streamEnd);

		FdoInt32 numPositions = 0;

		switch(geomType)
		{
		case FdoGeometryComponentType_CircularArcSegment:
			{
				numPositions = 2;
				break;
			}
		case FdoGeometryComponentType_LineStringSegment:
			{
				numPositions = ReadInt32(inputStream, streamEnd);
				break;
			}
		default:
    		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_UNKNOWN_GEOMETRY_COMPONENT_TYPE),
                                                                   L"FgfUtil::SkipCurveSegments",
                                                                   geomType));
		} // of switch

        FGFUTIL_SKIP_DIRECTPOSITIONS(inputStream, streamEnd, numOrdsPerPos, numPositions);
	} // of for
}

