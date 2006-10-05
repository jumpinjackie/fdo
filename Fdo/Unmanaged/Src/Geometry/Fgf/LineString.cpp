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

#include "LineString.h"
#include "Util.h"
#include <Geometry/DirectPositionImpl.h>


/************************************************************************/
/* Constructor                                                                     */
/************************************************************************/
FdoFgfLineString::FdoFgfLineString(
    FdoFgfGeometryFactory * factory, 
    FdoDirectPositionCollection* positions
    )
    : FdoFgfGeometryImpl<FdoILineString>(factory), m_previousPositionIndex(-1)
{
    Reset(positions);
}

void FdoFgfLineString::Reset(FdoDirectPositionCollection* positions)
{
	if ( (NULL == positions) ||
		 (positions->GetCount() < 1) )
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_FUNCTION),
                                                               L"FdoFgfLineString::Reset",
                                                               L"positions"));

    FdoByteArray * newByteArray = m_factory->GetByteArray();

    m_previousPositionIndex = -1;

	// Geometry type
	FGFUTIL_WRITE_INT32(&newByteArray, FdoGeometryType_LineString);

	// Dimension from 1st position
	FdoPtr<FdoIDirectPosition> startPos = positions->GetItem(0);
    FGFUTIL_WRITE_INT32(&newByteArray, startPos->GetDimensionality());

	// NumPoints
    FdoInt32 numPositions = positions->GetCount();
	FGFUTIL_WRITE_INT32(&newByteArray, numPositions);

	for (FdoInt32 i=0; i<numPositions; i++)
    {
        FdoPtr<FdoIDirectPosition> pos = positions->GetItem(i);
		FgfUtil::WriteDirectPosition(&newByteArray, pos);
    }

    SetFgf(newByteArray, NULL, 0);
    FDO_SAFE_RELEASE(newByteArray);
}



/************************************************************************/
/* Constructor                                                                     */
/************************************************************************/
FdoFgfLineString::FdoFgfLineString(
    FdoFgfGeometryFactory * factory,
    FdoInt32 dimensionType,
    FdoInt32 numOrdinates,
    double * ordinates
    )
    : FdoFgfGeometryImpl<FdoILineString>(factory), m_previousPositionIndex(-1)
{
    Reset(dimensionType, numOrdinates, ordinates);
}

void FdoFgfLineString::Reset(FdoInt32 dimensionType, FdoInt32 numOrdinates, double* ordinates)
{
    FdoByteArray * newByteArray = m_factory->GetByteArray();

    m_previousPositionIndex = -1;

    FdoInt32 numDims = GeometryUtility::DimensionalityToNumOrdinates(dimensionType);
    FdoInt32 numPositions = numOrdinates / numDims;

	FGFUTIL_WRITE_INT32(&newByteArray, FdoGeometryType_LineString);
	FGFUTIL_WRITE_INT32(&newByteArray, dimensionType);
	FGFUTIL_WRITE_INT32(&newByteArray, numPositions);

    FGFUTIL_WRITE_DOUBLES(&newByteArray, numOrdinates, ordinates);

    SetFgf(newByteArray, NULL, 0);
    FDO_SAFE_RELEASE(newByteArray);
}

/************************************************************************/
/* Constructor                                                                     */
/************************************************************************/
FdoFgfLineString::FdoFgfLineString(
    FdoFgfGeometryFactory * factory,
    FdoByteArray * byteArray,
    const FdoByte * data,
    FdoInt32 count
    )
    : FdoFgfGeometryImpl<FdoILineString>(factory, byteArray, data, count), m_previousPositionIndex(-1)
{
}


/************************************************************************/
/* Destructor                                                                     */
/************************************************************************/
FdoFgfLineString::~FdoFgfLineString()
{
}


