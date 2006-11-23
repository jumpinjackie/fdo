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

#include <Geometry/EnvelopeImpl.h>
#include "CurvePolygon.h"
#include "Util.h"


/************************************************************************/
/* Constructor                                                                     */
/************************************************************************/
FdoFgfCurvePolygon::FdoFgfCurvePolygon(
    FdoFgfGeometryFactory * factory,
    FdoIRing* exteriorRing,
    FdoRingCollection* interiorRings
    )
    : FdoFgfGeometryImpl<FdoICurvePolygon>(factory)
{
	if (NULL == exteriorRing)
         throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_CREATION),
                                                                L"FdoFgfCurvePolygon",
                                                                L"exteriorRing"));

    // Cannot use smart pointer for updating a FdoArray.
    FdoByteArray * newByteArray = m_factory->GetByteArray();

	// FdoGeometryType
	FGFUTIL_WRITE_INT32(&newByteArray, FdoGeometryType_CurvePolygon);

	// FdoDimensionality
	FdoInt32 dimensionality = exteriorRing->GetDimensionality();
	FGFUTIL_WRITE_INT32(&newByteArray, dimensionality);

    FdoInt32 numInteriorRings = (NULL==interiorRings) ? 0 : interiorRings->GetCount();

	// NumRings
	FGFUTIL_WRITE_INT32(&newByteArray, numInteriorRings + 1);		// Include exterior ring

	// Exterior ring
	FgfUtil::WriteRing(exteriorRing, &newByteArray);

	// Interior rings
	for (FdoInt32 i=0; i<numInteriorRings; i++)
    {
        FdoPtr<FdoIRing> ring = interiorRings->GetItem(i);
		FgfUtil::WriteRing(ring, &newByteArray);
    }

    SetFgf(newByteArray, NULL, 0);
    FDO_SAFE_RELEASE(newByteArray);
}

FdoFgfCurvePolygon::FdoFgfCurvePolygon(
    FdoFgfGeometryFactory * factory,
    FdoByteArray * byteArray,
    const FdoByte * data,
    FdoInt32 count
    )
    : FdoFgfGeometryImpl<FdoICurvePolygon>(factory)
{
    Reset(byteArray, data, count);
}

void FdoFgfCurvePolygon::Reset(
    FdoByteArray * byteArray,
    const FdoByte * data,
    FdoInt32 count
    )
{
    SetFgf(byteArray, data, count);
}


/************************************************************************/
/* Destructor                                                                     */
/************************************************************************/
FdoFgfCurvePolygon::~FdoFgfCurvePolygon()
{
}


/************************************************************************/
/* GetEnvelope                                                                     */
/************************************************************************/
FdoIEnvelope* FdoFgfCurvePolygon::GetEnvelope() const
{
	return this->ComputeEnvelope();
}


/************************************************************************/
/* GetDimensionality										                                   */
/************************************************************************/
FdoInt32 FdoFgfCurvePolygon::GetDimensionality() const
{
	m_streamPtr = m_data;
	FGFUTIL_SKIP_INT32S(&m_streamPtr, m_streamEnd, 1);  // Skip over geomtype
    return FgfUtil::ReadInt32(&m_streamPtr, m_streamEnd);
}

/************************************************************************/
/* GetDerivedType                                                                     */
/************************************************************************/
FdoGeometryType FdoFgfCurvePolygon::GetDerivedType() const
{
	return FdoGeometryType_CurvePolygon;
}


/************************************************************************/
/* Helper: ComputeEnvelope                                                                     */
/************************************************************************/
FdoIEnvelope* FdoFgfCurvePolygon::ComputeEnvelope() const
{
    FdoPtr<FdoEnvelopeImpl> envlImpl = FdoEnvelopeImpl::Create();

    FdoPtr<FdoIRing> extRing = this->GetExteriorRing();
	FdoPtr<FdoIEnvelope> extEnvl = extRing->GetEnvelope();
	envlImpl->Expand(extEnvl);

	FdoInt32 numIntRings = this->GetInteriorRingCount();
	for (FdoInt32 i =0; i<numIntRings; i++)
	{
		FdoPtr<FdoIRing> intRing = this->GetInteriorRing(i);
		FdoPtr<FdoIEnvelope> intEnvl = intRing->GetEnvelope();
		envlImpl->Expand(intEnvl);
	}

	return FDO_SAFE_ADDREF(envlImpl.p);
}


