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

#include "MultiPoint.h"
#include <Geometry/DirectPositionImpl.h>
#include <Geometry/EnvelopeImpl.h>
#include "Util.h"


/************************************************************************/
/* Constructor                                                                     */
/************************************************************************/
FdoFgfMultiPoint::FdoFgfMultiPoint(
    FdoFgfGeometryFactory * factory,
    FdoInt32 dimensionality,
    FdoInt32 numOrdinates, 
    double* ordinates
    )
    : FdoFgfGeometryImpl<FdoIMultiPoint>(factory),
      m_ordinates(NULL)
{
	if ( (NULL == ordinates) ||
		 (NULL == factory) )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));

    FdoByteArray * newByteArray = m_factory->GetByteArray();

	FdoPtr<FdoDirectPositionImpl> pos = FdoDirectPositionImpl::Create();
	pos->SetDimensionality(dimensionality);

	FdoInt32 numDims = GeometryUtility::DimensionalityToNumOrdinates(dimensionality);
	FdoInt32 numPositions = numOrdinates / numDims;

	FGFUTIL_WRITE_INT32(&newByteArray, FdoGeometryType_MultiPoint);
	FGFUTIL_WRITE_INT32(&newByteArray, numPositions);

	FdoInt32 i = 0;
	while (i < numOrdinates)
	{
		FGFUTIL_WRITE_INT32(&newByteArray, FdoGeometryType_Point);
		FGFUTIL_WRITE_INT32(&newByteArray, dimensionality);
		pos->SetX(ordinates[i++]);
		pos->SetY(ordinates[i++]);
		if (dimensionality & FdoDimensionality_Z)
		{
			pos->SetZ(ordinates[i++]);
			if (dimensionality & FdoDimensionality_M)
				pos->SetM(ordinates[i++]);
		}
		else if(dimensionality & FdoDimensionality_M)
			pos->SetM(ordinates[i++]);
		FgfUtil::WriteDirectPosition(&newByteArray, pos);
	}

    SetFgf(newByteArray, NULL, 0);
    FDO_SAFE_RELEASE(newByteArray);
}

/************************************************************************/
/* Constructor                                                                     */
/************************************************************************/
FdoFgfMultiPoint::FdoFgfMultiPoint(
    FdoFgfGeometryFactory * factory,
    FdoPointCollection* points
    )
    : FdoFgfGeometryImpl<FdoIMultiPoint>(factory),
      m_ordinates(NULL)
{
	if ( (NULL == points) ||
        (0 == points->GetCount()) ||
		(NULL == factory) )
	    throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));

    FdoByteArray * newByteArray = m_factory->GetByteArray();

	FGFUTIL_WRITE_INT32(&newByteArray, FdoGeometryType_MultiPoint);

    FdoInt32 numPoints = points->GetCount();

	FGFUTIL_WRITE_INT32(&newByteArray, numPoints);

	for (FdoInt32 i=0; i<numPoints; i++)
	{
		FGFUTIL_WRITE_INT32(&newByteArray, FdoGeometryType_Point);
        FdoPtr<FdoIPoint> point = points->GetItem(i);
		FGFUTIL_WRITE_INT32(&newByteArray, point->GetDimensionality());
        FdoPtr<FdoIDirectPosition> pos = point->GetPosition();
		FgfUtil::WriteDirectPosition(&newByteArray, pos);
	}

    SetFgf(newByteArray, NULL, 0);
    FDO_SAFE_RELEASE(newByteArray);
}

FdoFgfMultiPoint::FdoFgfMultiPoint(
    FdoFgfGeometryFactory * factory,
    FdoByteArray * byteArray,
    const FdoByte * data,
    FdoInt32 count
    )
    : FdoFgfGeometryImpl<FdoIMultiPoint>(factory, byteArray, data, count),
      m_ordinates(NULL)
{
}


/************************************************************************/
/* Destructor                                                                     */
/************************************************************************/
FdoFgfMultiPoint::~FdoFgfMultiPoint()
{
    if (NULL != m_ordinates)
    {
        delete [] m_ordinates;
        m_ordinates = NULL;
    }
}