/************************************************************************/
/* GetCount() : Return number of points in this linestring                                                                     */
/************************************************************************/
FdoInt32 FdoFgfLineString::GetCount() const
{
    m_previousPositionIndex = -1;
	m_streamPtr = m_data;
	FGFUTIL_SKIP_INT32S(&m_streamPtr, m_streamEnd, 2);      // Skip over geomtype and dimtype
    return FgfUtil::ReadInt32(&m_streamPtr, m_streamEnd);	// # of positions;
}


/************************************************************************/
/* GetDerivedType() : Return geometry type for this linestring                                                                     */
/************************************************************************/
FdoGeometryType FdoFgfLineString::GetDerivedType() const
{
	return FdoGeometryType_LineString;
}


/************************************************************************/
/* GetDimensionality() : Return dimensionality for this geometry                                                                     */
/************************************************************************/
FdoInt32 FdoFgfLineString::GetDimensionality() const
{
    m_previousPositionIndex = -1;
	m_streamPtr = m_data;
	FGFUTIL_SKIP_INT32S(&m_streamPtr, m_streamEnd, 1);  // Skip over geomtype
    return FgfUtil::ReadInt32(&m_streamPtr, m_streamEnd);
}


/************************************************************************/
/* GetEnvelope() : Return Envelope for this geometry                                                                     */
/************************************************************************/
FdoIEnvelope* FdoFgfLineString::GetEnvelope() const
{
    m_previousPositionIndex = -1;
	return this->ComputeEnvelope();
}


/************************************************************************/
/* GetIsClosed() : Return true/false if this geometry is closed/not                                                                     */
/************************************************************************/
bool FdoFgfLineString::GetIsClosed() const
{
    m_previousPositionIndex = -1;
	FdoPtr<FdoIDirectPosition> start = this->GetStartPosition();
	FdoPtr<FdoIDirectPosition> end = this->GetEndPosition();

    // TODO: the line isn't really closed if al ordinates are NaN, but rather than
    // fixing it here, a more general validity tests for the whole line should be done.
#ifdef _WIN32
	bool isEqual = ( (_isnan(start->GetX()) && _isnan(end->GetX())) || start->GetX() == end->GetX() ) &&
		           ( (_isnan(start->GetY()) && _isnan(end->GetY())) || start->GetY() == end->GetY() ) &&
				   ( (_isnan(start->GetZ()) && _isnan(end->GetZ())) || start->GetZ() == end->GetZ() );
#else
	bool isEqual = ( (isnan(start->GetX()) && isnan(end->GetX())) || start->GetX() == end->GetX() ) &&
		           ( (isnan(start->GetY()) && isnan(end->GetY())) || start->GetY() == end->GetY() ) &&
				   ( (isnan(start->GetZ()) && isnan(end->GetZ())) || start->GetZ() == end->GetZ() );
#endif

	return isEqual;
}


/************************************************************************/
/* GetItem() : Get DirectPosition at indexed position                                                                     */
/************************************************************************/
FdoIDirectPosition* FdoFgfLineString::GetItem(FdoInt32 index)	const	// 0 based
{
	// TODO: Check out of bounds index

    m_previousPositionIndex = -1;
	m_streamPtr = m_data;
	FGFUTIL_SKIP_INT32S(&m_streamPtr, m_streamEnd, 1);		// Geometry type
	FdoInt32 dimension =  FgfUtil::ReadInt32(&m_streamPtr, m_streamEnd);
	FdoInt32 numPositions = FgfUtil::ReadInt32(&m_streamPtr, m_streamEnd);
	if ( index > numPositions-1 )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INDEXOUTOFBOUNDS)));

    FdoInt32 numOrdsPerPos = GeometryUtility::DimensionalityToNumOrdinates(dimension);

	FGFUTIL_SKIP_DIRECTPOSITIONS(&m_streamPtr, m_streamEnd, numOrdsPerPos, index);

	FdoPtr<FdoIDirectPosition> pos = FgfUtil::ReadDirectPosition(m_factory, dimension, &m_streamPtr, m_streamEnd);

	return FDO_SAFE_ADDREF(pos.p);
}