/************************************************************************/
/* GetExteriorRing                                                                     */
/************************************************************************/
FdoIRing* FdoFgfCurvePolygon::GetExteriorRing() const
{
	m_streamPtr = m_data;
	FGFUTIL_SKIP_INT32S(&m_streamPtr, m_streamEnd, 1);  // Skip over geomtype

	// read FdoDimensionality
	FdoInt32 dimensionality = FgfUtil::ReadInt32(&m_streamPtr, m_streamEnd);

	// Skip over NumCurveRings
	FGFUTIL_SKIP_INT32S(&m_streamPtr, m_streamEnd, 1);

	// Read first CurveRing
	FdoPtr<FdoIRing> curveRing = ReadRing(dimensionality, &m_streamPtr, m_streamEnd);

	return FDO_SAFE_ADDREF(curveRing.p);
}


/************************************************************************/
/* GetInteriorRingCount                                                                     */
/************************************************************************/
FdoInt32 FdoFgfCurvePolygon::GetInteriorRingCount() const
{
	// Skip over Geomtype and DimType
	m_streamPtr = m_data;
	FGFUTIL_SKIP_INT32S(&m_streamPtr, m_streamEnd, 2);

	// Read numCurveRings
	FdoInt32 numInteriorRings = FgfUtil::ReadInt32(&m_streamPtr, m_streamEnd);

	return numInteriorRings - 1;		// Exclude exterior ring from count

}

/************************************************************************/
/* GetInteriorRing                                                                     */
/************************************************************************/
FdoIRing* FdoFgfCurvePolygon::GetInteriorRing(FdoInt32 Index) const
{
	m_streamPtr = m_data;

	// First ring is ExteriorRing

	// Skip over GeomType
	FGFUTIL_SKIP_INT32S(&m_streamPtr, m_streamEnd, 1);

	// FdoDimensionality
	FdoInt32 dimensionality = FgfUtil::ReadInt32(&m_streamPtr, m_streamEnd);

	// NumCurveRings
	FdoInt32 numCurveRings = FgfUtil::ReadInt32(&m_streamPtr, m_streamEnd);

	// Bounds check
	if (Index > numCurveRings - 2)
		 throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_FUNCTION),
                                                                L"FdoFgfCurvePolygon::GetInteriorRing",
                                                                L"Index"));

	// Skip to CurveRing
	FgfUtil::SkipRings(Index+1, dimensionality, &m_streamPtr, m_streamEnd);

	// And read it
	FdoPtr<FdoIRing> curveRing = ReadRing(dimensionality, &m_streamPtr, m_streamEnd);

	return FDO_SAFE_ADDREF(curveRing.p);

}


/************************************************************************/
/* ReadRing                                                                     */
/************************************************************************/
FdoIRing* FdoFgfCurvePolygon::ReadRing(
    FdoInt32 dimensionality,
    const FdoByte ** inputStream,
    const FdoByte * streamEnd ) const
{
	// StartPosition
	FdoPtr<FdoIDirectPosition> startPos =
        FgfUtil::ReadDirectPosition(m_factory, dimensionality, inputStream, streamEnd);

	// NumRings
	FdoInt32 numCurveSegments = FgfUtil::ReadInt32(inputStream, streamEnd);

	// CurveSegments
    FdoPtr<FdoCurveSegmentCollection> curveSegs = FdoCurveSegmentCollection::Create();

	for (FdoInt32 i=0; i<numCurveSegments; i++)
	{
		FdoPtr<FdoICurveSegmentAbstract> curveSeg =
            FgfUtil::ReadCurveSegment(m_factory, dimensionality, startPos, inputStream, streamEnd);
        curveSegs->Add(curveSeg);
		startPos = curveSeg->GetEndPosition();
	}

	FdoPtr<FdoIRing> ring = m_factory->CreateRing(curveSegs);

	return FDO_SAFE_ADDREF(ring.p);
}


/************************************************************************/
/* Dispose()                                                                     */
/************************************************************************/
void FdoFgfCurvePolygon::Dispose()
{
	delete this;
}

