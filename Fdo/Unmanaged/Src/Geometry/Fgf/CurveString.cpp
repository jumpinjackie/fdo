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

#include <float.h>      // For _isnan()
#include <math.h>

#include "CurveString.h"
#include <Geometry/ICircularArcSegment.h>
#include "CircularArcSegment.h"
#include <Geometry/ILineStringSegment.h>
#include "LineStringSegment.h"
#include "Util.h"


/************************************************************************/
/* Constructor                                                                     */
/************************************************************************/
FdoFgfCurveString::FdoFgfCurveString(
    FdoFgfGeometryFactory * factory,
    FdoCurveSegmentCollection* curveSegs
    )
    : FdoFgfGeometryImpl<FdoICurveString>(factory)
{
	if ( (NULL == curveSegs) ||
		  (NULL == factory) ||
          (0 == curveSegs->GetCount()) )
		 throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_CREATION),
                                                                L"FdoFgfCurveString",
                                                                L"curveSegs/factory"));

    FdoByteArray * newByteArray = m_factory->GetByteArray();

	// Geometrytype
	FGFUTIL_WRITE_INT32(&newByteArray, FdoGeometryType_CurveString);

	// Coordtype - from 1st element in curveSegs array
    FdoPtr<FdoICurveSegmentAbstract> firstCurve = curveSegs->GetItem(0);
	FdoInt32 dimensionality = firstCurve->GetDimensionality();
	FGFUTIL_WRITE_INT32(&newByteArray, dimensionality);

	// Startpoint - from 1st element in curveSegs array
	FdoPtr<FdoIDirectPosition> startPos = firstCurve->GetStartPosition();
	FgfUtil::WriteDirectPosition(&newByteArray, startPos);

	// Number of curveSegs
	FGFUTIL_WRITE_INT32(&newByteArray, curveSegs->GetCount());

	// CurveSegs
	for (FdoInt32 i=0; i<curveSegs->GetCount(); i++)
	{
        FdoPtr<FdoICurveSegmentAbstract> curveSeg = curveSegs->GetItem(i);
		FgfUtil::WriteCurveSegment(curveSeg, &newByteArray);
	} 

    SetFgf(newByteArray, NULL, 0);
    FDO_SAFE_RELEASE(newByteArray);
}

FdoFgfCurveString::FdoFgfCurveString(
    FdoFgfGeometryFactory * factory,
    FdoByteArray * byteArray,
    const FdoByte * data,
    FdoInt32 count
    )
    : FdoFgfGeometryImpl<FdoICurveString>(factory, byteArray, data, count)
{
}


/************************************************************************/
/* Destructor                                                                     */
/************************************************************************/
FdoFgfCurveString::~FdoFgfCurveString()
{
}


/************************************************************************/
/* GetEnvelope                                                                     */
/************************************************************************/
FdoIEnvelope* FdoFgfCurveString::GetEnvelope() const
{
	return this->ComputeEnvelope();
}


/************************************************************************/
/* GetDimensionality                                                                     */
/************************************************************************/
FdoInt32 FdoFgfCurveString::GetDimensionality() const
{
	m_streamPtr = m_data;
	FGFUTIL_SKIP_INT32S(&m_streamPtr, m_streamEnd, 1);  // Skip over geomtype
    return FgfUtil::ReadInt32(&m_streamPtr, m_streamEnd);
}


/************************************************************************/
/* GetDerivedType                                                                     */
/************************************************************************/
FdoGeometryType FdoFgfCurveString::GetDerivedType() const
{
	return FdoGeometryType_CurveString;
}

/************************************************************************/
/* GetStartPosition                                                                     */
/************************************************************************/
FdoIDirectPosition* FdoFgfCurveString::GetStartPosition() const
{
	m_streamPtr = m_data;

	// Skip over geometrytype
	FGFUTIL_SKIP_INT32S(&m_streamPtr, m_streamEnd, 1);

	// read FdoDimensionality
	FdoInt32 dimensionality = FgfUtil::ReadInt32(&m_streamPtr, m_streamEnd);
	
	// Read startPos
	return FgfUtil::ReadDirectPosition(m_factory, dimensionality, &m_streamPtr, m_streamEnd);
}