void FdoFgfLineString::GetItemByMembers(FdoInt32 index, double *x, double *y, double *z, double *m, FdoInt32 *dimensionality) const
{
	if ( -1 == m_previousPositionIndex || 
		 index-1 != m_previousPositionIndex )
	{
    	m_streamPtr = m_data;
	    FGFUTIL_SKIP_INT32S(&m_streamPtr, m_streamEnd, 1);		// Geometry type
        *dimensionality = FgfUtil::ReadInt32(&m_streamPtr, m_streamEnd);
        FdoInt32 numPositions = FgfUtil::ReadInt32(&m_streamPtr, m_streamEnd);
        if ( index > numPositions-1 )
			throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INDEXOUTOFBOUNDS)));

        FdoInt32 numOrdsPerPos = GeometryUtility::DimensionalityToNumOrdinates(*dimensionality);

    	FGFUTIL_SKIP_DIRECTPOSITIONS(&m_streamPtr, m_streamEnd, numOrdsPerPos, index);
	}

	FGFUTIL_READ_DIRECTPOSITION_BY_MEMBERS(&m_streamPtr, m_streamEnd, *dimensionality, x, y, z, m);

	m_previousPositionIndex = index;
}


/************************************************************************/
/* GetPositions() - Return array of DirectPositions that define this geometry                                                                     */
/************************************************************************/
FdoDirectPositionCollection* FdoFgfLineString::GetPositions()
{
    m_previousPositionIndex = -1;
	FdoInt32 count = this->GetCount();
    FdoPtr<FdoDirectPositionCollection> positions = FdoDirectPositionCollection::Create();
	for (FdoInt32 i = 0;  i < count;  i++)
    {
		FdoPtr<FdoIDirectPosition> pos = this->GetItem(i);
		positions->Add(pos);
	}

	return FDO_SAFE_ADDREF(positions.p);
}

const double * FdoFgfLineString::GetOrdinates()
{
	m_streamPtr = m_data;
    // Skip Geometry type, dimensionality and position count.
    FGFUTIL_SKIP_INT32S(&m_streamPtr, m_streamEnd, 3);
    const double * ordinates = (const double *) m_streamPtr;
	m_previousPositionIndex = -1;

    return ordinates;
}


/************************************************************************/
/* GetStartPosition() : Return first DirectPosition of this linestring                                                                     */
/************************************************************************/
FdoIDirectPosition* FdoFgfLineString::GetStartPosition() const
{
	return this->GetItem(0);
}


/************************************************************************/
/* GetEndPosition() - Return last DirectPosition of this linestring                                                                     */
/************************************************************************/
FdoIDirectPosition* FdoFgfLineString::GetEndPosition() const
{
	return this->GetItem(this->GetCount() - 1);
}


/************************************************************************/
/* ComputeEnvelope() - Computes envelope for this linestring                                                                     */
/************************************************************************/
FdoEnvelopeImpl* FdoFgfLineString::ComputeEnvelope() const
{
	FdoInt32 pointCount = this->GetCount();
    FdoPtr<FdoEnvelopeImpl> envlImpl = FdoEnvelopeImpl::Create();

	for (FdoInt32 i=0; i<pointCount; i++)
	{
		FdoPtr<FdoIDirectPosition> pos = this->GetItem(i);
		envlImpl->Expand(pos);
	}

	return FDO_SAFE_ADDREF(envlImpl.p);
}

void FdoFgfLineString::SetFgf(FdoByteArray * fgf, const FdoByte * fgfData, FdoInt32 count)
{
    m_previousPositionIndex = -1;
    FdoFgfGeometryImpl<FdoILineString>::SetFgf(fgf, fgfData, count);
}

void FdoFgfLineString::Dispose()
{
	delete this;
}

