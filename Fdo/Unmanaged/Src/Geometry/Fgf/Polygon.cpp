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

#include "Polygon.h"
#include "LinearRing.h"
#include <Geometry/EnvelopeImpl.h>
#include "Util.h"


/************************************************************************/
/* Constructor                                                                     */
/************************************************************************/
FdoFgfPolygon::FdoFgfPolygon(
    FdoFgfGeometryFactory * factory,
    FdoILinearRing* exteriorRing,
    FdoLinearRingCollection* interiorRings
    )
    : FdoFgfGeometryImpl<FdoIPolygon>(factory)
{
	if (NULL == exteriorRing)
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));

    FdoByteArray * newByteArray = m_factory->GetByteArray();

	// FdoGeometryType
	FGFUTIL_WRITE_INT32(&newByteArray, FdoGeometryType_Polygon);

	// FdoDimensionality
	FdoInt32 dimensionality = exteriorRing->GetDimensionality();
	FGFUTIL_WRITE_INT32(&newByteArray, dimensionality);

    FdoInt32 numInteriorRings = (NULL==interiorRings) ? 0 : interiorRings->GetCount();

	// NumRings
	FGFUTIL_WRITE_INT32(&newByteArray, 1 + numInteriorRings);		// Include exterior ring

    FgfUtil::WriteLinearRing(exteriorRing, &newByteArray);

	// InteriorRings
	for (FdoInt32 i=0; i<numInteriorRings; i++)
    {
        FdoPtr<FdoILinearRing> ring = interiorRings->GetItem(i);
		FgfUtil::WriteLinearRing(ring, &newByteArray);
    }

    SetFgf(newByteArray, NULL, 0);
    FDO_SAFE_RELEASE(newByteArray);
}

FdoFgfPolygon::FdoFgfPolygon(
    FdoFgfGeometryFactory * factory,
    FdoByteArray * byteArray,
    const FdoByte * data,
    FdoInt32 count
    )
    : FdoFgfGeometryImpl<FdoIPolygon>(factory, byteArray, data, count)
{
}


/************************************************************************/
/* Destructor                                                                     */
/************************************************************************/
FdoFgfPolygon::~FdoFgfPolygon()
{
}


/************************************************************************/
/* Dispose() - dispose ourself                                                                   */
/************************************************************************/
void FdoFgfPolygon::Dispose()
{
	delete this;
}


/************************************************************************/
/* GetEnvelope() - return envelope                                                                     */
/************************************************************************/
FdoIEnvelope* FdoFgfPolygon::GetEnvelope() const
{
	return ComputeEnvelope();	
}

/************************************************************************/
/* GetDimensionType() return dimensionality                                                                     */
/************************************************************************/
FdoInt32 FdoFgfPolygon::GetDimensionality() const
{
	m_streamPtr = m_data;
	FGFUTIL_SKIP_INT32S(&m_streamPtr, m_streamEnd, 1);  // Skip over geomtype
    return FgfUtil::ReadInt32(&m_streamPtr, m_streamEnd);
}


/************************************************************************/
/* GetDerivedType - Get geometrytype                                                                     */
/************************************************************************/
FdoGeometryType FdoFgfPolygon::GetDerivedType() const
{
	return FdoGeometryType_Polygon;
}


/************************************************************************/
/* GetExteriorRing() - return exterior ring                                                                     */
/************************************************************************/
FdoILinearRing* FdoFgfPolygon::GetExteriorRing() const
{
	m_streamPtr = m_data;

	// Skip over geometrytype
	FGFUTIL_SKIP_INT32S(&m_streamPtr, m_streamEnd, 1);

	FdoInt32 dimensionality = FgfUtil::ReadInt32(&m_streamPtr, m_streamEnd);

	// Skip over numRings
	FGFUTIL_SKIP_INT32S(&m_streamPtr, m_streamEnd, 1);

	// First ring is exterior ring

    FdoPtr<FdoILinearRing> lRing = ReadLinearRing(dimensionality, &m_streamPtr, m_streamEnd);

	return FDO_SAFE_ADDREF(lRing.p);
}