/************************************************************************/
/* GetEndPosition                                                                     */
/************************************************************************/
FdoIDirectPosition* FdoFgfCurveString::GetEndPosition() const
{
	// Get numSegs
	FdoInt32 numSegs = this->GetCount();

	// Get FdoDimensionality
	FdoInt32 dimensionality = this->GetDimensionality();

    FdoInt32 numOrdsPerPos = GeometryUtility::DimensionalityToNumOrdinates(dimensionality);

	// Skip over geomtype, dimtype
	m_streamPtr = m_data;
	FGFUTIL_SKIP_INT32S(&m_streamPtr, m_streamEnd, 2);

	// Skip over startpoint
    FGFUTIL_SKIP_DIRECTPOSITIONS(&m_streamPtr, m_streamEnd, numOrdsPerPos, 1);

	// Skip over numSegs
	FGFUTIL_SKIP_INT32S(&m_streamPtr, m_streamEnd, 1);

	// Skip to end of last segment
    FgfUtil::SkipCurveSegments(numSegs, dimensionality, &m_streamPtr, m_streamEnd);

    m_streamPtr -= numOrdsPerPos * sizeof(double);       // Back up on stream.

	return FgfUtil::ReadDirectPosition(m_factory, dimensionality, &m_streamPtr, m_streamEnd);
}

/************************************************************************/
/* GetIsClosed                                                                     */
/************************************************************************/
bool FdoFgfCurveString::GetIsClosed() const
{
	// Get 1st and last position 
	FdoPtr<FdoIDirectPosition> startPos = this->GetStartPosition();
	FdoPtr<FdoIDirectPosition> endPos = this->GetEndPosition();

    // TODO: the curve isn't really closed if all ordinates are NaN, but rather than
    // fixing it here, a more general validity tests for the whole line should be done.
#ifdef _WIN32
	bool isClosed = ( ((_isnan(startPos->GetX()) && _isnan(endPos->GetX())) || startPos->GetX() == endPos->GetX()) &&
		              ((_isnan(startPos->GetY()) && _isnan(endPos->GetY())) || startPos->GetY() == endPos->GetY()) &&
                      ((_isnan(startPos->GetZ()) && _isnan(endPos->GetZ())) || startPos->GetZ() == endPos->GetZ()) );
#else
	bool isClosed = ( ((isnan(startPos->GetX()) && isnan(endPos->GetX())) || startPos->GetX() == endPos->GetX()) &&
		              ((isnan(startPos->GetY()) && isnan(endPos->GetY())) || startPos->GetY() == endPos->GetY()) &&
                      ((isnan(startPos->GetZ()) && isnan(endPos->GetZ())) || startPos->GetZ() == endPos->GetZ()) );
#endif

	return isClosed;
}


/************************************************************************/
/* GetCount                                                                     */
/************************************************************************/
FdoInt32 FdoFgfCurveString::GetCount() const
{
	// Skip geomtype
	m_streamPtr = m_data;
	FGFUTIL_SKIP_INT32S(&m_streamPtr, m_streamEnd, 1);

	// Read dimtype
	FdoInt32 dimensionality = FgfUtil::ReadInt32(&m_streamPtr, m_streamEnd);

    FdoInt32 numOrdsPerPos = GeometryUtility::DimensionalityToNumOrdinates(dimensionality);

	// Skip startPoint
	FGFUTIL_SKIP_DIRECTPOSITIONS(&m_streamPtr, m_streamEnd, numOrdsPerPos, 1);

	// Now read numElements
	FdoInt32 numSegs = FgfUtil::ReadInt32(&m_streamPtr, m_streamEnd);

	return numSegs;
}


