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


#include "Point.h"
#include <Geometry/DirectPositionImpl.h>
#include <Geometry/EnvelopeImpl.h>
#include "Util.h"
#include "GeometryThreadData.h"


/************************************************************************/
/* Constructor                                                                     */
/************************************************************************/
FdoFgfPoint::FdoFgfPoint(
        FdoFgfGeometryFactory * factory,
        FdoFgfGeometryPools * pools,
        FdoInt32 dimensionType,
        double* ordinates
        )
    : FdoFgfGeometryImpl<FdoIPoint>(factory, pools)
{
	if ( (NULL == factory) ||
		 (NULL == ordinates) )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_CREATION),
                                                               L"FdoFgfPoint",
                                                               L"ordinates/factory"));

    // Cannot use smart pointer for updating a FdoArray.
    FdoByteArray * newByteArray = FgfUtil::GetPoolsNoRef(m_pools)->GetByteArray();

	FGFUTIL_WRITE_INT32(&newByteArray, FdoGeometryType_Point);
	FGFUTIL_WRITE_INT32(&newByteArray, dimensionType);
	
    FdoInt32 numOrds = GeometryUtility::DimensionalityToNumOrdinates(dimensionType);

    FGFUTIL_WRITE_DOUBLES(&newByteArray, numOrds, ordinates);

    SetFgf(newByteArray, NULL, 0);
    FDO_SAFE_RELEASE(newByteArray);
}


/************************************************************************/
/* Constructor                                                                     */
/************************************************************************/
FdoFgfPoint::FdoFgfPoint(
    FdoFgfGeometryFactory * factory,
    FdoFgfGeometryPools * pools,
    FdoIDirectPosition* position
    )
    : FdoFgfGeometryImpl<FdoIPoint>(factory, pools)
{
	if ( (NULL == factory) ||
		 (NULL == position) )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_CREATION),
                                                               L"FdoFgfPoint",
                                                               L"position/factory"));

    FdoByteArray * newByteArray = FgfUtil::GetPoolsNoRef(m_pools)->GetByteArray();

	FGFUTIL_WRITE_INT32(&newByteArray, FdoGeometryType_Point);
	FGFUTIL_WRITE_INT32(&newByteArray, position->GetDimensionality());
    FgfUtil::WriteDirectPosition(&newByteArray, position);

    SetFgf(newByteArray, NULL, 0);
    FDO_SAFE_RELEASE(newByteArray);
}

FdoFgfPoint::FdoFgfPoint(
    FdoFgfGeometryFactory * factory,
    FdoFgfGeometryPools * pools,
    FdoByteArray * byteArray,
    const FdoByte * data,
    FdoInt32 count
    )
    : FdoFgfGeometryImpl<FdoIPoint>(factory, pools)
{
    Reset(byteArray, data, count);
}


void FdoFgfPoint::Reset(
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
FdoFgfPoint::~FdoFgfPoint()
{
}


/************************************************************************/
/* GetPosition() - return DirectPosition                                                                     */
/************************************************************************/
FdoIDirectPosition* FdoFgfPoint::GetPosition() const
{
	FdoInt32 dimensionality = GetDimensionality();
	m_streamPtr = m_data;
	FGFUTIL_SKIP_INT32S(&m_streamPtr, m_streamEnd, 2);      // Skip over geomtype and dimtype
    FdoPtr<FdoFgfGeometryFactory> gf = GetFactory();
    return FgfUtil::ReadDirectPosition(gf, dimensionality, &m_streamPtr, m_streamEnd);
}

void FdoFgfPoint::GetPositionByMembers(double *x, double *y, double *z, double *m, FdoInt32 *dimensionality) const
{
	*dimensionality = GetDimensionality();
	m_streamPtr = m_data;
	FGFUTIL_SKIP_INT32S(&m_streamPtr, m_streamEnd, 2);      // Skip over geomtype and dimtype
	FGFUTIL_READ_DIRECTPOSITION_BY_MEMBERS(&m_streamPtr, m_streamEnd, *dimensionality, x, y, z, m);
}

const double * FdoFgfPoint::GetOrdinates()
{
	m_streamPtr = m_data;
    // Skip Geometry type and dimensionality.
    FGFUTIL_SKIP_INT32S(&m_streamPtr, m_streamEnd, 2);
    const double * ordinates = (const double *) m_streamPtr;

    return ordinates;
}


/************************************************************************/
/* GweteEnvelope() - return an empty envelope                                                                     */
/************************************************************************/
FdoIEnvelope* FdoFgfPoint::GetEnvelope() const
{
	FdoPtr<FdoIDirectPosition> pos = this->GetPosition();

    FdoPtr<FdoEnvelopeImpl> envelope = 
        FdoEnvelopeImpl::Create(pos->GetX(), pos->GetY(), pos->GetZ(),
		                        pos->GetX(), pos->GetY(), pos->GetZ());

    return FDO_SAFE_ADDREF(envelope.p);
}


/************************************************************************/
/* GetDimensionality() - return dimensionality                                                                      */
/************************************************************************/
FdoInt32 FdoFgfPoint::GetDimensionality() const
{
	m_streamPtr = m_data;
	FGFUTIL_SKIP_INT32S(&m_streamPtr, m_streamEnd, 1);  // Skip over geomtype
    return FgfUtil::ReadInt32(&m_streamPtr, m_streamEnd);
}


/************************************************************************/
/* GetDerivedType() - return the type of this geometry                                                                     */
/************************************************************************/
FdoGeometryType FdoFgfPoint::GetDerivedType() const
{
	return FdoGeometryType_Point;
}


/************************************************************************/
/* Dispose() - dispose ourself                                                                     */
/************************************************************************/
void FdoFgfPoint::Dispose()
{
    SurrenderByteArray();
    FGFUTIL_DISPOSE_TO_POOL_OR_HEAP(Point);
}