/************************************************************************/
/* Dispose                                                                     */
/************************************************************************/
void FdoFgfMultiPoint::Dispose()
{
	delete this;
}

/************************************************************************/
/* GetEnvelope                                                                     */
/************************************************************************/
FdoIEnvelope* FdoFgfMultiPoint::GetEnvelope() const
{
	return ComputeEnvelope();
}


/************************************************************************/
/* GetDimensionality                                                                     */
/************************************************************************/
FdoInt32 FdoFgfMultiPoint::GetDimensionality() const
{
	// Skip GeomType, NumPoints, geomtype (of Point)
	m_streamPtr = m_data;
	FGFUTIL_SKIP_INT32S(&m_streamPtr, m_streamEnd, 3);

	return FgfUtil::ReadInt32(&m_streamPtr, m_streamEnd);
}

/************************************************************************/
/* GetDerivedType                                                                     */
/***********************************************************************/
FdoGeometryType FdoFgfMultiPoint::GetDerivedType() const
{
	return FdoGeometryType_MultiPoint;
}


/************************************************************************/
/* GetItem                                                                     */
/************************************************************************/
FdoIPoint* FdoFgfMultiPoint::GetItem(FdoInt32 Index) const
{
	m_streamPtr = m_data;

    FdoPtr<FdoIGeometry> geometry =
        FgfUtil::ReadGeometryFromAggregate(m_factory, Index, FdoGeometryType_Point, &m_streamPtr, m_streamEnd);

    // Smart pointer assignments have problems with non-identical types.  Work around...
    FdoIPoint * derivedGeometry = static_cast<FdoIPoint *>(geometry.p);
    FdoPtr<FdoIPoint> value = FDO_SAFE_ADDREF(derivedGeometry);
    return FDO_SAFE_ADDREF(value.p);
}

const double * FdoFgfMultiPoint::GetOrdinates()
{
    FdoInt32 numPositions = GetCount();
    FdoInt32 dimensionality = GetDimensionality();
    bool hasZ = (dimensionality&FdoDimensionality_Z) != 0;
    bool hasM = (dimensionality&FdoDimensionality_M) != 0;

    if (NULL == m_ordinates)
    {
        FdoInt32 numDims = GeometryUtility::DimensionalityToNumOrdinates(GetDimensionality());
        FdoInt32 numOrds = numDims * numPositions;
        m_ordinates = new double[numOrds];
        if (NULL == m_ordinates)
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));
    }

    FdoInt32 ordIndex = 0;

    for (FdoInt32 i=0;  i<numPositions;  i++)
    {
        FdoPtr<FdoIPoint> point = GetItem(i);
        double x, y, z, m;
        FdoInt32 dim;
        point->GetPositionByMembers(&x, &y, &z, &m, &dim);
        m_ordinates[ordIndex++] = x;
        m_ordinates[ordIndex++] = y;
        if (hasZ)
            m_ordinates[ordIndex++] = z;
        if (hasM)
            m_ordinates[ordIndex++] = m;
    }

    return m_ordinates;
}


/************************************************************************/
/* GetCount                                                                     */
/************************************************************************/
FdoInt32 FdoFgfMultiPoint::GetCount() const
{
	m_streamPtr = m_data;
	FGFUTIL_SKIP_INT32S(&m_streamPtr, m_streamEnd, 1);  // Skip over geomtype

    return FgfUtil::ReadInt32(&m_streamPtr, m_streamEnd);
}


/************************************************************************/
/* Helper: ComputeEnvelope                                                                     */
/************************************************************************/
FdoIEnvelope* FdoFgfMultiPoint::ComputeEnvelope() const
{
	FdoInt32 pointCount = this->GetCount();
    FdoPtr<FdoEnvelopeImpl> envlImpl = FdoEnvelopeImpl::Create();

	for (FdoInt32 i=0; i<pointCount; i++)
	{
        FdoPtr<FdoIPoint>          point = this->GetItem(i);
		FdoPtr<FdoIDirectPosition> pos = point->GetPosition();
		envlImpl->Expand(pos);
	}

	return FDO_SAFE_ADDREF(envlImpl.p);
}