/************************************************************************/
/* GetItem                                                                     */
/************************************************************************/
FdoICurveSegmentAbstract* FdoFgfCurveString::GetItem(FdoInt32 Index) const
{
	FdoInt32 numSegs = this->GetCount();

	if ( (Index >= numSegs) ||
		 (Index < 0) )
		 throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INDEXOUTOFBOUNDS)));

	/*
	 *	Note: The first position for the segment to be returned is
	 *  stored as part of last position of immediately previous segment.
	 *  In case the segment to be returned is the 1st segment, its
	 *  start position is stored as StartPoint member of CurveString
	 */
	
	FdoPtr<FdoIDirectPosition> startPos;

	FdoInt32 dimensionality = this->GetDimensionality();

    FdoInt32 numOrdsPerPos = GeometryUtility::DimensionalityToNumOrdinates(dimensionality);

	// Read start position
	if (0 == Index)
	{
		startPos = this->GetStartPosition();
		
		// Skip to beginning of first segment
		m_streamPtr = m_data;
		
		// Skip over geomtype, dimensionality 
		FGFUTIL_SKIP_INT32S(&m_streamPtr, m_streamEnd, 2);

		// Skip over startpoint
        FGFUTIL_SKIP_DIRECTPOSITIONS(&m_streamPtr, m_streamEnd, numOrdsPerPos, 1);

		// Skip over numElements
		FGFUTIL_SKIP_INT32S(&m_streamPtr, m_streamEnd, 1);
	}
	else
	{
		// Skip over geomtype and dimtype
		m_streamPtr = m_data;
		FGFUTIL_SKIP_INT32S(&m_streamPtr, m_streamEnd, 2);

		// Read Startpos of curvesegment
        FdoPtr<FdoIDirectPosition> startPosPrevSeg =
            FgfUtil::ReadDirectPosition(m_factory, dimensionality, &m_streamPtr, m_streamEnd);

		// Skip numSegments
		FGFUTIL_SKIP_INT32S(&m_streamPtr, m_streamEnd, 1);

		// Skip to curve segment just before the one that we need
        FgfUtil::SkipCurveSegments(Index-1, dimensionality, &m_streamPtr, m_streamEnd);
		FdoPtr<FdoICurveSegmentAbstract> prevCurveSeg =
            FgfUtil::ReadCurveSegment(m_factory, dimensionality, startPosPrevSeg, &m_streamPtr, m_streamEnd);

		// And Read its last position
		// which will be startPos for the segment
		// we'll return
		startPos = prevCurveSeg->GetEndPosition();
	}

	// Read the CurveSegment we are interested in
	// We are already pointing at it
	FdoPtr<FdoICurveSegmentAbstract> thisCurveSeg = FgfUtil::ReadCurveSegment(m_factory, dimensionality, startPos, &m_streamPtr, m_streamEnd);

	return FDO_SAFE_ADDREF(thisCurveSeg.p);
}


/************************************************************************/
/* GetCurveSegments                                                                     */
/************************************************************************/
FdoCurveSegmentCollection* FdoFgfCurveString::GetCurveSegments() const
{
	// Get count
	FdoInt32 numSegs = this->GetCount();

    FdoPtr<FdoCurveSegmentCollection> curveSegs = FdoCurveSegmentCollection::Create();

	for (FdoInt32 i=0; i<numSegs; i++)
	{
		FdoPtr<FdoICurveSegmentAbstract> seg = this->GetItem(i);
        curveSegs->Add(seg);
	}

	return FDO_SAFE_ADDREF(curveSegs.p);
}


/************************************************************************/
/* Dispose                                                                     */
/************************************************************************/
void FdoFgfCurveString::Dispose()
{
	delete this;
}



/************************************************************************/
/* Helper: ComputeEnvelope                                                                     */
/************************************************************************/
FdoIEnvelope* FdoFgfCurveString::ComputeEnvelope() const
{
    FdoPtr<FdoEnvelopeImpl> envlImpl = FdoEnvelopeImpl::Create();

	FdoInt32 numSegments = this->GetCount();

	for (FdoInt32 i=0; i<numSegments; i++)
	{
		FdoPtr<FdoICurveSegmentAbstract> curveSeg = this->GetItem(i);
		FdoPtr<FdoIEnvelope> envlTmp = curveSeg->GetEnvelope();
		envlImpl->Expand(envlTmp);
	}

	return FDO_SAFE_ADDREF(envlImpl.p);
}