/************************************************************************/
/* GetInteriorCount() - return count of interiro rings                                                                     */
/************************************************************************/
FdoInt32 FdoFgfPolygon::GetInteriorRingCount() const
{
	m_streamPtr = m_data;

	// Skip over geometrytype and dimensiontype
	FGFUTIL_SKIP_INT32S(&m_streamPtr, m_streamEnd, 2);

	FdoInt32 totalRingCount = FgfUtil::ReadInt32(&m_streamPtr, m_streamEnd);
	return totalRingCount-1;			// take out exterior ring
}


/************************************************************************/
/* GetInteriorRing() = return interior ring at indexed position                                                                     */
/************************************************************************/
FdoILinearRing* FdoFgfPolygon::GetInteriorRing(FdoInt32 Index) const
{
	m_streamPtr = m_data;

	// First ring is exterior ring

	// Skip over geometrytype
	FGFUTIL_SKIP_INT32S(&m_streamPtr, m_streamEnd, 1);

	// Dimension type
	FdoInt32 dimensionality = FgfUtil::ReadInt32(&m_streamPtr, m_streamEnd);

	// NumRings
	FdoInt32 numRings = FgfUtil::ReadInt32(&m_streamPtr, m_streamEnd);

	// Bounds check
	if (Index > numRings - 2)
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));

	// NumPositions in a ring
	FdoInt32 numPositions = 0;

    FdoInt32 numOrdsPerPos = GeometryUtility::DimensionalityToNumOrdinates(dimensionality);

	FdoInt32 i = 0;
	while (i <= Index)
	{
		numPositions = FgfUtil::ReadInt32(&m_streamPtr, m_streamEnd);
    	FGFUTIL_SKIP_DIRECTPOSITIONS(&m_streamPtr, m_streamEnd, numOrdsPerPos, numPositions);
		i++;
	}

    FdoPtr<FdoILinearRing> lRing = ReadLinearRing(dimensionality, &m_streamPtr, m_streamEnd);

	return FDO_SAFE_ADDREF(lRing.p);
}


/************************************************************************/
/* ReadLinearRing() - Helper to read a linearring from current posiition                                                                     */
/************************************************************************/
FdoILinearRing* FdoFgfPolygon::ReadLinearRing(
    FdoInt32 dimensionality,
    const FdoByte ** inputStream,
    const FdoByte * streamEnd) const
{
    FdoInt32 numPositions = FgfUtil::ReadInt32(inputStream, streamEnd);

    FdoInt32 numDims = GeometryUtility::DimensionalityToNumOrdinates(dimensionality);
	FdoInt32 numOrds = numDims * numPositions;
    FdoInt32 numBytes = numOrds * sizeof(double);

    FGFUTIL_STREAM_CHECK(inputStream, streamEnd, numBytes);

	double * ords = (double *)(*inputStream);
    *inputStream += numBytes;

    FdoPtr<FdoILinearRing> lRing = m_factory->CreateLinearRing(dimensionality, numOrds, ords);

	return FDO_SAFE_ADDREF(lRing.p);
}


/************************************************************************/
/* ComputeEnvelope() - Helper to compute envelope                                                                     */
/************************************************************************/
FdoIEnvelope* FdoFgfPolygon::ComputeEnvelope() const
{
    FdoPtr<FdoEnvelopeImpl> envlImpl = FdoEnvelopeImpl::Create();

	// Skip over geomtype and dimtype
	//m_reader->SkipInts(2);

	FdoInt32 i = 0;
	FdoPtr<FdoILinearRing> extRing = this->GetExteriorRing();
	FdoInt32 numPositions = extRing->GetCount();
	for (i=0; i<numPositions; i++)
	{
		FdoPtr<FdoIDirectPosition> pos = extRing->GetItem(i);
		envlImpl->Expand(pos);
	}

	FdoInt32 numRings = this->GetInteriorRingCount();

	for (i=0; i<numRings; i++)
	{
		FdoPtr<FdoILinearRing> intRing = this->GetInteriorRing(i);
		numPositions = intRing->GetCount();
		for (FdoInt32 j=0; j<numPositions; j++)
		{
			FdoPtr<FdoIDirectPosition> pos = intRing->GetItem(j);
			envlImpl->Expand(pos);
		}
	}

	return FDO_SAFE_ADDREF(envlImpl.p);
}

